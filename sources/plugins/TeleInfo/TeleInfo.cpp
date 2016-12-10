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


CTeleInfo::CTeleInfo()
   : m_logger(std::cout),
     m_isDeveloperMode(false),
     m_runningState(false)
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
   kEvtTimerRefreshTeleInfoData,
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
   m_transceiver = CTeleInfoFactory::constructTransceiver(api);

   // Create the buffer handler
   m_receiveBufferHandler = CTeleInfoFactory::GetBufferHandler(api->getEventHandler(),
                                                               kEvtPortDataReceived,
                                                               512);

   m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                             shared::event::CEventTimer::kOneShot,
                                                             boost::posix_time::seconds(15));
   m_waitForAnswerTimer->stop();

   // Create the connection
   createConnection(api);

   // the main loop
   std::cout << "Teleinfo plugin is running..." << std::endl;

   // Timer used to read periodically information
   api->getEventHandler().createTimer(kEvtTimerRefreshTeleInfoData,
                                      shared::event::CEventTimer::kPeriodic,
                                      boost::posix_time::seconds(30));

   while (1)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            std::cout << "Stop requested" << std::endl;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }
      case kEvtPortConnection:
         {
            std::cout << "Teleinfo plugin :  Port Connection" << std::endl;
            api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

            if (api->getEventHandler().getEventData<bool>())
               processTeleInfoConnectionEvent(api);
            else
               processTeleInfoUnConnectionEvent(api);

            break;
         }
      case kEvtPortDataReceived:
         {
         if (m_isDeveloperMode) std::cout << "TeleInfo plugin :  DataReceived" << std::endl;

            processDataReceived(api,
                                api->getEventHandler().getEventData<const shared::communication::CByteBuffer>());

            break;
         }
      case kEvtTimerRefreshTeleInfoData:
         {
            // When received this timer, we restart the reception through the serial port
         if (m_isDeveloperMode) std::cout << "Teleinfo plugin :  Resume COM" << std::endl;
            m_transceiver->ResetRefreshTags();
            m_receiveBufferHandler->resume();

            //Lauch a new time the time out to detect connexion failure
            m_waitForAnswerTimer->start();

            break;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
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

   // Stop running reception, if any
   m_receiveBufferHandler->suspend();

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
                                    const shared::communication::CByteBuffer& data)
{
   // Stop timeout
   m_waitForAnswerTimer->stop();

   if (m_isDeveloperMode)
      m_logger.logReceived(data);

   m_transceiver->decodeTeleInfoMessage(api, data);

   // When all information are updated we stopped the reception !
   if (m_transceiver->IsInformationUpdated())
   {
      if (m_isDeveloperMode) std::cout << "Suspend COM" << std::endl;
      m_receiveBufferHandler->suspend();
   }

   auto newState = true;
   if (m_runningState != newState)
   {
      api->setPluginState(yApi::historization::EPluginState::kRunning);
      m_runningState = true;
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

   destroyConnection();
}

void CTeleInfo::errorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kError, "connectionLost");
   destroyConnection();
   api->getEventHandler().createTimer(kErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CTeleInfo::initTeleInfoReceiver() const
{
   std::cout << "Reset the TeleInfo..." << std::endl;

   // Flush receive buffer
   m_port->flush();
}

