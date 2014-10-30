#include "stdafx.h"
#include "Rfxcom.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>
#include "RfxcomFactory.h"
#include "ProtocolException.hpp"
#include <shared/communication/PortException.hpp>
#include <shared/plugin/yadomsApi/historization/Switch.h>

IMPLEMENT_PLUGIN(CRfxcom)


// Event IDs
enum
{
   kEvtPortConnection = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from yApi::IYadomsApi::kPluginFirstEventId
   kEvtPortDataReceived,
   kProtocolErrorRetryTimer,
   kAnswerTimeout,
};



//TODO dans package.json, compléter la liste manuallyDeviceCreationConfigurationSchema/type (voir Domoticz "Switch_Type_Desc")
CRfxcom::CRfxcom()
   :m_currentState(kNotInitialized)
{
}

CRfxcom::~CRfxcom()
{
}

void CRfxcom::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      m_currentState = kNotInitialized;

      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

      // the main loop
      YADOMS_LOG(debug) << "CRfxcom is running...";

      // Create the transceiver
      m_transceiver = CRfxcomFactory::constructTransceiver();

      m_waitForAnswerTimer = context->getEventHandler().createTimer(kAnswerTimeout, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(5));
      m_waitForAnswerTimer->stop();

      // Create the connection
      createConnection(context->getEventHandler());

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case yApi::IYadomsApi::kEventDeviceCommand:
            {
               // Command received from Yadoms
               boost::shared_ptr<const yApi::IDeviceCommand> command(context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >());
               YADOMS_LOG(debug) << "Command received :" << command->toString();

               onCommand(context, command->getBody(), context->getDeviceDetails(command->getTargetDevice()));

               break;
            }
         case yApi::IYadomsApi::kEventManuallyDeviceCreationTest:
            {
               // Yadoms asks for test device parameters to check if it works before creating it. So just send command, don't declare anything.
               boost::shared_ptr<yApi::IManuallyDeviceCreationTestData> data = context->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationTestData> >();
               YADOMS_LOG(debug) << "Test of device request received :" << data->toString();

               onCommand(context, data->getCommand()->getBody(), data->getDeviceParameters());

               break;
            }
         case yApi::IYadomsApi::kEventManuallyDeviceCreation:
            {
               // Yadoms asks for device creation
               boost::shared_ptr<yApi::IManuallyDeviceCreationData> data = context->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationData> >();

               //TODO : modifier les lignes suivantes
               /*
               YADOMS_LOG(debug) << "Manually device creation request received :" << data->toString();

               // Declare the device
               context->declareDevice(data->getDeviceName(), shared::CStringExtension::EmptyString, data->getParameters());
               // Declare associated keywords (= values managed by this device)
               context->declareKeyword(data->getDeviceName(), yApi::historization::CSwitch(data->getKeyword()));
               */
               break;
            }
         case yApi::IYadomsApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               shared::CDataContainer newConfigurationData = context->getEventHandler().getEventData<shared::CDataContainer>();
               YADOMS_LOG(debug) << "Configuration was updated...";
               BOOST_ASSERT(!newConfigurationData.empty());  // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Close connection
               CRfxcomConfiguration newConfiguration;
               newConfiguration.initializeWith(newConfigurationData);
               if (!connectionsAreEqual(m_configuration, newConfiguration))
               {
                  // Port has changed, destroy and recreate connection
                  destroyConnection();

                  // Update configuration
                  m_configuration.initializeWith(newConfigurationData);

                  // Create new connection
                  createConnection(context->getEventHandler());
               }
               else
               {
                  // Port is the same, don't destroy connection, just reconfigure RFXCom.

                  // Update configuration
                  m_configuration.initializeWith(newConfigurationData);

                  // Get status, to compare with new configuration
                  YADOMS_LOG(info) << "Get the RFXCom status...";
                  m_currentState = kGettingRfxcomStatus;
                  send(m_transceiver->buildGetStatusCmd(), true);
               }

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
               YADOMS_LOG(error) << "No answer received, try to reconnect in a moment...";
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
   else
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

