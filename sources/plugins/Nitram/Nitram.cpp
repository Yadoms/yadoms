#include "stdafx.h"
#include "Nitram.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "NitramFactory.h"
#include <shared/communication/PortException.hpp>
#include <shared/communication/BufferLogger.h>
#include "ProtocolException.hpp"
#include <shared/Log.h>
#include "NitramMessages/Ack.h"
#include "NitramMessages/MessagesDefinition.h"
#include "NitramMessages/NitramHelpers.h"

IMPLEMENT_PLUGIN(CNitram)


// Event IDs
enum
{
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId, // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kEvtPortDataReceived,
   kProtocolErrorRetryTimer,
   kAnswerTimeout,
   kUpsStatusRequestDelay,
   kUpsRuntimeInformation,
   kUpsBatteryVoltageCapacity,
   kUpsSendMessage
};

const std::string CNitram::DeviceName("UPS Nitram Elite");
const std::string CNitram::Model("Nitram Elite");

CNitram::CNitram()
   : m_logger(boost::make_shared<shared::communication::CBufferLogger>("trace")),
     m_protocolErrorCounter(0),
     m_acPowerActive(true),
     m_messageBatteryAndVoltage(boost::make_shared<nitramMessages::CBatteryCapacityAndVoltage>(DeviceName)),
     m_messageSummaryStatus(boost::make_shared<nitramMessages::CSummaryStatus>(DeviceName)),
     m_messageRuntime(boost::make_shared<nitramMessages::CRuntime>(DeviceName)),
     m_messageShutdownDelay(boost::make_shared<nitramMessages::CSetupShutdownDelay>()),
     m_messageStartupDelay(boost::make_shared<nitramMessages::CSetupStartupDelay>()),
     m_upsShutdown(boost::make_shared<yApi::historization::CEvent>("UpsShutdown")),
     m_upsRestore(boost::make_shared<yApi::historization::CEvent>("UpsRestore")),
     m_keywords({ m_upsShutdown , m_upsRestore })
{}

CNitram::~CNitram()
{}

