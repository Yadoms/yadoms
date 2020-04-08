#include "stdafx.h"
#include "SomfySituoFactory.h"
#include "SomfySituo.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/communication/PortException.hpp>
#include <shared/communication/AsciiBufferLogger.h>
#include "ProtocolException.hpp"
#include <shared/Log.h>

DECLARE_ENUM_IMPLEMENTATION(EChannel,
   ((Channel1))
   ((Channel2))
   ((Channel3))
   ((Channel4))
   ((Channel5))
);

IMPLEMENT_PLUGIN(CSomfySituo)
#define STX 0x02
#define ETX 0x03

inline int ctoi(const char c)
{
   return c - 48;
}

inline char itoc(const int i)
{
   return static_cast<char>(i) + 48;
}

#define DURATION_PUSH_BTN 250 // in ms 

// Event IDs
enum
{
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId,
   // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kEvtPortDataReceived,
   kProtocolErrorRetryTimer,
   kAnswerTimeout
};


const std::string CSomfySituo::DeviceName("SituoAdapter");
const std::map<std::string, int> CSomfySituo::m_somfyModels = {{"Situo1", 1}, {"Situo5", 5}};

CSomfySituo::CSomfySituo()
   : m_logger(boost::make_shared<shared::communication::CAsciiBufferLogger>("trace")),
     m_protocolErrorCounter(0),
     m_lastSentBuffer(1),
     m_activeChannel(0),
     m_channelSleep(true),
     m_curtain(boost::make_shared<yApi::historization::CCurtain>("Command")),
     m_keywords({m_curtain})
{
}

CSomfySituo::~CSomfySituo()
{
}

void CSomfySituo::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   YADOMS_LOG(information) << "CSomfySituo is starting...";

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   // Create the buffer handler
   m_ReceiveBufferHandler = CSomfySituoFactory::GetBufferHandler(api->getEventHandler(),
                                                                 kEvtPortDataReceived,
                                                                 false);

   m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                             shared::event::CEventTimer::kOneShot,
                                                             boost::posix_time::seconds(2));
   m_waitForAnswerTimer->stop();

   // Create the connection
   createConnection(api);

   // the main loop
   while (true)
   {
      manageEvents(api);
   }
}

