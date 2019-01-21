#include "stdafx.h"
#include "Rfxcom.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "RfxcomFactory.h"
#include "ProtocolException.hpp"
#include <shared/communication/PortException.hpp>
#include "ManuallyDeviceCreationException.hpp"
#include <shared/Log.h>

IMPLEMENT_PLUGIN(CRfxcom)


// Event IDs
enum
{
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId,
   kEvtPortDataReceived,
   kProtocolErrorRetryTimer,
   kProgressPairingTimer,
   kAnswerTimeout,
};


CRfxcom::CRfxcom()
   : m_logger("debug"),
     m_configurationUpdated(false),
     m_lastRequest(sizeof(RBUF))
{
}

CRfxcom::~CRfxcom()
{
}

void CRfxcom::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   YADOMS_LOG(information) << "RFXCom is starting...";

   m_configurationUpdated = false;

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   // Create the transceiver
   m_pairingHelper = m_factory.constructPairingHelper(api,
                                                      m_configuration.getPairingMode());
   m_transceiver = m_factory.constructTransceiver(m_pairingHelper);

   m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                             shared::event::CEventTimer::kOneShot,
                                                             boost::posix_time::seconds(5));
   m_waitForAnswerTimer->stop();

   // Create the connection
   createConnection(api->getEventHandler());

   // the main loop
   while (true)
   {
      try
      {
         // Wait for an event
         switch (api->getEventHandler().waitForEvents())
         {
         case yApi::IYPluginApi::kEventStopRequested:
            {
               YADOMS_LOG(information) << "Stop requested";
               api->setPluginState(yApi::historization::EPluginState::kStopped);
               return;
            }
         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command received from Yadoms
               const auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>());
               onCommand(api, command);

               break;
            }
         case yApi::IYPluginApi::kEventManuallyDeviceCreation:
            {
               // Yadoms asks for device creation
               auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
               YADOMS_LOG(information) << "Manually device creation request received for device : " << request->getData().getDeviceName();
               try
               {
                  m_transceiver->createDeviceManually(api,
                                                      request->getData());
                  request->sendSuccess();
               }
               catch (CManuallyDeviceCreationException& e)
               {
                  request->sendError(e.what());
               }

               break;
            }
         case yApi::IYPluginApi::kSetDeviceConfiguration:
            {
               // Yadoms notify for device configuration changed
               const auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration>>();
               YADOMS_LOG(information) << "Change device configuration for device : " << deviceConfiguration->name();
               m_transceiver->changeDeviceConfiguration(api,
                                                        deviceConfiguration);

               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());

               break;
            }

         case yApi::IYPluginApi::kEventExtraQuery:
            {
               // Command was received from Yadoms
               auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery>>();
               if (extraQuery)
               {
                  if (extraQuery->getData()->query() == "firmwareUpdate")
                     processFirmwareUpdate(api, extraQuery);
                  else if (extraQuery->getData()->query() == "pairing")
                     startManualPairing(api, extraQuery);
                  else
                  {
                     YADOMS_LOG(error) << "Unsupported query : " << extraQuery->getData()->query();
                     extraQuery->sendError("customLabels.extraquery.ErrorInternal");
                  }
               }
               else
               {
                  YADOMS_LOG(error) << "Invalid query";
                  extraQuery->sendError("customLabels.extraquery.ErrorInternal");
               }
               break;
            }
         case kEvtPortConnection:
            {
               const auto notif = api->getEventHandler().getEventData<boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>>();

               if (notif && notif->isConnected())
                  processRfxcomConnectionEvent(api);
               else
                  processRfxcomUnConnectionEvent(api, notif);

               break;
            }
         case kEvtPortDataReceived:
            {
               processRfxcomDataReceived(api,
                                         api->getEventHandler().getEventData<const shared::communication::CByteBuffer>());
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
         case kProgressPairingTimer:
            {
               if (m_pairingHelper->onProgressPairing())
               {
                  // Finished
                  m_progressPairingTimer.reset();
               }
               else
               {
                  // Next loop
                  if (m_progressPairingTimer)
                     m_progressPairingTimer->start();
               }
               break;
            }
         default:
            {
               YADOMS_LOG(warning) << "Unknown message id " << api->getEventHandler().getEventId();
               break;
            }
         }
      }
      catch (shared::communication::CPortException&)
      {
         YADOMS_LOG(error) << "The message is not sent and will be discarded";
         errorProcess(api);
      }
   }
}

void CRfxcom::createConnection(shared::event::CEventHandler& eventHandler)
{
   // Create the port instance
   m_port = m_factory.constructPort(m_configuration, eventHandler, kEvtPortConnection, kEvtPortDataReceived);
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
   YADOMS_LOG(information) << "Send command : " << yApi::IDeviceCommand::toString(command);

   if (!m_port)
   {
      YADOMS_LOG(warning) << "Command not sent (RFXCom is not ready) : " << yApi::IDeviceCommand::toString(command);
      return;
   }

   try
   {
      const auto message(m_transceiver->buildMessageToDevice(api, command));
      send(api, message);
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Fail to send command " << yApi::IDeviceCommand::toString(command) << ", error : " << e.what();
   }
}