void CNitram::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   setPluginState(api, ENitramPluginState::kConnecting);

   YADOMS_LOG(information) << "CNitram is starting..." ;

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                             shared::event::CEventTimer::kOneShot,
                                                             boost::posix_time::seconds(2));
   m_waitForAnswerTimer->stop();
   m_upsStatusRequestTimer = api->getEventHandler().createTimer(kUpsStatusRequestDelay,
                                                                shared::event::CEventTimer::kPeriodic,
                                                                boost::posix_time::seconds(10));
   m_upsStatusRequestTimer->stop();

   m_upsBatteryVoltageRequest = api->getEventHandler().createTimer(kUpsBatteryVoltageCapacity,
                                                                   shared::event::CEventTimer::kPeriodic,
                                                                   boost::posix_time::seconds(60));
   m_upsBatteryVoltageRequest->stop();

   // Create the connection
   createConnection(api);

   declareDevice(api, Model);

   // the main loop
   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested" ;
            setPluginState(api, ENitramPluginState::kStop);
            return;
         }
      case yApi::IYPluginApi::kEventDeviceCommand:
      {
         // Command received from Yadoms
         auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>());
         YADOMS_LOG(information) << "Command received : " << yApi::IDeviceCommand::toString(command);

         if (boost::iequals(command->getKeyword(), m_upsShutdown->getKeyword()))
            onCommandShutdown(api, command->getBody());
         else if (boost::iequals(command->getKeyword(), m_upsRestore->getKeyword()))
            onCommandRestore(api, command->getBody());
         else
            YADOMS_LOG(information) << "Received command for unknown keyword from Yadoms : " << yApi::IDeviceCommand::toString(command);

         break;
      }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            // Configuration was updated
            setPluginState(api, ENitramPluginState::kupdateConfiguration);
            auto newConfigurationData = api->getEventHandler().getEventData<shared::CDataContainer>();
            YADOMS_LOG(information) << "Update configuration..." ;
            BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

            // Close connection
            CNitramConfiguration newConfiguration;
            newConfiguration.initializeWith(newConfigurationData);

            // If port has changed, destroy and recreate connection (if any)
            auto needToReconnect = !connectionsAreEqual(m_configuration, newConfiguration) && !!m_port;

            if (needToReconnect)
               destroyConnection();

            // Update configuration
            m_configuration.initializeWith(newConfigurationData);

            if (needToReconnect)
               createConnection(api);
            else
               setPluginState(api, ENitramPluginState::kRunning);

            break;
         }
      case kEvtPortConnection:
         {
            auto notif = api->getEventHandler().getEventData<boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>>();

            if (notif && notif->isConnected())
               processConnectionEvent(api);
            else
               processUnConnectionEvent(api, notif);

            break;
         }
      case kEvtPortDataReceived:
         {
            const auto buffer(api->getEventHandler().getEventData<const shared::communication::CByteBuffer>());
            m_logger->logReceived(buffer);
            setPluginState(api, ENitramPluginState::kRunning);
            processDataReceived(api, buffer);
            break;
         }
      case kAnswerTimeout:
         {
            YADOMS_LOG(information) << "No answer received from UPS (timeout)" ;
            protocolErrorProcess(api);
            break;
         }
      case kUpsStatusRequestDelay:
         {
            // Ask for status
            sendGetStatusCmd();
            api->getEventHandler().createTimer(kUpsSendMessage,
                                               shared::event::CEventTimer::kOneShot,
                                               boost::posix_time::seconds(0));
            break;
         }
      case kUpsRuntimeInformation:
      {
         // Ask for runTime
         sendGetRuntimeCmd();
         break;
      }
      case kUpsBatteryVoltageCapacity:
      {
         sendGetBatteryVoltageCmd();
         break;
      }
      case kProtocolErrorRetryTimer:
         {
            createConnection(api);
            break;
         }
      case kUpsSendMessage:
      {
         send();
         break;
      }
      default:
         {
            YADOMS_LOG(warning) << "Unknown message id " << api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

void CNitram::createConnection(boost::shared_ptr<yApi::IYPluginApi> api)
{
   setPluginState(api, ENitramPluginState::kConnecting);

   // Create the port instance
   m_port = CNitramFactory::constructPort(m_configuration,
                                          api->getEventHandler(),
                                          kEvtPortConnection,
                                          kEvtPortDataReceived);
   m_port->start();
}

void CNitram::destroyConnection()
{
   YADOMS_LOG(information) << "destroy connection";

   m_port.reset();
   m_waitForAnswerTimer->stop();
   m_upsStatusRequestTimer->stop();
   m_upsBatteryVoltageRequest->stop();
}

bool CNitram::connectionsAreEqual(const CNitramConfiguration& conf1,
                                  const CNitramConfiguration& conf2)
{
   return (conf1.getSerialPort() == conf2.getSerialPort());
}

void CNitram::send(const boost::shared_ptr<nitramMessages::INitramMessage> message)
{
   if (!m_port)
      return;

   buffers.push(message);
}

void CNitram::send()
{
   if (!buffers.empty())
   {
      auto temp = buffers.front();
      auto buffer = *temp->encode();

      // Création de la trame complète
      shared::communication::CByteBuffer frame(buffer.size() + 2);

      //Mise en place du header
      frame[0] = createHeader(buffer.size(), false, true);

      //Copie du corps de la trame
      for (unsigned char counter = 0; counter < buffer.size(); ++counter)
         frame[counter + 1] = buffer[counter];

      //Mse en place du ckecksum
      frame[buffer.size() + 1] = calculateCheckSum(frame, frame.size() - 1);

      m_logger->logSent(frame);
      m_port->send(frame);
      // Cache the buffer only if not already sending last buffer
      if (m_lastSentBuffer != temp) // TODO : To be review
         m_lastSentBuffer = temp;

      m_waitForAnswerTimer->start();
   }
}

void CNitram::sendAck()
{
   if (!m_port)
      return;

   // L'envoie d'un Ack se réalise sans CRC
   nitramMessages::CAck messageAck;
   const shared::communication::CByteBuffer buffer(*(messageAck.encode()));

   m_logger->logSent(buffer);
   m_port->send(buffer);
}

void CNitram::processConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "UPS port opened";

   m_upsStatusRequestTimer->start();
   m_upsBatteryVoltageRequest->start();

   // Ask for UPS informations
   api->getEventHandler().createTimer(kUpsBatteryVoltageCapacity,
                                      shared::event::CEventTimer::kOneShot,
                                      boost::posix_time::seconds(0));

   api->getEventHandler().createTimer(kUpsStatusRequestDelay,
                                       shared::event::CEventTimer::kOneShot,
                                       boost::posix_time::seconds(0));
}

