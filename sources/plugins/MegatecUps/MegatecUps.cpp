#include "stdafx.h"
#include "MegatecUps.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "MegatecUpsFactory.h"
#include <shared/communication/PortException.hpp>
#include <shared/communication/AsciiBufferLogger.h>
#include "ProtocolException.hpp"
#include <shared/Log.h>

IMPLEMENT_PLUGIN(CMegatecUps)


// Event IDs
enum
{
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId, // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kEvtPortDataReceived,
   kProtocolErrorRetryTimer,
   kAnswerTimeout,
   kUpsStatusRequestDelay
};


const std::string CMegatecUps::DeviceName("UPS");


// Declare the decimal separator for the Megatec protocol (to not depend on locale)
class CMegatecDecimalSeparator : public std::numpunct<char>
{
protected:
   char do_decimal_point() const override
   {
      return '.';
   }
};

static const std::locale ProtocolFloatFormatingLocale(std::locale(),
                                                      new CMegatecDecimalSeparator());


CMegatecUps::CMegatecUps()
   : m_logger(boost::make_shared<shared::communication::CAsciiBufferLogger>("trace")),
     m_protocolErrorCounter(0),
     m_lastSentBuffer(1),
     m_answerIsRequired(true),
     m_acPowerActive(true),
     m_batteryNominalVoltage(0.0),
     m_inputVoltage(boost::make_shared<yApi::historization::CVoltage>("inputVoltage")),
     m_inputfaultVoltage(boost::make_shared<yApi::historization::CVoltage>("inputfaultVoltage")),
     m_outputVoltage(boost::make_shared<yApi::historization::CVoltage>("outputVoltage")),
     m_outputLoad(boost::make_shared<yApi::historization::CLoad>("outputLoad")),
     m_inputFrequency(boost::make_shared<yApi::historization::CFrequency>("inputFrequency")),
     m_batteryVoltage(boost::make_shared<yApi::historization::CVoltage>("batteryVoltage")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
     m_acPowerHistorizer(boost::make_shared<yApi::historization::CSwitch>("acPowerActive", yApi::EKeywordAccessMode::kGet)),
     m_batteryLowHistorizer(boost::make_shared<yApi::historization::CSwitch>("batteryLow", yApi::EKeywordAccessMode::kGet)),
     m_upsShutdown(boost::make_shared<yApi::historization::CEvent>("UpsShutdown")),
     m_keywords({m_inputVoltage ,
        m_inputfaultVoltage ,
        m_outputVoltage ,
        m_outputLoad ,
        m_inputFrequency ,
        m_batteryVoltage ,
        m_temperature ,
        m_acPowerHistorizer ,
        m_batteryLowHistorizer ,
        m_upsShutdown})
{
}

CMegatecUps::~CMegatecUps()
{
}

void CMegatecUps::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   YADOMS_LOG(information) << "CMegatecUps is starting..." ;

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout,
                                                             shared::event::CEventTimer::kOneShot,
                                                             boost::posix_time::seconds(2));
   m_waitForAnswerTimer->stop();
   m_upsStatusRequestTimer = api->getEventHandler().createTimer(kUpsStatusRequestDelay,
                                                                shared::event::CEventTimer::kOneShot,
                                                                boost::posix_time::seconds(10));
   m_upsStatusRequestTimer->stop();

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
            YADOMS_LOG(information) << "Stop requested" ;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }
      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // Command received from Yadoms
            auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>());
            YADOMS_LOG(information) << "Command received : " << yApi::IDeviceCommand::toString(command) ;

            if (boost::iequals(command->getKeyword(), m_upsShutdown->getKeyword()))
               onCommandShutdown(api, command->getBody());
            else
            YADOMS_LOG(information) << "Received command for unknown keyword from Yadoms : " << yApi::IDeviceCommand::toString(command) ;

            break;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            // Configuration was updated
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            auto newConfigurationData = api->getEventHandler().getEventData<shared::CDataContainer>();
            YADOMS_LOG(information) << "Update configuration..." ;
            BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

            // Close connection
            CMegatecUpsConfiguration newConfiguration;
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
            const auto buffer(api->getEventHandler().getEventData<const shared::communication::CByteBuffer>());
            m_logger->logReceived(buffer);

            // Message are in ASCII format
            std::string message(reinterpret_cast<const char*>(buffer.begin()), buffer.size());
            processDataReceived(api, message);
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
            break;
         }
      case kProtocolErrorRetryTimer:
         {
            createConnection(api);
            break;
         }
      default:
         {
            YADOMS_LOG(error) << "Unknown message id" ;
            break;
         }
      }
   }
}

