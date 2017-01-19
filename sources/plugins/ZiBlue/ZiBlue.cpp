#include "stdafx.h"
#include "ZiBlue.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/communication/BufferLogger.h>
#include "Factory.h"
#include "MessageHandler.h"

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
   kEvtPortFrameReceived,
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

      m_transceiver = boost::make_shared<CTransceiver>();

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
         case kEvtPortFrameReceived:
         {
            boost::shared_ptr<const frames::CFrame> frame = api->getEventHandler().getEventData< boost::shared_ptr<const frames::CFrame> >();
            if(frame->isAscii())
               processZiBlueAsciiFrameReceived(api, frame->getAscii());
            else
               processZiBlueBinaryFrameReceived(api, frame->getBinary());
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
   m_port = CFactory::constructPort(m_configuration);

   m_messageHandler = CFactory::constructMessageHandler(m_port, eventHandler, kEvtPortFrameReceived);

   m_port->subscribeForConnectionEvents(eventHandler, kEvtPortConnection);

   m_port->setReceiveBufferHandler(CFactory::constructReceiveBufferHandler(m_messageHandler));

   m_port->start();
}

void CZiBlue::destroyConnection()
{
   m_port.reset();
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

void CZiBlue::processZiBlueBinaryFrameReceived(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<frames::CBinaryFrame> data)
{
   if (m_isDeveloperMode)
      YADOMS_LOG(debug) << "ZiBlue Binary <<< " << shared::communication::CBufferLogger::byteBufferToHexString(*data->getContent());
}

void CZiBlue::processZiBlueAsciiFrameReceived(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<frames::CAsciiFrame> data)
{
   if (m_isDeveloperMode)
      YADOMS_LOG(debug) << "ZiBlue Ascii <<< " << data->getContent();
}



void CZiBlue::initZiBlue(boost::shared_ptr<yApi::IYPluginApi> api)
{
   if (!m_messageHandler->send(m_transceiver->buildHelloCmd(),
      [](boost::shared_ptr<const frames::CFrame> frame)
      {
         return frame->isAscii() && boost::icontains(frame->getAscii()->getContent(), "Ziblue Dongle");
      },
      [&](boost::shared_ptr<const frames::CFrame> frame)
      {
         //manage HELLO answer
         YADOMS_LOG(information) << "Dongle info  :" << frame->getAscii()->getContent();

         m_messageHandler->send(m_transceiver->buildStartListeningData());
      }))
      throw shared::exception::CException("Unable to send HELLO request, timeout waiting acknowledge");
}