void CNitram::protocolErrorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   if (m_protocolErrorCounter <= 3)
   {
      ++m_protocolErrorCounter;
      api->getEventHandler().createTimer(kUpsSendMessage,
                                         shared::event::CEventTimer::kOneShot,
                                         boost::posix_time::milliseconds(500));
      return;
   }

   processUnConnectionEvent(api);
}

void CNitram::processUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api,
                                       boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification> notification)
{
   YADOMS_LOG(information) << "UPS connection was lost";
   if (notification)
      api->setPluginState(yApi::historization::EPluginState::kError,
                          notification->getErrorMessageI18n(),
                          notification->getErrorMessageI18nParameters());
   else
      api->setPluginState(yApi::historization::EPluginState::kError,
                          "connectionFailed");

   // On vide le buffer
   while (!buffers.empty())
      buffers.pop();

   destroyConnection();

   // Retry full connection
   api->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CNitram::processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const shared::communication::CByteBuffer& buffer)
{
   if (buffer.size() == 0)
      return;

   try {
      // Si Nack
      if (buffer.size() == 1 && readHeaderError(buffer[0]))
      {
         YADOMS_LOG(error) << "receive a NACK";
         protocolErrorProcess(api);
      }
      else if (buffer.size() == 1 && readHeaderFinish(buffer[0])) // Si Ack
      {
         m_protocolErrorCounter = 0;
         // Pour les messages qui n'acceptent pas de retour on les dépile, et on arrête le timer
         if (buffers.front()->onlyAck())
         {
            m_waitForAnswerTimer->stop();
            buffers.pop();
         }
      }
      else // it's a message
      {
         m_protocolErrorCounter = 0;
         boost::shared_ptr<nitramMessages::INitramMessage> message;

         // Messages d'informations
         // Suivant le premier octet
         switch (buffer[1])
         {
         case nitramMessages::ENitramMessageID::BatteryCapacityAndVoltage:
            m_waitForAnswerTimer->stop();
            m_messageBatteryAndVoltage->decode(api, buffer);
            message = m_messageBatteryAndVoltage;
            buffers.pop(); // On dépile le message émetteur
            sendAck(); // Envoi de la bonne réception
            break;
         case nitramMessages::ENitramMessageID::Runtime:
            m_waitForAnswerTimer->stop();
            m_messageRuntime->decode(api, buffer);
            message = m_messageRuntime;
            buffers.pop(); // On dépile le message émetteur
            sendAck(); // Envoi de la bonne réception
            break;
         case nitramMessages::ENitramMessageID::SummaryStatus:
            m_waitForAnswerTimer->stop();
            m_messageSummaryStatus->decode(api, buffer);
            message = m_messageSummaryStatus;
            buffers.pop(); // On dépile le message émetteur
            sendAck(); // Envoi de la bonne réception

            // Si nous avons perdu l'alimentation, alors on récupère le temps qui reste
            // On envoie tout de suite un message pour récupérer la valeur
            if (!m_messageSummaryStatus->getACPresent())
            {
               api->getEventHandler().createTimer(kUpsRuntimeInformation,
                                                  shared::event::CEventTimer::kOneShot,
                                                  boost::posix_time::seconds(0));
            }
            else
            {
               m_messageRuntime->setRunTimeIndefined();
               m_messageRuntime->historizeData(api);
            }

            break;
         default: // If the first character is not recognize we flush the reception
            break;
         }

         // Historization du message reçu
         message->historizeData(api);

         // If any send new messages
         if (!buffers.empty())
         {
            api->getEventHandler().createTimer(kUpsSendMessage,
                                               shared::event::CEventTimer::kOneShot,
                                               boost::posix_time::milliseconds(500));
         }
      }
   }
   catch (CProtocolException& e)
   {
      YADOMS_LOG(error) << "Protocol error : " << e.what();
      protocolErrorProcess(api);
   }
   catch (shared::communication::CPortException& e)
   {
      YADOMS_LOG(error) << "Error connecting to UPS : " << e.what();
      // Disconnection will be notified, we just have to wait...
   }
}

