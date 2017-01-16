#include "stdafx.h"
#include "ZiBlue.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include "ZiBlueFactory.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values
IMPLEMENT_PLUGIN(CZiBlue)


CZiBlue::CZiBlue()
{
}

CZiBlue::~CZiBlue()
{
}

// Event IDs
enum
{
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId, // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kEvtPortDataReceived,
   kProtocolErrorRetryTimer,
   kAnswerTimeout,
};


void CZiBlue::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      YADOMS_LOG(information) << "CZiBlue is initializing" ;
      api->setPluginState(yApi::historization::EPluginState::kCustom, "Initialization");

      // Load configuration values (provided by database)
      m_configuration.initializeWith(api->getConfiguration());

      //create connection
      createConnection(api->getEventHandler());

      m_transceiver = boost::make_shared<CZiBlueTransceiver>();

      m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(5));
      m_waitForAnswerTimer->stop();

      
      YADOMS_LOG(information) << "CZiBlue is running" ;
      api->setPluginState(yApi::historization::EPluginState::kRunning);
      while (1)
      {
         // Wait for an event
         switch (api->getEventHandler().waitForEvents())
         {
         case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested" ;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }
         case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // Command was received from Yadoms
            auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
            YADOMS_LOG(information) << "Command received from Yadoms :" << yApi::IDeviceCommand::toString(command) ;
            try
            {
            }
            catch (shared::exception::CException& ex)
            {
               YADOMS_LOG(error) << "Fail to send command : " << ex.what() ;
            }
            catch (std::exception& ex)
            {
               YADOMS_LOG(error) << "Fail to send command. exception : " << ex.what() ;
            }
            catch (...)
            {
               YADOMS_LOG(error) << "Fail to send command. unknown exception" ;
            }
            break;
         }
         case yApi::IYPluginApi::kGetDeviceConfigurationSchemaRequest:
         {
            // Yadoms ask for device configuration schema
            // Schema can come from package.json, or built by code. In this example,
            // we just take the schema from package.json, in case of configuration is supported by device.
            auto deviceConfigurationSchemaRequest = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IDeviceConfigurationSchemaRequest> >();
            break;
         }

         case yApi::IYPluginApi::kSetDeviceConfiguration:
         {
            // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
            auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration> >();
            break;
         }

         case yApi::IYPluginApi::kEventExtraQuery:
         {
            // Command was received from Yadoms
            auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery> >();
            extraQuery->sendSuccess(shared::CDataContainer());
            break;
         }

         case yApi::IYPluginApi::kEventUpdateConfiguration:
            break;

         case kEvtPortConnection:
         {
            if (api->getEventHandler().getEventData<bool>())
               processZiBlueConnectionEvent(api);
            else
               processZiBlueUnConnectionEvent(api);

            break;
         }
         case kEvtPortDataReceived:
         {
            processZiBlueDataReceived(api, api->getEventHandler().getEventData<const shared::communication::CStringBuffer>());
            break;
         }
         case kAnswerTimeout:
         {
            YADOMS_LOG(error) << "No answer received, try to reconnect in a while...";
            errorProcess(api);
            break;
         }
         case kProtocolErrorRetryTimer:
         {
            createConnection(api->getEventHandler());
            break;
         }

         default:
            YADOMS_LOG(error) << "Unknown message id" ;
            break;
         }
      }
   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "The ZiBlue plugin fails. shared exception : " << ex.what() ;
      api->setPluginState(yApi::historization::EPluginState::kError, (boost::format("Shared exception : %1%") % ex.what()).str());
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "The ZiBlue plugin fails. exception : " << ex.what() ;
      api->setPluginState(yApi::historization::EPluginState::kError, (boost::format("Exception : %1%") % ex.what()).str());
   }
   catch (...)
   {
      YADOMS_LOG(error) << "The ZiBlue plugin fails. unknown exception" ;
      api->setPluginState(yApi::historization::EPluginState::kError, "Unknown exception");
   }

}


void CZiBlue::createConnection(shared::event::CEventHandler& eventHandler)
{
   // Create the port instance
   m_port = CZiBlueFactory::constructPort(m_configuration, eventHandler, kEvtPortConnection, kEvtPortDataReceived);
   m_port->start();
}

void CZiBlue::destroyConnection()
{
   m_port.reset();

   m_waitForAnswerTimer->stop();
}


void CZiBlue::send(boost::shared_ptr<yApi::IYPluginApi> api, const shared::communication::CStringBuffer& buffer, bool needAnswer)
{
   if (!m_port)
      return;

   
   //if (m_isDeveloperMode)
   //   m_logger.logSent(buffer);

   m_port->send((unsigned char*)buffer.begin(), buffer.size());

   if (needAnswer)
      m_waitForAnswerTimer->start();
}

void CZiBlue::send(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<std::queue<shared::communication::CStringBuffer > > buffers)
{
   if (!m_port)
      return;

   // Send all messages
   while (!buffers->empty())
   {
      send(api, buffers->front(), false);
      buffers->pop();
   }
}

void CZiBlue::processZiBlueConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "ZiBlue port opened";
   api->setPluginState(yApi::historization::EPluginState::kCustom, "initializing");

   try
   {
      // Reset the ZiBlue
      initZiBlue(api);
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Error resetting ZiBlue transceiver : " << e.what();

      // Stop the communication, and try later
      errorProcess(api);
   }
}

void CZiBlue::errorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   m_port.reset();
   api->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CZiBlue::processZiBlueUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "ZiBlue connection was lost";
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connectionLost");

   errorProcess(api);
}

void CZiBlue::processZiBlueDataReceived(boost::shared_ptr<yApi::IYPluginApi> api, const shared::communication::CStringBuffer& data)
{
//   if (m_isDeveloperMode)
//      m_logger.logReceived(data);

   // Message was recognized, stop timeout
   m_waitForAnswerTimer->stop();

}

void CZiBlue::initZiBlue(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Send hello command to the ZiBlue
   send(api, m_transceiver->buildHelloCmd(), true);

   // ZiBlue needs some time to recover after reset (see specifications)
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
}