#include "stdafx.h"
#include "Rfxcom.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>
#include "RfxcomFactory.h"
#include "ProtocolException.hpp"
#include "PortException.hpp"

IMPLEMENT_PLUGIN(CRfxcom)


//TODO dans package.json, compléter la liste manuallyDeviceCreationConfigurationSchema/type (voir Domoticz "Switch_Type_Desc")
CRfxcom::CRfxcom()
   :m_currentState(kNotInitialized)
{
}

CRfxcom::~CRfxcom()
{
}

// Event IDs
enum
{
   kEvtPortConnection = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from yApi::IYadomsApi::kPluginFirstEventId
   kEvtPortDataReceived,
   kprotocolErrorRetryTimer
};

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
               boost::shared_ptr<yApi::IDeviceCommand> command(context->getEventHandler().getEventData<boost::shared_ptr<yApi::IDeviceCommand> >());
               YADOMS_LOG(debug) << "Command received :" << command->toString();

               onCommand(command->getBody(), context->getDeviceDetails(command->getTargetDevice()));

               break;
            }
         case yApi::IYadomsApi::kEventManuallyDeviceCreationTest:
            {
               // Yadoms asks for test device parameters to check if it work before creating it. So just send command, don't declare anything.
               boost::shared_ptr<yApi::IManuallyDeviceCreationTestData> data = context->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationTestData> >();
               YADOMS_LOG(debug) << "Test of device request received :" << data->toString();

               onCommand(data->getCommand()->getBody(), data->getDeviceParameters());

               break;
            }
         case yApi::IYadomsApi::kEventManuallyDeviceCreation:
            {
               // Yadoms asks for device creation
               boost::shared_ptr<yApi::IManuallyDeviceCreationData> data = context->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationData> >();
               YADOMS_LOG(debug) << "Manually device creation request received :" << data->toString();

               // Declare the device
               context->declareDevice(data->getDevice(), shared::CStringExtension::EmptyString, data->getParameters());
               // Declare associated keywords (= values managed by this device)
               context->declareKeyword(data->getDevice(), data->getKeyword(), yApi::CStandardCapacities::Switch/* TODO en attendant de récupérer de data->getCapacity()*/);

               break;
            }
         case yApi::IYadomsApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               shared::CDataContainer newConfiguration = context->getEventHandler().getEventData<shared::CDataContainer>();
               YADOMS_LOG(debug) << "Configuration was updated...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Close connection
               destroyConnection();

               // Update configuration
               m_configuration.initializeWith(newConfiguration);

               // Create new connection
               createConnection(context->getEventHandler());

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
               processRfxcomDataReceived(context, context->getEventHandler().getEventData<CByteBuffer>());
               break;
            }
         case kprotocolErrorRetryTimer:
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
   m_portLogger = CRfxcomFactory::constructPortLogger();
   m_port = CRfxcomFactory::constructPort(m_configuration, eventHandler, kEvtPortConnection, kEvtPortDataReceived, m_portLogger);
   m_port->setReceiveBufferSize(RFXMESSAGE_maxSize);
   m_port->start();
}

void CRfxcom::destroyConnection()
{
   m_port.reset();
   m_portLogger.reset();
}

void CRfxcom::onCommand(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
{
   if (!m_port || m_currentState != kRfxcomIsRunning)
      YADOMS_LOG(warning) << "Command not send (RFXCom is not ready) : " << command;

   m_port->send(m_transceiver->buildMessageToDevice(command, deviceParameters));
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
      destroyConnection();
      context->getEventHandler().createTimer(kprotocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
   }
   catch (CPortException& e)
   {
      YADOMS_LOG(error) << "Error connecting to RFXCom transceiver : " << e.what();
      // Disconnection will be notified, we just have to wait...
   }
}

void CRfxcom::processRfxcomUnConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context)
{
   YADOMS_LOG(debug) << "RFXCom connection was lost";
   context->recordPluginEvent(yApi::IYadomsApi::kInfo, "RFXCom connection was lost");

   destroyConnection();
}

void CRfxcom::processRfxcomDataReceived(boost::shared_ptr<yApi::IYadomsApi> context, const CByteBuffer& data)
{
   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> message = m_transceiver->decodeRfxcomMessage(data);

   if (!message)
   {
      YADOMS_LOG(warning) << "Unable to decode received message";
      return;
   }

   // Decoding is OK, process received message
   boost::shared_ptr<rfxcomMessages::CTransceiverStatus> statusMessage = boost::dynamic_pointer_cast<rfxcomMessages::CTransceiverStatus>(message);
   if (!!statusMessage)
   {
      processRfxcomStatusMessage(*statusMessage);
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

   // Raz transceiver
   m_transceiver->reset();

   // Send reset command to the RfxCom
   YADOMS_LOG(info) << "Reset the RFXCom...";
   m_port->send(m_transceiver->buildResetCmd());
   // No answer

   // RFXCom needs some time to recover after reset (see specifications)
   boost::this_thread::sleep(boost::posix_time::milliseconds(50));

   // Flush receive buffer according to RFXCom specifications
   m_port->flush();

   // Now get the actual RFXCom configuration to check if reconfiguration is needed
   YADOMS_LOG(info) << "Get the RFXCom status...";
   m_currentState = kGettingRfxcomStatus;
   m_port->send(m_transceiver->buildGetStatusCmd());
}

void CRfxcom::processRfxcomStatusMessage(const rfxcomMessages::CTransceiverStatus& status) const
{
   // The status message can be received after a get status command or a set mode command
   YADOMS_LOG(info) << "RFXCom status, type (" << status.rfxcomTypeToString() << "), firmware version (" << status.getFirmwareVersion() << ")";
   status.traceEnabledProtocols();

   if (status.needConfigurationUpdate(m_configuration))
   {
      // Incorrect configuration is only possible while initialization procedure
      if (m_currentState != kGettingRfxcomStatus)
         throw CProtocolException("Error configuring RfxCom, configuration was not taken into account");

      // Update active protocols list
      m_currentState = kSettingRfxcomMode;
      m_port->send(m_transceiver->buildSetModeCmd(status.getRfxcomType(), m_configuration));// Don't change the RFXCom frequency
      // Expected reply is also a status message
   }

   m_currentState = kRfxcomIsRunning;
}

void CRfxcom::processRfxcomAckMessage(const rfxcomMessages::CAck& ack) const
{
   if (ack.isOk())
      YADOMS_LOG(debug) << "RFXCom acknowledge";
   else
      YADOMS_LOG(warning) << "RFXCom Received acknowledge is KO";
}