void CMegatecUps::createConnection(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   // Create the port instance
   m_port = CMegatecUpsFactory::constructPort(m_configuration,
                                              api->getEventHandler(),
                                              kEvtPortConnection,
                                              kEvtPortDataReceived);
   m_port->start();
}

void CMegatecUps::destroyConnection()
{
   m_port.reset();
   m_waitForAnswerTimer->stop();
   m_upsStatusRequestTimer->stop();
}

bool CMegatecUps::connectionsAreEqual(const CMegatecUpsConfiguration& conf1,
                                      const CMegatecUpsConfiguration& conf2)
{
   return (conf1.getSerialPort() == conf2.getSerialPort());
}

void CMegatecUps::send(const std::string& message,
                       bool needAnswer,
                       bool answerIsRequired)
{
   shared::communication::CByteBuffer buffer(message.size());
   memcpy(buffer.begin(), message.c_str(), message.size());
   send(buffer, needAnswer, answerIsRequired);
}

void CMegatecUps::send(const shared::communication::CByteBuffer& buffer,
                       bool needAnswer,
                       bool answerIsRequired)
{
   if (!m_port)
      return;

   m_logger->logSent(buffer);
   m_port->send(buffer);
   // Cache the buffer only if not already sending last buffer
   if (&m_lastSentBuffer != &buffer)
      m_lastSentBuffer = buffer;
   m_answerIsRequired = answerIsRequired;
   if (needAnswer)
      m_waitForAnswerTimer->start();
}

void CMegatecUps::onCommandShutdown(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const std::string& command)
{
   if (!m_port)
   YADOMS_LOG(information) << "Command not send (UPS is not ready) : " << command ;

   sendShtudownCmd();
}

void CMegatecUps::processConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "UPS port opened" ;


   try
   {
      // Ask for UPS informations
      sendGetInformationCmd();
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

void CMegatecUps::protocolErrorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   if (m_protocolErrorCounter <= 3)
   {
      ++m_protocolErrorCounter;
      send(m_lastSentBuffer, true, m_answerIsRequired);
      return;
   }

   // Already 3 tries, and still no answer...
   if (!m_answerIsRequired)
   {
      // The no-answer for the last command can be omitted.
      // This is done for some UPS not supporting identification 'I' command (not responding to)
      if (m_lastSentBuffer[0] == 'I')
      {
         // Declare the device with default string
         declareDevice(api, "noname UPS");

         // Now ask for rating informations
         sendGetRatingInformationCmd();

         return;
      }
   }

   // Retry full connection
   processUnConnectionEvent(api);
   api->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CMegatecUps::processUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "UPS connection was lost" ;
   api->setPluginState(yApi::historization::EPluginState::kError, "connectionFailed");

   destroyConnection();
}

