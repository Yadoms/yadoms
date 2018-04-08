#include "stdafx.h"
#include "TeleInfo.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/communication/PortException.hpp>
#include "TeleInfoFactory.h"
#include <shared/Log.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CTeleInfo)

CTeleInfo::CTeleInfo():
   m_isDeveloperMode(false),
   m_runningState(ETeleInfoPluginState::kUndefined),
   m_scanPort(0)
{
}

CTeleInfo::~CTeleInfo()
{
}

// Event IDs
enum
{
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtPortDataReceived,
   kErrorRetryTimer,
   kAnswerTimeout,
   kSamplingTimer
};

void CTeleInfo::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Teleinfo is starting..." ;

   m_isDeveloperMode = api->getYadomsInformation()->developperMode();

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   // Create the transceiver
   m_decoder[0] = CTeleInfoFactory::constructDecoder(api);
   m_decoder[1] = CTeleInfoFactory::constructDecoder(api);

   // Create the buffer handler
   m_receiveBufferHandler = CTeleInfoFactory::GetBufferHandler(api->getEventHandler(),
                                                               kEvtPortDataReceived,
                                                               m_isDeveloperMode);

   m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                             shared::event::CEventTimer::kOneShot,
                                                             boost::posix_time::seconds(20));

   m_waitForAnswerTimer->stop();

   m_periodicSamplingTimer = api->getEventHandler().createTimer(kSamplingTimer,
                                                                shared::event::CEventTimer::kPeriodic,
                                                                boost::posix_time::seconds(30));

   // Fire immediately a sampling time
   api->getEventHandler().createTimer(kSamplingTimer,
                                      shared::event::CEventTimer::kOneShot,
                                      boost::posix_time::seconds(0));

   // Create the connection
   createConnection(api);

   // the main loop
   YADOMS_LOG(information) << "Teleinfo plugin is running..." ;

   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested" ;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            m_runningState = kStop;
            return;
         }
      case kEvtPortConnection:
         {
            YADOMS_LOG(information) << "Teleinfo plugin :  Port Connection" ;
            api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");
            m_runningState = kConnecting;

            auto notif = api->getEventHandler().getEventData<boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>>();

            if (notif && notif->isConnected())
               processTeleInfoConnectionEvent(api);
            else
            {
               processTeleInfoUnConnectionEvent(api, notif);

               // attempt a new connection
               api->getEventHandler().createTimer(kErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
            }
            break;
         }
      case kEvtPortDataReceived:
         {
			 m_waitForAnswerTimer->stop();
          YADOMS_LOG(trace) << "TeleInfo plugin : DataReceived";

          processDataReceived(api,
                              api->getEventHandler().getEventData<boost::shared_ptr<std::map<std::string, std::string>>>());

            if (m_decoder[m_scanPort]->isERDFCounterDesactivated())
            {
               if (m_runningState != kErDFCounterdesactivated)
               {
                  api->setPluginState(yApi::historization::EPluginState::kCustom, "ErDFCounterdesactivated");
                  m_runningState = kErDFCounterdesactivated;
               }
            }

            m_receiveBufferHandler->desactivate();
            m_port->desactivateGPIO();

            if (m_scanPort==0 && 
               (m_configuration.getEquipmentType() == TwoInputs) && 
               ((m_configuration.getInputsActivated() == Input1Activated) ||
               (m_configuration.getInputsActivated() == AllInputsActivated)))
            {
               m_scanPort = 1;

               api->getEventHandler().createTimer(kSamplingTimer,
                                                  shared::event::CEventTimer::kOneShot,
                                                  boost::posix_time::seconds(0));
            }

            break;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            m_runningState = kupdateConfiguration;
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
            api->setPluginState(yApi::historization::EPluginState::kRunning);

            break;
         }
      case kErrorRetryTimer:
         {
            createConnection(api);
            break;
         }
      case kSamplingTimer:
         {
            // Initial port to scan
            /* TODO : faire un object spécifique
            if (m_configuration.getInputsActivated() == Input1Activated ||
                m_configuration.getInputsActivated() == AllInputsActivated)
            {
               m_scanPort = 0;
            }
            else
               m_scanPort = 1;
            */
            // Activate the port
            m_port->activateGPIO(m_scanPort + 1);
            m_receiveBufferHandler->activate();

            //Lauch a new time the time out to detect connexion failure
            m_waitForAnswerTimer->start();
            break;
         }
      case kAnswerTimeout:
         {
            m_waitForAnswerTimer->stop();
            m_port->desactivateGPIO();

            YADOMS_LOG(error) << "No answer received, try to reconnect in a while..." ;

            if (m_scanPort == 0 &&
               (m_configuration.getEquipmentType() == TwoInputs) &&
                ((m_configuration.getInputsActivated() == Input1Activated) ||
                (m_configuration.getInputsActivated() == AllInputsActivated)))
            {
               m_scanPort = 1;

               api->getEventHandler().createTimer(kSamplingTimer,
                                                  shared::event::CEventTimer::kOneShot,
                                                  boost::posix_time::seconds(0));
            }
            else {
               errorProcess(api);
            }
            break;
         }
      default:
         {
            YADOMS_LOG(error) << "Unknown message id " << api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

void CTeleInfo::createConnection(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");
   // Create the port instance
   m_port = CTeleInfoFactory::constructPort(m_configuration,
                                            api->getEventHandler(),
                                            m_receiveBufferHandler,
                                            kEvtPortConnection);
   m_port->start();
   m_waitForAnswerTimer->start();
}

void CTeleInfo::destroyConnection()
{
   m_port.reset();
   m_waitForAnswerTimer->stop();
}

void CTeleInfo::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                      const shared::CDataContainer& newConfigurationData)
{
   // Stop running timers, if any
   m_waitForAnswerTimer->stop();

   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration..." ;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // If plugin instance is not running, just update configuration
   if (!m_port)
   {
      // Update configuration
      m_configuration.initializeWith(newConfigurationData);
      return;
   }

   // Port has changed, destroy and recreate connection
   destroyConnection();

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);

   // Create new connection
   createConnection(api);
}