void CRfxcom::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration...";
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // If plugin instance is not running, just update configuration
   if (!m_port)
   {
      // Update configuration
      m_configuration.initializeWith(newConfigurationData);
      m_pairingHelper->setMode(m_configuration.getPairingMode());
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
      m_pairingHelper->setMode(m_configuration.getPairingMode());
      m_configurationUpdated = false;

      // Ask status, to compare with new configuration
      YADOMS_LOG(information) << "Ask the RFXCom status...";
      send(api, m_transceiver->buildGetStatusCmd(), true);
      return;
   }

   // Port has changed, destroy and recreate connection
   destroyConnection();

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
   m_pairingHelper->setMode(m_configuration.getPairingMode());

   // Create new connection
   createConnection(api->getEventHandler());
}

void CRfxcom::processRfxcomConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "RFXCom port opened";
   api->setPluginState(yApi::historization::EPluginState::kCustom, "initializing");

   try
   {
      // Reset the RFXCom
      initRfxcom(api);
   }
   catch (CProtocolException& e)
   {
      YADOMS_LOG(error) << "Error resetting RFXCom transceiver : " << e.what();

      // Stop the communication, and try later
      errorProcess(api);
   }
}

void CRfxcom::errorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   destroyConnection();
   api->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CRfxcom::processRfxcomUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api,
                                             boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification> notification)
{
   YADOMS_LOG(information) << "RFXCom connection was lost";
   if (notification)
      api->setPluginState(yApi::historization::EPluginState::kError,
                          notification->getErrorMessageI18n(),
                          notification->getErrorMessageI18nParameters());
   else
      api->setPluginState(yApi::historization::EPluginState::kCustom, "connectionLost");

   errorProcess(api);
}

void CRfxcom::processRfxcomDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                                        const shared::communication::CByteBuffer& data)
{
   m_logger.logReceived(data);

   const auto message = m_transceiver->decodeRfxcomMessage(api, data);

   if (!message)
      return;

   // Message was recognized, stop timeout
   m_waitForAnswerTimer->stop();

   // Decoding is OK, process received message
   const auto statusMessage = boost::dynamic_pointer_cast<rfxcomMessages::CTransceiverStatus>(message);
   if (!!statusMessage)
   {
      processRfxcomCommandResponseMessage(api, *statusMessage);
      return;
   }

   const auto ackMessage = boost::dynamic_pointer_cast<rfxcomMessages::CAck>(message);
   if (!!ackMessage)
   {
      processRfxcomAckMessage(*ackMessage);
      return;
   }

   // Sensor message, historize all data contained in the message
   if (api->deviceExists(message->getDeviceName()))
   {
      createPossiblyMissingKeywords(api,
                                    message);
      message->historizeData(api);
   }
   else
   {
      YADOMS_LOG(information) << "Device not declared, message ignored";
   }
}

void CRfxcom::createPossiblyMissingKeywords(boost::shared_ptr<yApi::IYPluginApi> api,
                                            boost::shared_ptr<rfxcomMessages::IRfxcomMessage> message)
{
   const auto knownKeywords = api->getAllKeywords(message->getDeviceName());
   for (const auto& keyword : message->keywords())
   {
      if (std::find(knownKeywords.begin(), knownKeywords.end(), keyword->getKeyword()) == knownKeywords.end())
         api->declareKeyword(message->getDeviceName(), keyword);
   }
}

void CRfxcom::processFirmwareUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                    boost::shared_ptr<yApi::IExtraQuery> extraQuery)
{
   boost::shared_ptr<IRfxcomFirmwareUpdater> updater;

   api->setPluginState(yApi::historization::EPluginState::kCustom, "updateFirmware");

   // First step : initialization. No connection restart if fail.
   try
   {
      if (m_configuration.comIsEthernet())
         throw std::runtime_error("customLabels.firmwareUpdate.ErrorOnlyAvailableForSerial");

      if (m_port)
         destroyConnection();

      updater = m_factory.constructFirmwareUpdater(api,
                                                   extraQuery,
                                                   m_configuration.getSerialPort());
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "RFXCom firmware update failed, unable to construct FirmwareUpdater object : " << e.what();
      extraQuery->sendError(e.what());
      return;
   }

   // Second step : do the update. Connection restart when success or fail.
   try
   {
      updater->update();

      YADOMS_LOG(information) << "RFXCom firmware successufuly updated";
      extraQuery->sendSuccess(shared::CDataContainer::EmptyContainer);
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "RFXCom firmware update failed : " << e.what();
      extraQuery->sendError(e.what());
   }

   // Restart connection
   createConnection(api->getEventHandler());
}