void CMegatecUps::processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                                      const std::string& message)
{
   try
   {
      if (message.length() < 1)
         throw CProtocolException("message size < 1");

      switch (message[0])
      {
      case '(': // Status
         {
            char sepArray[] = {'(', ' ', MEGATEC_EOF, 0};
            boost::char_separator<char> sep(sepArray);
            boost::tokenizer<boost::char_separator<char>> tokens(message, sep);

            // Count tokens
            unsigned int tokenCount = 0;
            for (boost::tokenizer<boost::char_separator<char>>::const_iterator itToken = tokens.begin(); itToken != tokens.end(); ++itToken)
               ++tokenCount;

            if (tokenCount != 8)
               throw CProtocolException("invalid status message");

            processReceivedStatus(api, tokens);
            break;
         }
      case '#': // Information or rating information
         {
            char sepArray[] = {'#', ' ', MEGATEC_EOF, 0};
            boost::char_separator<char> sep(sepArray);
            boost::tokenizer<boost::char_separator<char>> tokens(message, sep);

            // Count tokens
            unsigned int tokenCount = 0;
            for (auto itToken = tokens.begin(); itToken != tokens.end(); ++itToken)
               ++tokenCount;

            if (tokenCount == 3)
            {
               processReceivedInformation(api, tokens);

               // Now ask for rating informations
               sendGetRatingInformationCmd();
            }
            else if (tokenCount == 4)
            {
               processReceivedRatingInformation(tokens);

               // End of initialization, plugin is now running
               api->setPluginState(yApi::historization::EPluginState::kRunning);

               // Now ask for status
               sendGetStatusCmd();
            }
            else
               throw CProtocolException("invalid information message");
            break;
         }
      default:
         {
            // Maybe some dummy data are at beginning of the string, try to remove them
            auto messageStartPos = message.find_first_of("(#");
            if (messageStartPos == std::string::npos)
            {
               // None of starting characters found, the message is definitively bad...
               throw CProtocolException((boost::format("invalid start byte : %1%") % message[0]).str());
            }

            // Remove bad part of the message, and retry
            processDataReceived(api, message.substr(messageStartPos, message.length() - messageStartPos));
         }
      }

      // Message was recognized, stop timeout
      m_waitForAnswerTimer->stop();
   }
   catch (CProtocolException& e)
   {
      YADOMS_LOG(information) << "Unable to decode received message : " << e.what() ;
      protocolErrorProcess(api);
   }
}

void CMegatecUps::sendGetInformationCmd()
{
   std::ostringstream cmd;
   cmd << 'I' << MEGATEC_EOF;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true, false);
}

void CMegatecUps::sendGetRatingInformationCmd()
{
   std::ostringstream cmd;
   cmd << 'F' << MEGATEC_EOF;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}

void CMegatecUps::sendGetStatusCmd()
{
   std::ostringstream cmd;
   cmd << "Q1" << MEGATEC_EOF;
   m_protocolErrorCounter = 0;
   send(cmd.str(), true);
}

void CMegatecUps::sendToggleBeepCmd()
{
   std::ostringstream cmd;
   cmd << 'Q' << MEGATEC_EOF;
   m_protocolErrorCounter = 0;
   send(cmd.str());
}

void CMegatecUps::sendShtudownCmd()
{
   std::ostringstream cmd;
   cmd.imbue(ProtocolFloatFormatingLocale);

   cmd << 'S';
   if (m_configuration.outputShutdownDelay() < 1)
   {
      // Number <1 should be at format ".2", ".3"

      // No easy solution to format a decimal number without the '0' before the '.'
      std::ostringstream value;
      value << std::setw(2) << std::setprecision(1) << std::fixed << m_configuration.outputShutdownDelay();
      std::string sValue = value.str();
      cmd << sValue.substr(1, sValue.size() - 1);
   }
   else
      cmd << std::setw(2) << std::setfill('0') << m_configuration.outputShutdownDelay();// Number >=1 should be at format "01", "02", "10"

   cmd << 'R' << std::setw(4) << std::setprecision(0) << std::setfill('0') << m_configuration.outputRestoreDelay();

   cmd << MEGATEC_EOF;

   m_protocolErrorCounter = 0;
   send(cmd.str());
}