void CTeleInfo::processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const boost::shared_ptr<std::map<std::string, std::string>>& messages)
{
   m_decoder[m_scanPort]->decodeTeleInfoMessage(api, messages);

   if (m_runningState != kRunning)
   {
      api->setPluginState(yApi::historization::EPluginState::kRunning);
      m_runningState = kRunning;
   }
}

void CTeleInfo::processTeleInfoConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   YADOMS_LOG(information) << "TeleInfo port opened" ;

   try
   {
      // Init The TeleInfo Receiver
      initTeleInfoReceiver();
   }
   catch (shared::communication::CPortException& e)
   {
      YADOMS_LOG(error) << "Error connecting to TeleInfo transceiver : " << e.what() ;
      // Disconnection will be notified, we just have to wait...
   }
}

void CTeleInfo::processTeleInfoUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification> notification)
{
   YADOMS_LOG(information) << "TeleInfo connection was lost" ;
   if (notification)
      api->setPluginState(yApi::historization::EPluginState::kError, notification->getErrorMessageI18n(), notification->getErrorMessageI18nParameters());
   else
      api->setPluginState(yApi::historization::EPluginState::kError, "connectionLost");
   m_runningState = kConnectionLost;

   m_receiveBufferHandler->desactivate();

   destroyConnection();
}

void CTeleInfo::errorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kError, "connectionLost");
   destroyConnection();
   m_runningState = kConnectionLost;
   api->getEventHandler().createTimer(kErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CTeleInfo::initTeleInfoReceiver() const
{
   YADOMS_LOG(information) << "Reset the TeleInfo..." ;

   // Flush receive buffer
   m_port->flush();
}