void CRfxcom::startManualPairing(boost::shared_ptr<yApi::IYPluginApi> api,
                                 boost::shared_ptr<yApi::IExtraQuery> extraQuery)
{
   if (m_pairingHelper->startPairing(extraQuery))
      m_progressPairingTimer = api->getEventHandler().createTimer(kProgressPairingTimer,
                                                                  shared::event::CEventTimer::kOneShot,
                                                                  boost::posix_time::seconds(m_pairingHelper->getPairingPeriodTimeSeconds()));
}

void CRfxcom::initRfxcom(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Reset the RFXCom.
   // Nbote that this sequence doesn't respect specification (see RFXCom SDK documentation),
   // since the official sequence seems to not work all times.

   // Send reset command to the RfxCom
   YADOMS_LOG(information) << "Reset the RFXCom...";
   send(api, m_transceiver->buildResetCmd(), false);

   m_configurationUpdated = false;

   // RFXCom needs some time to recover after reset (see specifications)
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));

   YADOMS_LOG(information) << "Start the RFXtrx receiver...";
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
   case rfxcomMessages::CTransceiverStatus::kUnknownRfyRemote: processRfxcomUnknownRfyRemoteMessage(api, status);
      break;
   case rfxcomMessages::CTransceiverStatus::kWrongCommand: processRfxcomWrongCommandMessage(api, status);
      break;
   case rfxcomMessages::CTransceiverStatus::kReceiverStarted: processRfxcomReceiverStartedMessage(api, status);
      break;
   default:
      YADOMS_LOG(information) << "Status message (" << status.getStatusType() << ") not yet supported";
      break;
   }
}

void CRfxcom::processRfxcomStatusMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                         const rfxcomMessages::CTransceiverStatus& status)
{
   YADOMS_LOG(information) << "RFXCom status, type (" << status.rfxcomTypeToString() << "), firmware type (" << status.getFirmwareType() <<
      "), firmware version (" << status.getFirmwareVersion() << "), hardware version (" << status.getHardwareVersion() << ")";
   status.traceEnabledProtocols();

   if (m_configurationUpdated)
   {
      if (status.needConfigurationUpdate(m_configuration))
      {
         YADOMS_LOG(error) << "Unable to set configuration as expected, maybe incompatible protocols were selected";
         api->setPluginState(yApi::historization::EPluginState::kError, "failToConfigure");
         throw boost::thread_interrupted();
      }

      YADOMS_LOG(information) << "RFXCom is running";
      api->setPluginState(yApi::historization::EPluginState::kRunning);
   }
   else
   {
      if (status.needConfigurationUpdate(m_configuration))
      {
         YADOMS_LOG(information) << "Incorrect RFXCom configuration. Updating configuration...";
         send(api, m_transceiver->buildSetModeCmd(status.getRfxcomType(), m_configuration), true);// Don't change the RFXCom frequency
      }
      m_configurationUpdated = true;
      YADOMS_LOG(information) << "RFXCom is running";
      api->setPluginState(yApi::historization::EPluginState::kRunning);
   }
}

void CRfxcom::processRfxcomReceiverStartedMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                                  const rfxcomMessages::CTransceiverStatus& status)
{
   YADOMS_LOG(information) << "RFXCom started message, device \"" << status.getValidMessage() << "\" detected";

   YADOMS_LOG(information) << "Ask the RFXCom status...";
   send(api, m_transceiver->buildGetStatusCmd(), true);
}

void CRfxcom::processRfxcomWrongCommandMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                               const rfxcomMessages::CTransceiverStatus& status)
{
   const auto lastRequest = reinterpret_cast<const RBUF* const>(m_lastRequest.begin());

   if (lastRequest->ICMND.packettype == pTypeInterfaceControl &&
      lastRequest->ICMND.subtype == sTypeInterfaceCommand &&
      lastRequest->ICMND.cmnd == cmdStartRec)
   {
      // Message "start receiver" is not supported by old firmwares, so ignore wrong answer
      YADOMS_LOG(information) << "Ask the RFXCom status...";
      send(api, m_transceiver->buildGetStatusCmd(), true);
   }
   else
   {
      YADOMS_LOG(information) << "RFXCom wrong command response (is your firmware up-to-date ?)";
   }
}

void CRfxcom::processRfxcomUnknownRfyRemoteMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                                   const rfxcomMessages::CTransceiverStatus& status)
{
   YADOMS_LOG(information) << "RFXCom doesn't know this RFY remote, try to send a program message...";

   const auto lastRequest = reinterpret_cast<const RBUF* const>(m_lastRequest.begin());

   if (lastRequest->RAW.packettype != pTypeRFY)
   {
      YADOMS_LOG(error) << "Last command was not a RFY command, can not send RFY program message";
      return;
   }

   YADOMS_LOG(information) << "RFXCom doesn't know this RFY remote, send a program message...";
   send(api, m_transceiver->buildRfyProgramMessage(m_lastRequest));
}

void CRfxcom::processRfxcomAckMessage(const rfxcomMessages::CAck& ack)
{
   if (ack.isOk())
      YADOMS_LOG(information) << "RFXCom acknowledge";
   else
      YADOMS_LOG(information) << "RFXCom Received acknowledge is KO";
}
