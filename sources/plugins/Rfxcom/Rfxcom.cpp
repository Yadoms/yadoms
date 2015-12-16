#include "stdafx.h"
#include "Rfxcom.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>
#include "RfxcomFactory.h"
#include "ProtocolException.hpp"
#include <shared/communication/PortException.hpp>
#include <shared/plugin/yPluginApi/historization/Dimmable.h>
#include "ManuallyDeviceCreationException.hpp"

IMPLEMENT_PLUGIN(CRfxcom)


// Event IDs
enum
{
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kEvtPortDataReceived,
   kProtocolErrorRetryTimer,
   kAnswerTimeout,
};



CRfxcom::CRfxcom()
   :m_currentState(kNotInitialized)
{
}

CRfxcom::~CRfxcom()
{
}

void CRfxcom::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      context->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

      YADOMS_LOG(debug) << "CRfxcom is starting...";

      m_currentState = kNotInitialized;

      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

      // Create the transceiver
      m_transceiver = CRfxcomFactory::constructTransceiver();

      m_waitForAnswerTimer = context->getEventHandler().createTimer(kAnswerTimeout, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(5));
      m_waitForAnswerTimer->stop();

      // Create the connection
      createConnection(context->getEventHandler());

      // the main loop
      while (1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command received from Yadoms
               boost::shared_ptr<const yApi::IDeviceCommand> command(context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >());
               onCommand(context, command);

               break;
            }
         case yApi::IYPluginApi::kEventManuallyDeviceCreationTest:
            {
               // Yadoms asks for test device parameters to check if it works before creating it. So just send command, don't declare anything.
               boost::shared_ptr<const yApi::IManuallyDeviceCreationTestData> data = context->getEventHandler().getEventData<const boost::shared_ptr<yApi::IManuallyDeviceCreationTestData> >();
               onCommand(context, data->getCommand());

               break;
            }
         case yApi::IYPluginApi::kEventManuallyDeviceCreation:
            {
               // Yadoms asks for device creation
               boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> request = context->getEventHandler().getEventData< boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> >();
               YADOMS_LOG(debug) << "Manually device creation request received for device :" << request->getData().getDeviceName();
               try
               {
                  request->sendSuccess(m_transceiver->createDeviceManually(context, request->getData()));
               }
               catch (CManuallyDeviceCreationException& e)
               {
                  request->sendError(e.what());
               }
               
               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               onUpdateConfiguration(context, context->getEventHandler().getEventData<shared::CDataContainer>());

               break;
            }
         case kEvtPortConnection:
            {
               if (context->getEventHandler().getEventData<bool>())
                  processRfxcomConnectionEvent(context);
               else
                  processRfxcomUnConnectionEvent(context);

               break;
            }
         case kEvtPortDataReceived:
            {
               processRfxcomDataReceived(context, context->getEventHandler().getEventData<const shared::communication::CByteBuffer>());
               break;
            }
         case kAnswerTimeout:
            {
               YADOMS_LOG(error) << "No answer received, try to reconnect in a while...";
               errorProcess(context);
               break;
            }
         case kProtocolErrorRetryTimer:
            {
               createConnection(context->getEventHandler());
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
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(information) << "Thread is stopping...";
   }
}

void CRfxcom::createConnection(shared::event::CEventHandler& eventHandler)
{
   // Create the port instance
   m_port = CRfxcomFactory::constructPort(m_configuration, eventHandler, kEvtPortConnection, kEvtPortDataReceived);
   m_port->setReceiveBufferMaxSize(RFXMESSAGE_maxSize);
   m_port->start();
}

void CRfxcom::destroyConnection()
{
   m_port.reset();

   m_waitForAnswerTimer->stop();
}

bool CRfxcom::connectionsAreEqual(const CRfxcomConfiguration& conf1, const CRfxcomConfiguration& conf2) const
{
   if (conf1.comIsEthernet() != conf2.comIsEthernet())
      return false;

   if (conf1.comIsEthernet())
      return (conf1.getEthernetAddress() == conf2.getEthernetAddress() && conf1.getEthernetPort() == conf2.getEthernetPort());

   return (conf1.getSerialPort() == conf2.getSerialPort());
}

void CRfxcom::send(const shared::communication::CByteBuffer& buffer, bool needAnswer)
{
   if (!m_port)
      return;

   m_logger.logSent(buffer);
   m_port->send(buffer);
   if (needAnswer)
      m_waitForAnswerTimer->start();
}