void CNitram::sendGetRuntimeCmd()
{
   send(m_messageRuntime);
}

void CNitram::sendGetBatteryVoltageCmd()
{
   send(m_messageBatteryAndVoltage);
}

void CNitram::sendGetStatusCmd()
{
   send(m_messageSummaryStatus);
}

void CNitram::onCommandShutdown(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& command)
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywordsToHistorize = { m_upsShutdown };

   if (!m_port)
      YADOMS_LOG(information) << "Command not send (UPS is not ready) : " << command;

   ChangeShutdownDelay(api, m_configuration.outputShutdownDelay());
   ChangeRestoreDelay(api, m_configuration.outputRestoreDelay());

   if (m_configuration.outputRestoreDelayActivated())
      m_keywordsToHistorize.push_back(m_upsRestore);

   api->historizeData(DeviceName, m_keywordsToHistorize);

   api->getEventHandler().createTimer(kUpsSendMessage,
                                      shared::event::CEventTimer::kOneShot,
                                      boost::posix_time::milliseconds(500));
}

void CNitram::onCommandRestore(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string& command)
{
   if (!m_port)
      YADOMS_LOG(information) << "Command not send (UPS is not ready) : " << command;

   ChangeRestoreDelay(api, m_configuration.outputRestoreDelay());

   api->historizeData(DeviceName, m_upsRestore);

   api->getEventHandler().createTimer(kUpsSendMessage,
                                      shared::event::CEventTimer::kOneShot,
                                      boost::posix_time::milliseconds(500));
}

void CNitram::ChangeShutdownDelay(boost::shared_ptr<yApi::IYPluginApi> api,
                                  boost::posix_time::time_duration delay)
{
   if (!m_port)
      YADOMS_LOG(information) << "Command not send (UPS is not ready)";

   m_messageShutdownDelay->setDelay(delay);
   send(m_messageShutdownDelay);
}

void CNitram::ChangeRestoreDelay(boost::shared_ptr<yApi::IYPluginApi> api,
                                 boost::posix_time::time_duration delay)
{
   if (!m_port)
      YADOMS_LOG(information) << "Command not send (UPS is not ready)";

   if (m_configuration.outputRestoreDelayActivated())
      m_messageStartupDelay->setDelay(delay);
   else
      m_messageStartupDelay->setMessageForShutdown();

   send(m_messageStartupDelay);
}

void CNitram::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                            const std::string& model)
{
   // Add all keywords into the main keywords list
   m_keywords.insert(m_keywords.end(), m_messageBatteryAndVoltage->keywords().begin(), m_messageBatteryAndVoltage->keywords().end());
   m_keywords.insert(m_keywords.end(), m_messageSummaryStatus->keywords().begin(), m_messageSummaryStatus->keywords().end());
   m_keywords.insert(m_keywords.end(), m_messageRuntime->keywords().begin(), m_messageRuntime->keywords().end());

   if (!api->deviceExists(DeviceName))
   {
      api->declareDevice(DeviceName, model, model, m_keywords);
   }
}

void CNitram::setPluginState(boost::shared_ptr<yApi::IYPluginApi> api, ENitramPluginState newState)
{
   if (m_runningState != newState)
   {
      switch (newState)
      {
      case ENitramPluginState::kInitializationError:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");
         break;
      case ENitramPluginState::kupdateConfiguration:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "updateconfiguration");
         break;
      case ENitramPluginState::kConnecting:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");
         break;
      case ENitramPluginState::kNoConnection:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection");
         break;
      case ENitramPluginState::kRunning:
         api->setPluginState(yApi::historization::EPluginState::kRunning);
         break;
      case ENitramPluginState::kStop:
         api->setPluginState(yApi::historization::EPluginState::kStopped);
         break;
      default:
         YADOMS_LOG(error) << "this plugin status does not exist : " << newState;
         break;
      }

      m_runningState = newState;
   }
}