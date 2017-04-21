#include "stdafx.h"
#include "RfPlayer.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/communication/BufferLogger.h>
#include "Factory.h"
#include "MessageHandler.h"
#include <shared/encryption/Base64.h>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values
IMPLEMENT_PLUGIN(CRfPlayer)


CRfPlayer::CRfPlayer()
{
}

CRfPlayer::~CRfPlayer()
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


void CRfPlayer::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      YADOMS_LOG(information) << "CRfPlayer is initializing" ;
      api->setPluginState(yApi::historization::EPluginState::kCustom, "Initialization");

      // Load configuration values (provided by database)
      m_configuration.initializeWith(api->getConfiguration());

      //create connection
      createConnection(api->getEventHandler());

      m_transceiver = boost::make_shared<CTransceiver>();

      YADOMS_LOG(information) << "CRfPlayer is running" ;
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
               //TODO : send command to device
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
            if (extraQuery)
            {
               if (extraQuery->getData()->query() == "firmwareUpdate")
               {
                  std::string base64firmware = extraQuery->getData()->data().get<std::string>("fileContent");

                  std::string firmwareContent = shared::encryption::CBase64::decode(base64firmware);

                  for (int i = 0; i < 100; ++i)
                  {
                     if(i<50)
                        extraQuery->reportProgress(i*1.0f, "customLabels.firmwareUpdate.sendFile");
                     else
                        extraQuery->reportProgress(i*1.0f, "customLabels.firmwareUpdate.writeFile");
                     boost::this_thread::sleep(boost::posix_time::milliseconds(200));
                  }
                  //YADOMS_LOG(information) << "FirmwareUpdate: content =" << firmwareContent;

                  //m_messageHandler->send(firmwareContent);
               }
            }
            extraQuery->sendSuccess(shared::CDataContainer());
            break;
         }

         case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            // Configuration was updated
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            auto newConfigurationData = api->getEventHandler().getEventData<shared::CDataContainer>();
            YADOMS_LOG(information) << "Update configuration...";
            BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

                                                         // Close connection
            CConfiguration newConfiguration;
            newConfiguration.initializeWith(newConfigurationData);

            // If port has changed, destroy and recreate connection (if any)
            bool needToReconnect = !connectionsAreEqual(m_configuration, newConfiguration) && !!m_port;

            if (needToReconnect)
               destroyConnection();

            // Update configuration
            m_configuration.initializeWith(newConfigurationData);

            if (needToReconnect)
               createConnection(api->getEventHandler());
            else
               api->setPluginState(yApi::historization::EPluginState::kRunning);


            if(!needToReconnect) //if reconnect needed, configuration is applyed after dogle detection
               updateDongleConfiguration(api);

            break;
         }

         case yApi::IYPluginApi::kEventManuallyDeviceCreation:
         {
            // Yadoms asks for device creation
            auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
            YADOMS_LOG(information) << "Manually device creation request received for device :" << request->getData().getDeviceName();
            try
            {
               std::string newDeviceName = m_transceiver->createDeviceManually(api, request->getData());
               request->sendSuccess(newDeviceName);
            }
            catch (std::exception& e)
            {
               request->sendError(e.what());
            }

            break;
         }
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
      YADOMS_LOG(error) << "The RfPlayer plugin fails. shared exception : " << ex.what() ;
      api->setPluginState(yApi::historization::EPluginState::kError, (boost::format("Shared exception : %1%") % ex.what()).str());
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "The RfPlayer plugin fails. exception : " << ex.what() ;
      api->setPluginState(yApi::historization::EPluginState::kError, (boost::format("Exception : %1%") % ex.what()).str());
   }
   catch (...)
   {
      YADOMS_LOG(error) << "The RfPlayer plugin fails. unknown exception" ;
      api->setPluginState(yApi::historization::EPluginState::kError, "Unknown exception");
   }

}


void CRfPlayer::createConnection(shared::event::CEventHandler& eventHandler)
{
   // Create the port instance
   m_port = CFactory::constructPort(m_configuration);

   m_messageHandler = CFactory::constructMessageHandler(m_port, eventHandler, kEvtPortFrameReceived);

   m_port->subscribeForConnectionEvents(eventHandler, kEvtPortConnection);

   m_port->setReceiveBufferHandler(CFactory::constructReceiveBufferHandler(m_messageHandler));

   m_port->start();
}


void CRfPlayer::destroyConnection()
{
   m_port.reset();
}

bool CRfPlayer::connectionsAreEqual(const CConfiguration& conf1, const CConfiguration& conf2)
{
   return (conf1.getSerialPort() == conf2.getSerialPort());
}



void CRfPlayer::processZiBlueConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "RfPlayer port opened";
   api->setPluginState(yApi::historization::EPluginState::kCustom, "initializing");

   try
   {
      // Reset the RfPlayer
      initZiBlue(api);
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Error resetting RfPlayer transceiver : " << e.what();

      // Stop the communication, and try later
      errorProcess(api);
   }
}

void CRfPlayer::errorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   m_port.reset();
   api->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CRfPlayer::processZiBlueUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "RfPlayer connection was lost";
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connectionLost");

   errorProcess(api);
}

void CRfPlayer::processZiBlueBinaryFrameReceived(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<frames::CBinaryFrame> data)
{
   try
   {
      if (data)
      {
         if (m_isDeveloperMode)
         {
            data->printToLog(YADOMS_LOG(debug));
         }

         data->historizeData(api);
      }
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Error processing binary frame : " << e.what();
   }
}

void CRfPlayer::processZiBlueAsciiFrameReceived(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<frames::CAsciiFrame> data)
{
   if (m_isDeveloperMode)
      YADOMS_LOG(debug) << "RfPlayer Ascii <<< " << data->getContent();
}



void CRfPlayer::initZiBlue(boost::shared_ptr<yApi::IYPluginApi> api)
{
   if (!m_messageHandler->send(m_transceiver->buildHelloCmd(),
      [](boost::shared_ptr<const frames::CFrame> frame)
      {
         return frame->isAscii() && boost::icontains(frame->getAscii()->getContent(), "Ziblue Dongle");
      },
      [&](boost::shared_ptr<const frames::CFrame> frame)
      {
         m_dongle = CDongle::create(frame->getAscii()->getContent());

         if (m_dongle)
         {
            YADOMS_LOG(information) << "Dongle :" << m_dongle->getType() << " " << m_dongle->getModel() << " v" << m_dongle->getFirmwareVersion();
         }
         else
         {
            YADOMS_LOG(information) << "Unknown dongle, or not fully supported firmware";
         }
         api->setPluginState(yApi::historization::EPluginState::kRunning);
         updateDongleConfiguration(api);
         m_messageHandler->send(m_transceiver->buildStartListeningData());
      }))
      throw shared::exception::CException("Unable to send HELLO request, timeout waiting acknowledge");
}

void CRfPlayer::updateDongleConfiguration(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      m_messageHandler->send(m_transceiver->buildReceiverConfigurationCommand(m_configuration.getReceiverActiveProtocols()));
      m_messageHandler->send(m_transceiver->buildRepeaterActivationCommand(m_configuration.isRepeaterActive()));
      if (m_configuration.isRepeaterActive())
         m_messageHandler->send(m_transceiver->buildRepeaterConfigurationCommand(m_configuration.getRepeaterActiveProtocols()));
      m_messageHandler->send(m_transceiver->buildLedActivityCommand(m_configuration.getLedActivity()));
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Error configuring RfPlayer transceiver : " << e.what();

      // Stop the communication, and try later
      errorProcess(api);
   }
}