void CRfxcom::send(boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > buffers)
{
   if (!m_port)
      return;

   // Send all messages
   while (!buffers->empty())
   {
      send(buffers->front(), false);
      buffers->pop();
   }
}

void CRfxcom::onCommand(boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   YADOMS_LOG(debug) << "Command received :" << command->toString();

   if (!m_port || m_currentState != kRfxcomIsRunning)
   {
      YADOMS_LOG(warning) << "Command not sent (RFXCom is not ready) : " << command->toString();
      return;
   }

   send(m_transceiver->buildMessageToDevice(context, command));
}

void CRfxcom::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> context, const shared::CDataContainer& newConfigurationData)
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

   // Plugin instance is running, close connection only if connection parameters was changed
   CRfxcomConfiguration newConfiguration;
   newConfiguration.initializeWith(newConfigurationData);
   if (connectionsAreEqual(m_configuration, newConfiguration))
   {
      // Port is the same, don't destroy connection, just reconfigure RFXCom.

      // Update configuration
      m_configuration.initializeWith(newConfigurationData);

      // Get status, to compare with new configuration
      YADOMS_LOG(information) << "Get the RFXCom status...";
      m_currentState = kGettingRfxcomStatus;
      send(m_transceiver->buildGetStatusCmd(), true);
      return;
   }

   // Port has changed, destroy and recreate connection
   destroyConnection();

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);

   // Create new connection
   createConnection(context->getEventHandler());
}

void CRfxcom::processRfxcomConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> context)
{
   YADOMS_LOG(debug) << "RFXCom is now connected";
   context->setPluginState(yApi::historization::EPluginState::kCustom, "connected");

   try
   {
      // Reset the RFXCom
      m_currentState = kResettingRfxcom;
      initRfxcom();
   }
   catch (CProtocolException& e)
   {
      YADOMS_LOG(error) << "Error resetting RFXCom transceiver : " << e.what();

      // Stop the communication, and try later
      errorProcess(context);
   }
   catch (shared::communication::CPortException& e)
   {
      YADOMS_LOG(error) << "Error connecting to RFXCom transceiver : " << e.what();
      // Disconnection will be notified, we just have to wait...
   }
}

void CRfxcom::errorProcess(boost::shared_ptr<yApi::IYPluginApi> context)
{
   destroyConnection();
   context->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CRfxcom::processRfxcomUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> context)
{
   YADOMS_LOG(debug) << "RFXCom connection was lost";
   context->setPluginState(yApi::historization::EPluginState::kCustom, "connectionLost");

   destroyConnection();
}

void CRfxcom::processRfxcomDataReceived(boost::shared_ptr<yApi::IYPluginApi> context, const shared::communication::CByteBuffer& data)
{
   m_logger.logReceived(data);

   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> message = m_transceiver->decodeRfxcomMessage(context, data);

   if (!message)
   {
      YADOMS_LOG(warning) << "Unable to decode received message";
      return;
   }

   // Message was recognized, stop timeout
   m_waitForAnswerTimer->stop();

   // Decoding is OK, process received message
   boost::shared_ptr<rfxcomMessages::CTransceiverStatus> statusMessage = boost::dynamic_pointer_cast<rfxcomMessages::CTransceiverStatus>(message);
   if (!!statusMessage)
   {
      processRfxcomCommandResponseMessage(context, *statusMessage);
      return;
   }

   boost::shared_ptr<rfxcomMessages::CAck> ackMessage = boost::dynamic_pointer_cast<rfxcomMessages::CAck>(message);
   if (!!ackMessage)
   {
      processRfxcomAckMessage(*ackMessage);
      return;
   }

   // Sensor message, historize all data contained in the message
   message->historizeData(context);
}

void CRfxcom::initRfxcom()
{
   // Reset the RFXCom.
   // See the RFXCom SDK specification for more information about this sequence

   // Send reset command to the RfxCom
   YADOMS_LOG(information) << "Reset the RFXCom...";
   send(m_transceiver->buildResetCmd());
   // No answer

   // RFXCom needs some time to recover after reset (see specifications)
   boost::this_thread::sleep(boost::posix_time::milliseconds(100));

   // Flush receive buffer according to RFXCom specifications
   m_port->flush();

   // Now get the actual RFXCom configuration to check if reconfiguration is needed
   YADOMS_LOG(information) << "Get the RFXCom status...";
   m_currentState = kGettingRfxcomStatus;
   send(m_transceiver->buildGetStatusCmd(), true);
}