void CSomfySituo::manageEvents(boost::shared_ptr<yApi::IYPluginApi> api)
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
         auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>());
         YADOMS_LOG(information) << "Command received : " << yApi::IDeviceCommand::toString(command);
         if (boost::iequals(command->getKeyword(), m_curtain->getKeyword()))
         {
            m_curtain->set(shared::plugin::yPluginApi::historization::ECurtainCommand(command->getBody()));
            onCommand(api, command->getDevice(), command->getBody());
         }
         else
            YADOMS_LOG(information) << "Received command for unknown keyword from Yadoms : " << yApi::IDeviceCommand::
               toString(command);

         break;
      }
   case yApi::IYPluginApi::kEventUpdateConfiguration:
      {
         // Configuration was updated
         api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
         auto newConfigurationData = api->getEventHandler().getEventData<shared::CDataContainerSharedPtr>();
         YADOMS_LOG(information) << "Update configuration...";
         BOOST_ASSERT(!newConfigurationData->empty());
         // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

         // Close connection
         CSomfySituoConfiguration newConfiguration;
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
            api->setPluginState(yApi::historization::EPluginState::kRunning);

         break;
      }
   case yApi::IYPluginApi::kBindingQuery:
      {
         // Yadoms ask for a binding query 
         auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IBindingQueryRequest>>();
         if (request->getData().getQuery() == "channels")
         {
            std::vector<std::string> allDevices = api->getAllDevices();
            shared::CDataContainerSharedPtr ev = new_CDataContainerSharedPtr();
            for (std::string& str : api->getAllDevices())
            {
               if (str.find(DeviceName) != std::string::npos)
                  ev->set(str.substr(str.length() - 1, 1), str);
            }

            request->sendSuccess(ev);
         }
         else
         {
            // Receive an unsupported query (is your package.json synchronized with the code ?)
            auto errorMessage = (boost::format("unknown query : %1%") % request->getData().getQuery()).str();
            request->sendError(errorMessage);
            YADOMS_LOG(error) << errorMessage;
         }
         break;
      }
   case yApi::IYPluginApi::kEventExtraQuery:
      {
         // Command was received from Yadoms
         auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery>>();
         if (extraQuery)
         {
            if (extraQuery->getData()->query() == "progChannel")
            {
               const auto chanToProg = atoi(extraQuery->getData()->data()->get<std::string>("Channel").c_str());


               if (chanToProg < 1 || static_cast<unsigned int>(chanToProg) > api->getAllDevices().size())
               {
                  YADOMS_LOG(error) << "Unsupported channel : " << chanToProg;
                  extraQuery->sendError("channel must be under  " + std::to_string(api->getAllDevices().size()));
               }
               else
               {
                  YADOMS_LOG(information) << "Channel : " << chanToProg;
                  sendQuickProgCmd(chanToProg);
                  extraQuery->sendSuccess(new_CDataContainerSharedPtr());
               }
            }
            else
            {
               YADOMS_LOG(error) << "Unsupported query : " << extraQuery->getData()->query();
               extraQuery->sendError("unsupported query : " + extraQuery->getData()->query());
            }
         }
         else
         {
            YADOMS_LOG(error) << "Invalid extra-query";
            extraQuery->sendError("invalid paramerter");
         }
         break;
      }
   case kEvtPortConnection:
      {
         auto notif = api->getEventHandler().getEventData<boost::shared_ptr<shared::communication::
            CAsyncPortConnectionNotification>>();

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

         // Message are in ASCII format
         std::string message(reinterpret_cast<const char*>(buffer.begin()), buffer.size());
         YADOMS_LOG(information) << "buffer received from controller : " << message;
         processDataReceived(api, message);
         break;
      }
   case kAnswerTimeout:
      {
         YADOMS_LOG(information) << "No answer received from Controller (timeout)";
         protocolErrorProcess(api);
         break;
      }
   case kProtocolErrorRetryTimer:
      {
         createConnection(api);
         break;
      }
   default:
      {
         YADOMS_LOG(warning) << "Unknown message id " << api->getEventHandler().getEventId();
         break;
      }
   }
}

void CSomfySituo::createConnection(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   // Create the port instance
   m_port = CSomfySituoFactory::constructPort(m_configuration,
                                              api->getEventHandler(),
                                              m_ReceiveBufferHandler,
                                              kEvtPortConnection);
   m_port->start();
}

void CSomfySituo::destroyConnection()
{
   m_port.reset();
   m_waitForAnswerTimer->stop();
}

bool CSomfySituo::connectionsAreEqual(const CSomfySituoConfiguration& conf1,
                                      const CSomfySituoConfiguration& conf2)
{
   return (conf1.getSerialPort() == conf2.getSerialPort());
}

void CSomfySituo::send(const std::string& message,
                       bool needAnswer)
{
   YADOMS_LOG(information) << "Command send : " << message;

   std::vector<unsigned char> messageToSend{STX}; // Header
   for (unsigned int i = 0; i < message.size(); i++)
      messageToSend.push_back(message[i]);
   messageToSend.push_back(ETX); // footer
   shared::communication::CByteBuffer buffer(messageToSend);

   send(buffer, needAnswer);
}


void CSomfySituo::send(const shared::communication::CByteBuffer& buffer,
                       bool needAnswer)
{
   if (!m_port)
      return;

   m_logger->logSent(buffer);
   m_port->send(buffer);
   // Cache the buffer only if not already sending last buffer
   if (&m_lastSentBuffer != &buffer)
      m_lastSentBuffer = buffer;
   if (needAnswer)
      m_waitForAnswerTimer->start();
}

