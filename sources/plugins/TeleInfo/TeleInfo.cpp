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
   kAnswerTimeout
};

void CTeleInfo::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Teleinfo is starting..." ;

   m_isDeveloperMode = api->getYadomsInformation()->developperMode();

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   // Create the transceiver
   m_decoder = CTeleInfoFactory::constructDecoder(api);

   // Create the buffer handler
   m_receiveBufferHandler = CTeleInfoFactory::GetBufferHandler(api->getEventHandler(),
                                                               kEvtPortDataReceived);

   m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                             shared::event::CEventTimer::kOneShot,
                                                             boost::posix_time::seconds(45));

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

            if (api->getEventHandler().getEventData<bool>())
               processTeleInfoConnectionEvent(api);
            else
            {
               processTeleInfoUnConnectionEvent(api);

               // attempt a new connection
               api->getEventHandler().createTimer(kErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
            }
            break;
         }
      case kEvtPortDataReceived:
         {
			 m_waitForAnswerTimer->stop();

            if (m_isDeveloperMode) YADOMS_LOG(information) << "TeleInfo plugin :  DataReceived" ;

            processDataReceived(api,
                                api->getEventHandler().getEventData<boost::shared_ptr<std::map<std::string, std::string>>>());

            if (m_decoder->isERDFCounterDesactivated())
            {
               if (m_runningState != kErDFCounterdesactivated)
               {
                  api->setPluginState(yApi::historization::EPluginState::kCustom, "ErDFCounterdesactivated");
                  m_runningState = kErDFCounterdesactivated;
               }
            }

			//Lauch a new time the time out to detect connexion failure
			m_waitForAnswerTimer->start();

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
      case kAnswerTimeout:
         {
            m_waitForAnswerTimer->stop();
            YADOMS_LOG(error) << "No answer received, try to reconnect in a while..." ;
            errorProcess(api);
            break;
         }
      default:
         {
            YADOMS_LOG(error) << "Unknown message id" ;
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
   m_decoder->decodeTeleInfoMessage(api, messages);

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

void CTeleInfo::processTeleInfoUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "TeleInfo connection was lost" ;
   api->setPluginState(yApi::historization::EPluginState::kError, "connectionLost");
   m_runningState = kConnectionLost;

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