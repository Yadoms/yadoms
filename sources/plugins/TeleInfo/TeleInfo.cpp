#include "stdafx.h"
#include "TeleInfo.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/communication/PortException.hpp>
#include "TeleInfoFactory.h"

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
   std::cout << "Teleinfo is starting..." << std::endl;

   m_isDeveloperMode = api->getYadomsInformation()->developperMode();

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   // Create the transceiver
   m_decoder = CTeleInfoFactory::constructDecoder(api);

   // Create the buffer handler
   m_receiveBufferHandler = CTeleInfoFactory::GetBufferHandler(api->getEventHandler(),
                                                               kEvtPortDataReceived,
															                  m_isDeveloperMode);

   m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                             shared::event::CEventTimer::kOneShot,
                                                             boost::posix_time::seconds(45));

   // Create the connection
   createConnection(api);

   // the main loop
   std::cout << "Teleinfo plugin is running..." << std::endl;

   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            std::cout << "Stop requested" << std::endl;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            m_runningState = kStop;
            return;
         }
      case kEvtPortConnection:
         {
            std::cout << "Teleinfo plugin :  Port Connection" << std::endl;
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

            if (m_isDeveloperMode) std::cout << "TeleInfo plugin :  DataReceived" << std::endl;

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
            std::cerr << "No answer received, try to reconnect in a while..." << std::endl;
            errorProcess(api);
            break;
         }
      default:
         {
            std::cerr << "Unknown message id" << std::endl;
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
   std::cout << "Update configuration..." << std::endl;
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
   std::cout << "TeleInfo port opened" << std::endl;

   try
   {
      // Init The TeleInfo Receiver
      initTeleInfoReceiver();
   }
   catch (shared::communication::CPortException& e)
   {
      std::cerr << "Error connecting to TeleInfo transceiver : " << e.what() << std::endl;
      // Disconnection will be notified, we just have to wait...
   }
}

void CTeleInfo::processTeleInfoUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "TeleInfo connection was lost" << std::endl;
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
   std::cout << "Reset the TeleInfo..." << std::endl;

   // Flush receive buffer
   m_port->flush();
}