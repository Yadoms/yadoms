#include "stdafx.h"
#include "EnOcean.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "EnOceanFactory.h"
#include <shared/communication/PortException.hpp>
#include "ProtocolException.hpp"

IMPLEMENT_PLUGIN(CEnOcean)


// Event IDs
enum
{
   //TODO faire le ménage dans les enum
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId, // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kEvtPortDataReceived,
   kProtocolErrorRetryTimer,
   kAnswerTimeout,
};


CEnOcean::CEnOcean()
{
}

CEnOcean::~CEnOcean()
{
}

void CEnOcean::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   std::cout << "CEnOcean is starting..." << std::endl;

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                             shared::event::CEventTimer::kOneShot,
                                                             boost::posix_time::milliseconds(500));
   m_waitForAnswerTimer->stop();

   // Create the connection
   createConnection(api);

   // the main loop
   while (true)
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
            std::cout << "Command received : " << yApi::IDeviceCommand::toString(command) << std::endl;

            onCommand(api, command);

            break;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            // Configuration was updated
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            auto newConfigurationData = api->getEventHandler().getEventData<shared::CDataContainer>();
            std::cout << "Update configuration..." << std::endl;
            BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

            // Close connection
            CEnOceanConfiguration newConfiguration;
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
      case kEvtPortConnection:
         {
            if (api->getEventHandler().getEventData<bool>())
               processConnectionEvent(api);
            else
               processUnConnectionEvent(api);

            break;
         }
      case kEvtPortDataReceived:
         {
            const auto message(api->getEventHandler().getEventData<const EnOceanMessage::CMessage>());
            //TODO            m_logger.logReceived(buffer);

            processDataReceived(api, message);
            break;
         }
      case kAnswerTimeout:
         {
            std::cout << "No answer received from EnOcean dongle (timeout)" << std::endl;
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
            std::cerr << "Unknown message id" << std::endl;
            break;
         }
      }
   }
}

void CEnOcean::createConnection(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   // Create the port instance
   m_port = CEnOceanFactory::constructPort(m_configuration,
                                           api->getEventHandler(),
                                           kEvtPortConnection,
                                           kEvtPortDataReceived);
   m_port->start();
}

void CEnOcean::destroyConnection()
{
   m_port.reset();
   m_waitForAnswerTimer->stop();
}

bool CEnOcean::connectionsAreEqual(const CEnOceanConfiguration& conf1,
                                   const CEnOceanConfiguration& conf2)
{
   return (conf1.getSerialPort() == conf2.getSerialPort());
}

void CEnOcean::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                         boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command)
{
   if (!m_port)
      std::cout << "Command not send (dongle is not ready) : " << command << std::endl;

   //TODO
}

void CEnOcean::send(const std::string& message,
                    bool needAnswer,
                    bool answerIsRequired)
{
   shared::communication::CByteBuffer buffer(message.size());
   memcpy(buffer.begin(), message.c_str(), message.size());
   send(buffer, needAnswer, answerIsRequired);
}

void CEnOcean::send(const shared::communication::CByteBuffer& buffer,
                    bool needAnswer,
                    bool answerIsRequired)
{
   if (!m_port)
      return;

   m_logger.logSent(buffer);
   m_port->send(buffer);

   if (needAnswer)
      m_waitForAnswerTimer->start();
}

void CEnOcean::processConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "EnOcean port opened" << std::endl;

   try
   {
      //TODO
   }
   catch (CProtocolException& e)
   {
      std::cerr << "Protocol error : " << e.what();
      protocolErrorProcess(api);
   }
   catch (shared::communication::CPortException& e)
   {
      std::cerr << "Error connecting to EnOcean dongle : " << e.what();
      // Disconnection will be notified, we just have to wait...
   }
}

void CEnOcean::protocolErrorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Retry full connection
   processUnConnectionEvent(api);
   api->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CEnOcean::processUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "EnOcean connection was lost" << std::endl;
   api->setPluginState(yApi::historization::EPluginState::kError, "connectionFailed");

   destroyConnection();
}

void CEnOcean::processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const EnOceanMessage::CMessage& message)
{
   try
   {
      //TODO
      //if (message.length() < 1)
      //   throw CProtocolException("message size < 1");

      ////TODO
      //switch (message[0])
      //{
      //case '(': // Status
      //   {
      //      char sepArray[] = {'(', ' ', MEGATEC_EOF, 0};
      //      boost::char_separator<char> sep(sepArray);
      //      boost::tokenizer<boost::char_separator<char> > tokens(message, sep);

      //      // Count tokens
      //      unsigned int tokenCount = 0;
      //      for (boost::tokenizer<boost::char_separator<char> >::const_iterator itToken = tokens.begin(); itToken != tokens.end(); ++itToken)
      //         ++tokenCount;

      //      if (tokenCount != 8)
      //         throw CProtocolException("invalid status message");

      //      processReceivedStatus(api, tokens);
      //      break;
      //   }
      //case '#': // Information or rating information
      //   {
      //      char sepArray[] = {'#', ' ', MEGATEC_EOF, 0};
      //      boost::char_separator<char> sep(sepArray);
      //      boost::tokenizer<boost::char_separator<char> > tokens(message, sep);

      //      // Count tokens
      //      unsigned int tokenCount = 0;
      //      for (auto itToken = tokens.begin(); itToken != tokens.end(); ++itToken)
      //         ++tokenCount;

      //      if (tokenCount == 3)
      //      {
      //         processReceivedInformation(api, tokens);

      //         // Now ask for rating informations
      //         sendGetRatingInformationCmd();
      //      }
      //      else if (tokenCount == 4)
      //      {
      //         processReceivedRatingInformation(tokens);

      //         // End of initialization, plugin is now running
      //         api->setPluginState(yApi::historization::EPluginState::kRunning);

      //         // Now ask for status
      //         sendGetStatusCmd();
      //      }
      //      else
      //         throw CProtocolException("invalid information message");
      //      break;
      //   }
      //default:
      //   {
      //      // Maybe some dummy data are at beginning of the string, try to remove them
      //      auto messageStartPos = message.find_first_of("(#");
      //      if (messageStartPos == std::string::npos)
      //      {
      //         // None of starting characters found, the message is definitively bad...
      //         throw CProtocolException((boost::format("invalid start byte : %1%") % message[0]).str());
      //      }

      //      // Remove bad part of the message, and retry
      //      processDataReceived(api, message.substr(messageStartPos, message.length() - messageStartPos));
      //   }
      //}

      // Message was recognized, stop timeout
      m_waitForAnswerTimer->stop();
   }
   catch (CProtocolException& e)
   {
      std::cout << "Unable to decode received message : " << e.what() << std::endl;
      protocolErrorProcess(api);
   }
}

