#include "stdafx.h"
#include "TeleInfo.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/communication/PortException.hpp>
#include "TeleInfoFactory.h"
#include <shared/Log.h>
#include "GPIOManager.hpp"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CTeleInfo)

CTeleInfo::CTeleInfo():
   m_isDeveloperMode(false),
   m_runningState(ETeleInfoPluginState::kUndefined)
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

   m_configuration = boost::make_shared<CTeleInfoConfiguration>();

   // Load configuration values (provided by database)
   m_configuration->initializeWith(api->getConfiguration());

   m_GPIOManager = boost::make_shared<CGPIOManager>(m_configuration);

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
            setPluginState(api, ETeleInfoPluginState::kStop);
            return;
         }
      case kEvtPortConnection:
         {
            YADOMS_LOG(information) << "Teleinfo plugin :  Port Connection" ;
            setPluginState(api, ETeleInfoPluginState::kConnecting);

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

            m_receiveBufferHandler->desactivate();

            CTeleInfoFactory::FTDI_DisableGPIO(m_port);

            if (!m_GPIOManager->isLast())
            {
               // Go to the next GPIO
               m_GPIOManager->next();
               api->getEventHandler().createTimer(kSamplingTimer,
                                                  shared::event::CEventTimer::kOneShot,
                                                  boost::posix_time::seconds(0));
            }
            else // Return to the beginning of the list
               m_GPIOManager->front();

            break;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            setPluginState(api, ETeleInfoPluginState::kupdateConfiguration);
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
            setPluginState(api, ETeleInfoPluginState::kRunning);

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
            // Activate the port
            CTeleInfoFactory::FTDI_ActivateGPIO(m_port, m_GPIOManager->getGPIO());
            m_receiveBufferHandler->activate();

            //Lauch a new time the time out to detect connexion failure
            m_waitForAnswerTimer->start();
            break;
         }
      case kAnswerTimeout:
         {
            m_waitForAnswerTimer->stop();
            m_periodicSamplingTimer->stop();
            CTeleInfoFactory::FTDI_DisableGPIO(m_port);

            YADOMS_LOG(error) << "No answer received, try to reconnect in a while...";
            errorProcess(api);
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
   setPluginState(api, ETeleInfoPluginState::kConnecting);
   // Create the port instance

   try {
      m_port = CTeleInfoFactory::constructPort(*m_configuration,
                                               api->getEventHandler(),
                                               m_receiveBufferHandler,
                                               kEvtPortConnection);
      m_port->start();
      m_waitForAnswerTimer->start();
   }
   catch (std::exception &e)
   {
      YADOMS_LOG(error) << e.what();
   }
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
      m_configuration->initializeWith(newConfigurationData);
      return;
   }

   // Port has changed, destroy and recreate connection
   destroyConnection();

   // Update configuration
   m_configuration->initializeWith(newConfigurationData);

   // Update GPIOManager
   m_GPIOManager = boost::make_shared<CGPIOManager>(m_configuration);

   // Create new connection
   createConnection(api);
}

void CTeleInfo::processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const boost::shared_ptr<std::map<std::string, std::string>>& messages)
{
   m_decoder[m_GPIOManager->getGPIO()-1]->decodeTeleInfoMessage(api, messages);
   setPluginState(api, ETeleInfoPluginState::kRunning);

   if (m_decoder[m_GPIOManager->getGPIO() - 1]->isERDFCounterDesactivated())
   {
      if (m_runningState != kErDFCounterdesactivated)
      {
         setPluginState(api, ETeleInfoPluginState::kErDFCounterdesactivated);
      }
   }
}

void CTeleInfo::processTeleInfoConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   YADOMS_LOG(information) << "TeleInfo port opened" ;

   try
   {
      // Init The TeleInfo Receiver
      initTeleInfoReceiver();
      
      // Restart timer and fire immediately a sampling period
      m_periodicSamplingTimer->start();
      api->getEventHandler().createTimer(kSamplingTimer,
                                         shared::event::CEventTimer::kOneShot,
                                         boost::posix_time::seconds(0));
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
      setPluginErrorState(api, notification->getErrorMessageI18n(), notification->getErrorMessageI18nParameters());
   else
      setPluginState(api, ETeleInfoPluginState::kConnectionLost);

   m_receiveBufferHandler->desactivate();

   destroyConnection();
}

void CTeleInfo::errorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   setPluginState(api, ETeleInfoPluginState::kConnectionLost);
   destroyConnection();
   api->getEventHandler().createTimer(kErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CTeleInfo::initTeleInfoReceiver() const
{
   YADOMS_LOG(information) << "Reset the TeleInfo..." ;

   // Flush receive buffer
   m_port->flush();
}

void CTeleInfo::setPluginState(boost::shared_ptr<yApi::IYPluginApi> api, ETeleInfoPluginState newState)
{
   if (m_runningState != newState)
   {
      switch (newState)
      {
      case ETeleInfoPluginState::kErDFCounterdesactivated:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "ErDFCounterdesactivated");
         break;
      case ETeleInfoPluginState::kupdateConfiguration:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "updateconfiguration");
         break;
      case ETeleInfoPluginState::kConnectionLost:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "connectionLost");
         break;
      case ETeleInfoPluginState::kConnecting:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");
         break;
      case ETeleInfoPluginState::kRunning:
         api->setPluginState(yApi::historization::EPluginState::kRunning);
         break;
      case ETeleInfoPluginState::kStop:
         api->setPluginState(yApi::historization::EPluginState::kStopped);
         break;
      default:
         YADOMS_LOG(error) << "this plugin status does not exist : " << newState;
         break;
      }

      m_runningState = newState;
   }
}

void CTeleInfo::setPluginErrorState(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const std::string& ErrorMessageI18n,
                                    const std::map<std::string, std::string>& ErrorMessageI18nParameters)
{
   if (m_runningState != kError)
   {
      m_runningState = kError;
      api->setPluginState(yApi::historization::EPluginState::kError, ErrorMessageI18n, ErrorMessageI18nParameters);
   }
}