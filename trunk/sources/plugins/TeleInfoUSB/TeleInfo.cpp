#include "stdafx.h"
#include "TeleInfo.h"
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/plugin/ImplementationHelper.h>
#include <shared/communication/PortException.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include "TeleInfoFactory.h"
#include "boost/date_time/posix_time/posix_time.hpp"

//
#include "TeleInfotrxHelpers.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CTeleInfo)


CTeleInfo::CTeleInfo(): m_deviceName("TeleInfoUSB")
{
}

CTeleInfo::~CTeleInfo()
{
}

// Event IDs
enum
{
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtPortDataReceived,
   kAnswerTimeout
};

void CTeleInfo::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{

   try
   {
      YADOMS_LOG(debug) << "Teleinfo is starting...";

      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

      // Create the transceiver
      m_transceiver = CTeleInfoFactory::constructTransceiver();

      m_waitForAnswerTimer = context->getEventHandler().createTimer(kAnswerTimeout, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(5));
      m_waitForAnswerTimer->stop();

      // Create the connection
      createConnection(context->getEventHandler());

      YADOMS_LOG(debug) << "Teleinfo is running...";

      // the main loop
      YADOMS_LOG(debug) << "Teleinfo plugin is running...";

	   if (!context->deviceExists(m_deviceName))
	   {
		  std::string m_URL = m_deviceName;

		  context->declareDevice(m_deviceName, m_URL);
	   }

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {		 
         case kEvtPortConnection:
            {
               YADOMS_LOG(debug) << "Teleinfo plugin :  Port Connection";

               if (context->getEventHandler().getEventData<bool>())
                  processTeleInfoConnectionEvent(context);
               else
                  processTeleInfoUnConnectionEvent(context);

               break;
            }
         case kEvtPortDataReceived:
            {
               YADOMS_LOG(debug) << "TeleInfo plugin :  DataReceived";

               processDataReceived(context,
								   context->getEventHandler().getEventData<const shared::communication::CByteBuffer>());

               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               onUpdateConfiguration(context, context->getEventHandler().getEventData<shared::CDataContainer>());

               break;
            }
         default:
            {
               YADOMS_LOG(error) << "Unknown message id";
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
      YADOMS_LOG(information) << "TeleInfo is stopping..."  << std::endl;
   }
}

void CTeleInfo::createConnection(shared::event::CEventHandler& eventHandler)
{
   // Create the port instance
   m_port = CTeleInfoFactory::constructPort(m_configuration, eventHandler, kEvtPortConnection, kEvtPortDataReceived);
   m_port->setReceiveBufferMaxSize(TeleInfoMESSAGE_maxSize);
   m_port->start();
}

void CTeleInfo::destroyConnection()
{
   m_port.reset();

   m_waitForAnswerTimer->stop();
}

void CTeleInfo::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> context, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(debug) << "Configuration was updated...";
   BOOST_ASSERT(!newConfigurationData.empty());  // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

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
   createConnection(context->getEventHandler());
}

void CTeleInfo::processDataReceived(boost::shared_ptr<yApi::IYPluginApi> context, 
									const shared::communication::CByteBuffer& data)
{
   m_logger.logReceived(data);

   m_transceiver->decodeTeleInfoMessage(context,
	                                    m_deviceName,
	        							data);
}

void CTeleInfo::processTeleInfoConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> context)
{
   YADOMS_LOG(debug) << "TeleInfo is now connected";
   context->setPluginState(yApi::historization::EPluginState::kRunning);

   try
   {
      // Init The TeleInfo Receiver
      initTeleInfoReceiver();
   }
   catch (shared::communication::CPortException& e)
   {
      YADOMS_LOG(error) << "Error connecting to TeleInfo transceiver : " << e.what();
      // Disconnection will be notified, we just have to wait...
   }
}

void CTeleInfo::processTeleInfoUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> context)
{
   YADOMS_LOG(debug) << "TeleInfo connection was lost";
   context->setPluginState(yApi::historization::EPluginState::kError, "connectionLost");

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

void CTeleInfo::initTeleInfoReceiver()
{
   YADOMS_LOG(information) << "Reset the TeleInfo...";

   // TeleInfo Receiver needs some time to recover after reset (see specifications)
   boost::this_thread::sleep(boost::posix_time::milliseconds(50));

   // Flush receive buffer
   m_port->flush();
}