void CMegatecUps::processReceivedStatus(boost::shared_ptr<yApi::IYPluginApi> api,
                                        const boost::tokenizer<boost::char_separator<char>>& tokens)
{
   auto itToken = tokens.begin();
   m_inputVoltage->set(upsStr2Double(*itToken));
   ++itToken;
   m_inputfaultVoltage->set(upsStr2Double(*itToken));
   ++itToken;
   m_outputVoltage->set(upsStr2Double(*itToken));
   ++itToken;
   m_outputLoad->set(upsStr2Double(*itToken));
   ++itToken;
   m_inputFrequency->set(upsStr2Double(*itToken));
   ++itToken;
   m_batteryVoltage->set(upsStr2Double(*itToken));
   ++itToken;
   m_temperature->set(upsStr2Double(*itToken));
   ++itToken;

   const auto& status = *itToken;
   if (status.size() != 8)
      throw CProtocolException("Invalid status size");
   auto utilityFail = status[0] == '1';
   auto batteryLow = status[1] == '1';
   auto bypassActive = status[2] == '1';
   auto upsFailed = status[3] == '1';
   auto upsIsStandby = status[4] == '1';
   auto testInProgress = status[5] == '1';
   auto shutdownActive = status[6] == '1';
   auto beeperOn = status[7] == '1';

   m_batteryLowHistorizer->set(batteryLow);
   m_acPowerHistorizer->set(!utilityFail);

   api->historizeData(DeviceName, m_keywords);

   YADOMS_LOG(information) << "UPS current informations : inputVoltage=" << m_inputVoltage->get() <<
      ", inputfaultVoltage=" << m_inputfaultVoltage->get() <<
      ", outputVoltage=" << m_outputVoltage->get() <<
      ", m_outputLoad=" << m_outputLoad->get() <<
      ", inputFrequency=" << m_inputFrequency->get() <<
      ", batteryVoltage=" << m_batteryVoltage->get() <<
      ", temperature=" << m_temperature->get() ;
   YADOMS_LOG(information) << "UPS status : utilityFail=" << (utilityFail ? "YES" : "NO") <<
      ", batteryLow=" << (batteryLow ? "YES" : "NO") <<
      ", bypassActive=" << (bypassActive ? "YES" : "NO") <<
      ", upsFailed=" << (upsFailed ? "YES" : "NO") <<
      ", upsIsStandby=" << (upsIsStandby ? "YES" : "NO") <<
      ", testInProgress=" << (testInProgress ? "YES" : "NO") <<
      ", shutdownActive=" << (shutdownActive ? "YES" : "NO") <<
      ", beeperOn=" << (beeperOn ? "YES" : "NO") ;

   // Toggle beep if not in expected state
   if (beeperOn != m_configuration.upsBeepEnable())
      sendToggleBeepCmd();

   // Wait for next status request
   m_upsStatusRequestTimer->start();
}

void CMegatecUps::processReceivedInformation(boost::shared_ptr<yApi::IYPluginApi> api,
                                             const boost::tokenizer<boost::char_separator<char>>& tokens) const
{
   auto itToken = tokens.begin();
   auto company(*itToken);
   ++itToken;
   auto model(*itToken);
   ++itToken;
   auto version(*itToken);

   YADOMS_LOG(information) << "UPS Informations :" ;
   YADOMS_LOG(information) << "   - company : " << company ;
   YADOMS_LOG(information) << "   - model   : " << model ;
   YADOMS_LOG(information) << "   - version : " << version ;

   // Declare device/keywords if necessary
   declareDevice(api, company + std::string(" ") + model + std::string(" ") + version);
}

void CMegatecUps::processReceivedRatingInformation(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
   auto itToken = tokens.begin();
   auto ratingVoltage = upsStr2Double(*itToken);
   ++itToken;
   auto ratingCurrent = upsStr2Double(*itToken);
   ++itToken;
   m_batteryNominalVoltage = upsStr2Double(*itToken);
   ++itToken;
   auto frequency = upsStr2Double(*itToken);

   YADOMS_LOG(information) << "UPS rating informations : voltage=" << ratingVoltage << ", current=" << ratingCurrent << ", batteryVoltage=" << m_batteryNominalVoltage << ", frequency=" << frequency ;
}

double CMegatecUps::upsStr2Double(const std::string& str)
{
   double number;
   std::istringstream convert(str);
   convert.imbue(ProtocolFloatFormatingLocale);
   if (!(convert >> number))
   {
      YADOMS_LOG(information) << "Unable to decode number \"" << str << "\"" ;
      number = 0.0;
   }
   return number;
}

void CMegatecUps::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& model) const
{
   if (!api->deviceExists(DeviceName))
   {
      api->declareDevice(DeviceName, model, model, m_keywords);

      // Force a first historization to let Yadoms know the shutdown state
      api->historizeData(DeviceName, m_upsShutdown);
   }
}