void CRfxcom::processRfxcomCommandResponseMessage(boost::shared_ptr<yApi::IYPluginApi> context, const rfxcomMessages::CTransceiverStatus& status)
{
   // The status message can be received after a get status command or a set mode command
   switch(status.getStatusType())
   {
   case rfxcomMessages::CTransceiverStatus::kStatus: processRfxcomStatusMessage(context, status); break;
   case rfxcomMessages::CTransceiverStatus::kWrongCommand: processRfxcomWrongCommandMessage(context, status); break;
   case rfxcomMessages::CTransceiverStatus::kReceiverStarted: processRfxcomReceiverStartedMessage(context, status); break;
   default:
      YADOMS_LOG(warning) << "Status message (" << status.getStatusType() << ") not yet supported";
      break;
   }
}

void CRfxcom::processRfxcomStatusMessage(boost::shared_ptr<yApi::IYPluginApi> context, const rfxcomMessages::CTransceiverStatus& status)
{
   YADOMS_LOG(information) << "RFXCom status, type (" << status.rfxcomTypeToString() << "), firmware version (" << status.getFirmwareVersion() << ")";
   status.traceEnabledProtocols();

   switch (m_currentState)
   {
   case kGettingRfxcomStatus:
      if (status.needConfigurationUpdate(m_configuration))
      {
         YADOMS_LOG(information) << "Incorrect RFXCom configuration. Updating configuration...";
         m_currentState = kSettingRfxcomMode;
         send(m_transceiver->buildSetModeCmd(status.getRfxcomType(), m_configuration), true);// Don't change the RFXCom frequency
         // Expected reply is also a status message
      }
      else
      {
         YADOMS_LOG(information) << "Start the RFXtrx receiver...";
         m_currentState = kStartReceiver;
         send(m_transceiver->buildStartReceiverCmd(), true);
         // Expected reply is a response message
      }
      break;

   case kSettingRfxcomMode:
      if (status.needConfigurationUpdate(m_configuration))
      {
         YADOMS_LOG(warning) << "Unable to set configuration as expected, maybe incompatible protocols were selected";
         context->setPluginState(yApi::historization::EPluginState::kRunning);
         m_currentState = kRfxcomIsRunning;
      }
      else
      {
         YADOMS_LOG(information) << "Start the RFXtrx receiver...";
         m_currentState = kStartReceiver;
         send(m_transceiver->buildStartReceiverCmd(), true);
         // Expected reply is a response message for recent firmware, or wrong command for old firmware (safe to ignore)
      }
      break;

   default:
      YADOMS_LOG(warning) << "Received non-expected message (RFXCom status)";
      context->setPluginState(yApi::historization::EPluginState::kRunning);
      m_currentState = kRfxcomIsRunning;
      break;
   }
}

void CRfxcom::processRfxcomReceiverStartedMessage(boost::shared_ptr<yApi::IYPluginApi> context, const rfxcomMessages::CTransceiverStatus& status)
{
   YADOMS_LOG(information) << "RFXCom started message, device \"" << status.getValidMessage() << "\" detected";

   switch (m_currentState)
   {
   case kStartReceiver:
      YADOMS_LOG(information) << "Receiver started";
      context->setPluginState(yApi::historization::EPluginState::kRunning);
      m_currentState = kRfxcomIsRunning;
      break;

   default:
      YADOMS_LOG(warning) << "Received non-expected message (Receiver started)";
      context->setPluginState(yApi::historization::EPluginState::kRunning);
      m_currentState = kRfxcomIsRunning;
      break;
   }
}

void CRfxcom::processRfxcomWrongCommandMessage(boost::shared_ptr<yApi::IYPluginApi> context, const rfxcomMessages::CTransceiverStatus& status)
{
   YADOMS_LOG(information) << "RFXCom wrong command response";
   if (m_currentState == kStartReceiver)
   {
      // Start receiver message is unknown by old firmwares. In this case, just ignore the message
      YADOMS_LOG(information) << "Receiver started (old firmware)";
      context->setPluginState(yApi::historization::EPluginState::kRunning);
      m_currentState = kRfxcomIsRunning;
   }
}

void CRfxcom::processRfxcomAckMessage(const rfxcomMessages::CAck& ack) const
{
   if (ack.isOk())
      YADOMS_LOG(debug) << "RFXCom acknowledge";
   else
      YADOMS_LOG(warning) << "RFXCom Received acknowledge is KO";
}
