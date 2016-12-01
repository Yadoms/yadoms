#include "stdafx.h"
#include "Rfxcom.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "RfxcomFactory.h"
#include "ProtocolException.hpp"
#include <shared/communication/PortException.hpp>
#include "ManuallyDeviceCreationException.hpp"

IMPLEMENT_PLUGIN(CRfxcom)


// Event IDs
enum
{
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId, // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kEvtPortDataReceived,
   kProtocolErrorRetryTimer,
   kAnswerTimeout,
};


CRfxcom::CRfxcom()
   : m_logger(std::cout),
     m_configurationUpdated(false),
     m_lastRequest(sizeof(RBUF)),
     m_isDeveloperMode(false)
{
}

CRfxcom::~CRfxcom()
{
}

void CRfxcom::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   std::cout << "CRfxcom is starting..." << std::endl;

   m_configurationUpdated = false;

   m_isDeveloperMode = api->getYadomsInformation()->developperMode();

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   // Create the transceiver
   m_transceiver = CRfxcomFactory::constructTransceiver();

   m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                             shared::event::CEventTimer::kOneShot,
                                                             boost::posix_time::seconds(5));
   m_waitForAnswerTimer->stop();

   // Create the connection
   createConnection(api->getEventHandler());

   // the main loop
   while (1)
   {
      try
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
         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command received from Yadoms
               auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>());
               onCommand(api, command);

               break;
            }
         case yApi::IYPluginApi::kEventManuallyDeviceCreation:
            {
               // Yadoms asks for device creation
               auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
               std::cout << "Manually device creation request received for device :" << request->getData().getDeviceName() << std::endl;
               try
               {
                  request->sendSuccess(m_transceiver->createDeviceManually(api, request->getData()));
               }
               catch (CManuallyDeviceCreationException& e)
               {
                  request->sendError(e.what());
               }

               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());

               break;
            }
         case kEvtPortConnection:
            {
               if (api->getEventHandler().getEventData<bool>())
                  processRfxcomConnectionEvent(api);
               else
                  processRfxcomUnConnectionEvent(api);

               break;
            }
         case kEvtPortDataReceived:
            {
               processRfxcomDataReceived(api, api->getEventHandler().getEventData<const shared::communication::CByteBuffer>());
               break;
            }
         case kAnswerTimeout:
            {
               std::cerr << "No answer received, try to reconnect in a while..." << std::endl;
               errorProcess(api);
               break;
            }
         case kProtocolErrorRetryTimer:
            {
               createConnection(api->getEventHandler());
               break;
            }
         default:
            {
               std::cerr << "Unknown message id" << std::endl;
               break;
            }
         }
      }
      catch (shared::communication::CPortException&)
      {
         std::cerr << "The message is not sent and will be discarded" << std::endl;
         errorProcess(api);
      }
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

bool CRfxcom::connectionsAreEqual(const CRfxcomConfiguration& conf1,
                                  const CRfxcomConfiguration& conf2)
{
   if (conf1.comIsEthernet() != conf2.comIsEthernet())
      return false;

   if (conf1.comIsEthernet())
      return (conf1.getEthernetAddress() == conf2.getEthernetAddress() && conf1.getEthernetPort() == conf2.getEthernetPort());

   return (conf1.getSerialPort() == conf2.getSerialPort());
}

void CRfxcom::send(boost::shared_ptr<yApi::IYPluginApi> api,
                   const shared::communication::CByteBuffer& buffer,
                   bool needAnswer)
{
   if (!m_port)
      return;

   if (m_isDeveloperMode)
      m_logger.logSent(buffer);
   m_port->send(buffer);
   m_lastRequest = buffer;
   if (needAnswer)
      m_waitForAnswerTimer->start();
}

void CRfxcom::send(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> buffers)
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

void CRfxcom::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                        boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   std::cout << "Command received :" << yApi::IDeviceCommand::toString(command) << std::endl;

   if (!m_port)
   {
      std::cout << "Command not sent (RFXCom is not ready) : " << yApi::IDeviceCommand::toString(command) << std::endl;
      return;
   }

   auto message(m_transceiver->buildMessageToDevice(api, command));
   send(api, message);
}

void CRfxcom::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
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

   // Plugin instance is running, close connection only if connection parameters was changed
   CRfxcomConfiguration newConfiguration;
   newConfiguration.initializeWith(newConfigurationData);
   if (connectionsAreEqual(m_configuration, newConfiguration))
   {
      // Port is the same, don't destroy connection, just reconfigure RFXCom.

      // Update configuration
      m_configuration.initializeWith(newConfigurationData);
      m_configurationUpdated = false;

      // Ask status, to compare with new configuration
      std::cout << "Ask the RFXCom status..." << std::endl;
      send(api, m_transceiver->buildGetStatusCmd(), true);
      return;
   }

   // Port has changed, destroy and recreate connection
   destroyConnection();

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);

   // Create new connection
   createConnection(api->getEventHandler());
}

void CRfxcom::processRfxcomConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "RFXCom port opened" << std::endl;
   api->setPluginState(yApi::historization::EPluginState::kCustom, "initializing");

   try
   {
      // Reset the RFXCom
      initRfxcom(api);
   }
   catch (CProtocolException& e)
   {
      std::cerr << "Error resetting RFXCom transceiver : " << e.what() << std::endl;

      // Stop the communication, and try later
      errorProcess(api);
   }
}