void CSomfySituo::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                            const std::string& device, const std::string& command)
{
   if (!m_port)
      YADOMS_LOG(information) << "Command not send (Controler is not ready) : " << command;

   // Get Device number
   int channelToActivate = ctoi(device[device.length() - 1]);

   // Managet he command
   switch (m_curtain->get())
   {
   case yApi::historization::ECurtainCommand::kStopValue:
      sendQuickMyCmd(channelToActivate);
      break;
   case yApi::historization::ECurtainCommand::kOpenValue:
      sendQuickUpCmd(channelToActivate);
      break;
   case yApi::historization::ECurtainCommand::kCloseValue:
      sendQuickDownCmd(channelToActivate);
      break;
   default:
      YADOMS_LOG(information) << "Unkown command : " << command;
   }
}

void CSomfySituo::processConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Controler port opened";


   try
   {
      // Ask for Controler informations
      sendGetStatusCmd();
   }
   catch (CProtocolException& e)
   {
      YADOMS_LOG(error) << "Protocol error : " << e.what();
      protocolErrorProcess(api);
   }
   catch (shared::communication::CPortException& e)
   {
      YADOMS_LOG(error) << "Error connecting to Controler : " << e.what();
      // Disconnection will be notified, we just have to wait...
   }
}

void CSomfySituo::protocolErrorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   if (m_protocolErrorCounter <= 3)
   {
      ++m_protocolErrorCounter;
      send(m_lastSentBuffer, true);
      return;
   }


   // Retry full connection
   processUnConnectionEvent(api);
   api->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot,
                                      boost::posix_time::seconds(30));
}

void CSomfySituo::processUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api,
                                           boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>
                                           notification)
{
   YADOMS_LOG(information) << "Controler connection was lost";
   if (notification)
      api->setPluginState(yApi::historization::EPluginState::kError,
                          notification->getErrorMessageI18n(),
                          notification->getErrorMessageI18nParameters());
   else
      api->setPluginState(yApi::historization::EPluginState::kError,
                          "connectionFailed");

   destroyConnection();
}

void CSomfySituo::processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                                      const std::string& message)
{
   try
   {
      if (message.length() < 1)
         throw CProtocolException("message size < 1");

      switch (message[0])
      {
      case 's': // Status
         {
            // End of initialization, plugin is now running
            api->setPluginState(yApi::historization::EPluginState::kRunning);

            char sepArray[] = {'-', '\r', 0};
            boost::char_separator<char> sep(sepArray);
            boost::tokenizer<boost::char_separator<char>> tokens(message, sep);

            // Count tokens
            unsigned int tokenCount = 0;
            for (auto itToken = tokens.begin(); itToken != tokens.end(); ++itToken)
               ++tokenCount;

            if (tokenCount != 2)
               throw CProtocolException("invalid status message");

            processReceivedInformation(api, tokens);
            break;
         }
      case 'l': // get the channel
         {
            YADOMS_LOG(information) << "channel message received : " << message;
            if (message.length() == 1)
               YADOMS_LOG(information) << "it was an ACK ";
               //throw CProtocolException("Get a wrong channel response (too short)");
               //do nothing, it's just the ack
            else if (message.length() > 2)
            {
               YADOMS_LOG(error) << "the message is too long ! ";
               throw CProtocolException("Get a wrong channel response (too long)");
            }
            else
            {
               if (ctoi(message[1]) > 0)
               {
                  m_activeChannel = ctoi(message[1]);
                  m_channelSleep = false;
                  YADOMS_LOG(information) << "Active channel : " << m_activeChannel;
               }
               else //the Led's is off. We don't care.
               {
                  m_channelSleep = true;
               }
            }
            break;
         }
      case 'c': //get response from the configuration command
         {
            if (message.length() == 1)
               YADOMS_LOG(information) << "it was an ACK ";
            else
               YADOMS_LOG(information) << "Configuration : " << &message[1];
            break;
         }
      case 'u': // get a ack of the command
      case 'U':
      case 'd':
      case 'D':
      case 'm':
      case 'M':
      case 'p':
      case 'P':
         {
            //do nothing, it was just a ack;
            break;
         }
      default:
         {
            // Maybe some dummy data are at beginning of the string, try to remove them
            auto messageStartPos = message.find_first_of("sludmpUDMP");
            if (messageStartPos == std::string::npos)
            {
               // None of starting characters found, the message is definitively bad...
               throw CProtocolException((boost::format("invalid start byte : %1%") % message[0]).str());
            }

            // Remove bad part of the message, and retry
            processDataReceived(api, message.substr(messageStartPos, message.length() - messageStartPos));
            break;
         }
      }

      // Message was recognized, stop timeout
      m_waitForAnswerTimer->stop();
   }
   catch (CProtocolException& e)
   {
      YADOMS_LOG(information) << "Unable to decode received message : " << e.what();
      protocolErrorProcess(api);
   }
}

