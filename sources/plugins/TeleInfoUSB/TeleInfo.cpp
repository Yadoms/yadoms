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
   kAnswerTimeout
};

void CTeleInfo::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      std::cout << "Teleinfo is starting..." << std::endl;
      api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

      // Load configuration values (provided by database)
      m_configuration.initializeWith(api->getConfiguration());

      // Create the transceiver
      m_transceiver = CTeleInfoFactory::constructTransceiver();

      // Create the buffer handler
      m_receiveBufferHandler = CTeleInfoFactory::GetBufferHandler(api->getEventHandler(),
                                                                  kEvtPortDataReceived);

      m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                                shared::event::CEventTimer::kOneShot,
                                                                boost::posix_time::seconds(5));
      m_waitForAnswerTimer->stop();

      // Create the connection
      createConnection(api);

      std::cout << "Teleinfo is running..." << std::endl;

      // the main loop
      std::cout << "Teleinfo plugin is running..." << std::endl;

      // Timer used to read periodically information
      api->getEventHandler().createTimer(kEvtTimerRefreshTeleInfoData,
                                         shared::event::CEventTimer::kPeriodic,
                                         boost::posix_time::seconds(15));

      while (1)
      {
         // Wait for an event
         switch (api->getEventHandler().waitForEvents())
         {
         case kEvtPortConnection:
            {
               std::cout << "Teleinfo plugin :  Port Connection" << std::endl;

               if (api->getEventHandler().getEventData<bool>())
                  processTeleInfoConnectionEvent(api);
               else
                  processTeleInfoUnConnectionEvent(api);

               break;
            }
         case kEvtPortDataReceived:
            {
               std::cout << "TeleInfo plugin :  DataReceived" << std::endl;

               processDataReceived(api,
                                   api->getEventHandler().getEventData<const shared::communication::CByteBuffer>());

               break;
            }
         case kEvtTimerRefreshTeleInfoData:
            {
               // When received this timer, we restart the reception through the serial port
               std::cout << "Teleinfo plugin :  Resume COM" << std::endl;
               m_transceiver->ResetRefreshTags();
               m_receiveBufferHandler->resume();
               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
               api->setPluginState(yApi::historization::EPluginState::kRunning);

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
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
      std::cout << "TeleInfo is stopping..." << std::endl;
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
   m_logger.logReceived(data);

   m_transceiver->decodeTeleInfoMessage(api, data);

   // When all information are updated we stopped the reception !
   if (m_transceiver->IsInformationUpdated())
   {
      std::cout << "Suspend COM" << std::endl;
      m_receiveBufferHandler->suspend();
   }
}

void CTeleInfo::processTeleInfoConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   std::cout << "TeleInfo port opened" << std::endl;
   api->setPluginState(yApi::historization::EPluginState::kRunning);

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

void CTeleInfo::send(const shared::communication::CByteBuffer& buffer, bool needAnswer)
{
   if (!m_port)
      return;

   m_logger.logSent(buffer);
   m_port->send(buffer);
   if (needAnswer)
      m_waitForAnswerTimer->start();
}

void CTeleInfo::initTeleInfoReceiver() const
{
   std::cout << "Reset the TeleInfo..." << std::endl;

   // TeleInfo Receiver needs some time to recover after reset (see specifications)
   boost::this_thread::sleep(boost::posix_time::milliseconds(50));

   // Flush receive buffer
   m_port->flush();
}