void CRfxcom::errorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   destroyConnection();
   api->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CRfxcom::processRfxcomUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "RFXCom connection was lost" << std::endl;
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connectionLost");

   errorProcess(api);
}

void CRfxcom::processRfxcomDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                                        const shared::communication::CByteBuffer& data)
{
   if (m_isDeveloperMode)
      m_logger.logReceived(data);

   auto message = m_transceiver->decodeRfxcomMessage(api, data);

   if (!message)
   {
      std::cout << "Unable to decode received message" << std::endl;
      return;
   }

   // Message was recognized, stop timeout
   m_waitForAnswerTimer->stop();

   // Decoding is OK, process received message
   auto statusMessage = boost::dynamic_pointer_cast<rfxcomMessages::CTransceiverStatus>(message);
   if (!!statusMessage)
   {
      processRfxcomCommandResponseMessage(api, *statusMessage);
      return;
   }

   auto ackMessage = boost::dynamic_pointer_cast<rfxcomMessages::CAck>(message);
   if (!!ackMessage)
   {
      processRfxcomAckMessage(*ackMessage);
      return;
   }

   // Sensor message, historize all data contained in the message
   message->historizeData(api);
}

void CRfxcom::initRfxcom(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Reset the RFXCom.
   // Nbote that this sequence doesn't respect specification (see RFXCom SDK documentation),
   // since the official sequence seems to not work all times.

   // Send reset command to the RfxCom
   std::cout << "Reset the RFXCom..." << std::endl;
   send(api, m_transceiver->buildResetCmd(), false);

   m_configurationUpdated = false;

   // RFXCom needs some time to recover after reset (see specifications)
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));

   std::cout << "Start the RFXtrx receiver..." << std::endl;
   send(api, m_transceiver->buildStartReceiverCmd(), true);
}

void CRfxcom::processRfxcomCommandResponseMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                                  const rfxcomMessages::CTransceiverStatus& status)
{
   // The status message can be received after a get status command or a set mode command
   switch (status.getStatusType())
   {
   case rfxcomMessages::CTransceiverStatus::kStatus: processRfxcomStatusMessage(api, status);
      break;
   case rfxcomMessages::CTransceiverStatus::kWrongCommand: processRfxcomWrongCommandMessage(api, status);
      break;
   case rfxcomMessages::CTransceiverStatus::kReceiverStarted: processRfxcomReceiverStartedMessage(api, status);
      break;
   default:
      std::cout << "Status message (" << status.getStatusType() << ") not yet supported" << std::endl;
      break;
   }
}

void CRfxcom::processRfxcomStatusMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                         const rfxcomMessages::CTransceiverStatus& status)
{
   std::cout << "RFXCom status, type (" << status.rfxcomTypeToString() << "), firmware type (" << status.getFirmwareType() << "), firmware version (" << status.getFirmwareVersion() << ")" << std::endl;
   status.traceEnabledProtocols();

   if (m_configurationUpdated)
   {
      if (status.needConfigurationUpdate(m_configuration))
      {
         std::cout << "Unable to set configuration as expected, maybe incompatible protocols were selected" << std::endl;
         api->setPluginState(yApi::historization::EPluginState::kError, "failToConfigure");
         throw boost::thread_interrupted();
      }

      std::cout << "RFXCom is running" << std::endl;
      api->setPluginState(yApi::historization::EPluginState::kRunning);
   }
   else
   {
      if (status.needConfigurationUpdate(m_configuration))
      {
         std::cout << "Incorrect RFXCom configuration. Updating configuration..." << std::endl;
         send(api, m_transceiver->buildSetModeCmd(status.getRfxcomType(), m_configuration), true);// Don't change the RFXCom frequency
      }
      m_configurationUpdated = true;
      std::cout << "RFXCom is running" << std::endl;
      api->setPluginState(yApi::historization::EPluginState::kRunning);
   }
}

void CRfxcom::processRfxcomReceiverStartedMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                                  const rfxcomMessages::CTransceiverStatus& status)
{
   std::cout << "RFXCom started message, device \"" << status.getValidMessage() << "\" detected" << std::endl;

   std::cout << "Ask the RFXCom status..." << std::endl;
   send(api, m_transceiver->buildGetStatusCmd(), true);
}

void CRfxcom::processRfxcomWrongCommandMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                               const rfxcomMessages::CTransceiverStatus& status)
{
   const RBUF* const lastRequest = reinterpret_cast<const RBUF* const>(m_lastRequest.begin());

   if (lastRequest->ICMND.packettype == pTypeInterfaceControl &&
      lastRequest->ICMND.subtype == sTypeInterfaceCommand &&
      lastRequest->ICMND.cmnd == cmdStartRec)
   {
      // Message "start receiver" is not supported by old firmwares, so ignore wrong answer
      std::cout << "Ask the RFXCom status..." << std::endl;
      send(api, m_transceiver->buildGetStatusCmd(), true);
   }
   else
   {
      std::cout << "RFXCom wrong command response (is your firmware up-to-date ?)" << std::endl;
   }
}

void CRfxcom::processRfxcomAckMessage(const rfxcomMessages::CAck& ack)
{
   if (ack.isOk())
      std::cout << "RFXCom acknowledge" << std::endl;
   else
      std::cout << "RFXCom Received acknowledge is KO" << std::endl;
}