void CRfxcom::onCommand(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
{
   if (!m_port || m_currentState != kRfxcomIsRunning)
      YADOMS_LOG(warning) << "Command not send (RFXCom is not ready) : " << command;

   send(m_transceiver->buildMessageToDevice(context, command, deviceParameters));
}

void CRfxcom::processRfxcomConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context)
{
   YADOMS_LOG(debug) << "RFXCom is now connected";
   context->recordPluginEvent(yApi::IYadomsApi::kInfo, "RFXCom is now connected");

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

void CRfxcom::errorProcess(boost::shared_ptr<yApi::IYadomsApi> context)
{
   destroyConnection();
   context->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CRfxcom::processRfxcomUnConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context)
{
   YADOMS_LOG(debug) << "RFXCom connection was lost";
   context->recordPluginEvent(yApi::IYadomsApi::kInfo, "RFXCom connection was lost");

   destroyConnection();
}

void CRfxcom::processRfxcomDataReceived(boost::shared_ptr<yApi::IYadomsApi> context, const shared::communication::CByteBuffer& data)
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
   YADOMS_LOG(info) << "Reset the RFXCom...";
   send(m_transceiver->buildResetCmd());
   // No answer

   // RFXCom needs some time to recover after reset (see specifications)
   boost::this_thread::sleep(boost::posix_time::milliseconds(50));

   // Flush receive buffer according to RFXCom specifications
   m_port->flush();

   // Now get the actual RFXCom configuration to check if reconfiguration is needed
   YADOMS_LOG(info) << "Get the RFXCom status...";
   m_currentState = kGettingRfxcomStatus;
   send(m_transceiver->buildGetStatusCmd(), true);
}

void CRfxcom::processRfxcomCommandResponseMessage(boost::shared_ptr<yApi::IYadomsApi> context, const rfxcomMessages::CTransceiverStatus& status)
{
   // The status message can be received after a get status command or a set mode command
   switch(status.getStatusType())
   {
   case rfxcomMessages::CTransceiverStatus::kStatus: processRfxcomStatusMessage(context, status); break;
   case rfxcomMessages::CTransceiverStatus::kWrongCommand: processRfxcomWrongCommandMessage(context, status); break;
   default:
      YADOMS_LOG(warning) << "Status message (" << status.getStatusType() << ") not yet supported";
      break;
   }
}

void CRfxcom::processRfxcomStatusMessage(boost::shared_ptr<yApi::IYadomsApi> context, const rfxcomMessages::CTransceiverStatus& status)
{
   YADOMS_LOG(info) << "RFXCom status, type (" << status.rfxcomTypeToString() << "), firmware version (" << status.getFirmwareVersion() << ")";
   status.traceEnabledProtocols();

   if (!status.needConfigurationUpdate(m_configuration))
   {
      m_currentState = kRfxcomIsRunning;
      return;
   }

   // Incorrect configuration is only possible while initialization procedure
   if (m_currentState != kGettingRfxcomStatus)
   {
      YADOMS_LOG(warning) << "Unable to set configuration as expected, maybe incompatible protocols were selected";
      context->recordPluginEvent(yApi::IYadomsApi::kError, "Unable to set configuration as expected, maybe incompatible protocols was selected");
      m_currentState = kRfxcomIsRunning;
      return;
   }

   YADOMS_LOG(info) << "Incorrect RFXCom configuration. Updating configuration...";

   // Update active protocols list
   m_currentState = kSettingRfxcomMode;
   send(m_transceiver->buildSetModeCmd(status.getRfxcomType(), m_configuration), true);// Don't change the RFXCom frequency
   // Expected reply is also a status message
}

void CRfxcom::processRfxcomWrongCommandMessage(boost::shared_ptr<yApi::IYadomsApi> context, const rfxcomMessages::CTransceiverStatus& status)
{
   YADOMS_LOG(info) << "RFXCom wrong command response";
}

void CRfxcom::processRfxcomAckMessage(const rfxcomMessages::CAck& ack) const
{
   if (ack.isOk())
      YADOMS_LOG(debug) << "RFXCom acknowledge";
   else
      YADOMS_LOG(warning) << "RFXCom Received acknowledge is KO";
}