void CSomfySituo::sendConfigCmd(ConfigSituo conf, int value)
{
   std::ostringstream cmd;
   cmd << 'C' << conf << value;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}

void CSomfySituo::sendChannelCmd()
{
   std::ostringstream cmd;
   cmd << 'L' << DURATION_PUSH_BTN;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}


void CSomfySituo::sendGetStatusCmd()
{
   std::ostringstream cmd;
   cmd << 'S';
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}

void CSomfySituo::sendUpCmd()
{
   std::ostringstream cmd;
   cmd << 'U' << DURATION_PUSH_BTN;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}

void CSomfySituo::sendDownCmd()
{
   std::ostringstream cmd;
   cmd << 'D' << DURATION_PUSH_BTN;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}

void CSomfySituo::sendMyCmd()
{
   std::ostringstream cmd;
   cmd << 'M' << DURATION_PUSH_BTN;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}

void CSomfySituo::sendProgCmd()
{
   std::ostringstream cmd;
   cmd << 'P' << DURATION_PUSH_BTN;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}


void CSomfySituo::sendQuickUpCmd(int chan)
{
   std::ostringstream cmd;
   cmd << 'u' << chan;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}

void CSomfySituo::sendQuickDownCmd(int chan)
{
   std::ostringstream cmd;
   cmd << 'd' << chan;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}

void CSomfySituo::sendQuickMyCmd(int chan)
{
   std::ostringstream cmd;
   cmd << 'm' << chan;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}

void CSomfySituo::sendQuickProgCmd(int chan)
{
   std::ostringstream cmd;
   cmd << 'p' << chan;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}

void CSomfySituo::processReceivedInformation(boost::shared_ptr<yApi::IYPluginApi> api,
                                             const boost::tokenizer<boost::char_separator<char>>& tokens) const
{
   auto itToken = tokens.begin();
   auto model(*itToken);
   ++itToken;
   auto version(*itToken);

   YADOMS_LOG(information) << "Controler Informations :";
   YADOMS_LOG(information) << "   model   : " << &model[1];
   YADOMS_LOG(information) << "   - version : " << version;

   declareDevice(api, &model[1], version);
}


void CSomfySituo::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& model, const std::string& version) const
{
   int numberOfChannel = 0;
   // Declare device/keywords if necessary
   for (std::map<std::string, int>::const_iterator it = m_somfyModels.begin(); it != m_somfyModels.end(); ++it)
      if (boost::iequals(it->first, model))
         numberOfChannel = it->second;

   auto deviceModel = model + std::string(" ") + version;
   if (numberOfChannel > 0)
   {
      for (int i = 1; i <= numberOfChannel; i++)
      {
         std::string name = DeviceName + std::string("_Channel") + std::to_string(i);
         if (!api->deviceExists(name))
         {
            api->declareDevice(name, deviceModel, deviceModel, m_keywords);
         }
      }
   }
   else
   {
      YADOMS_LOG(information) << "this model of remote control is not handled yet by YADOMS : " << model;
   }
}
