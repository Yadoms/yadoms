#include "stdafx.h"
#include "MegatecUps.h"
#include <shared/Log.h>
#include <shared/plugin/ImplementationHelper.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include "MegatecUpsFactory.h"
#include <shared/communication/PortException.hpp>
#include "ProtocolException.hpp"

IMPLEMENT_PLUGIN(CMegatecUps)


// Event IDs
enum
{
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kEvtPortDataReceived,
   kProtocolErrorRetryTimer,
   kAnswerTimeout,
   kUpsStatusRequestDelay,
};


const std::string CMegatecUps::DeviceName("UPS");


// Declare the decimal separator for the Megatec protocol (to not depend on locale)
class CMegatecDecimalSeparator : public std::numpunct<char>
{
protected:
   char do_decimal_point() const { return '.'; }
};
static const std::locale ProtocolFloatFormatingLocale(std::locale(), new CMegatecDecimalSeparator());



CMegatecUps::CMegatecUps()
   :m_protocolErrorCounter(0), m_lastSentBuffer(1), m_answerIsRequired(true),
   m_acPowerActive(true), m_batteryNominalVoltage(0.0),
   m_inputVoltage("inputVoltage"), m_inputfaultVoltage("inputfaultVoltage"), m_outputVoltage("outputVoltage"),
   m_outputLoad("outputLoad"), m_inputFrequency("inputFrequency"), m_batteryVoltage("batteryVoltage"),
   m_temperature("temperature"), m_acPowerHistorizer("acPowerActive", yApi::EKeywordAccessMode::kGet),
   m_batteryLowHistorizer("batteryLow", yApi::EKeywordAccessMode::kGet), m_upsShutdown("UpsShutdown")
{
}

CMegatecUps::~CMegatecUps()
{
}

void CMegatecUps::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      context->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

      YADOMS_LOG(debug) << "CMegatecUps is starting...";

      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

      m_waitForAnswerTimer = context->getEventHandler().createTimer(kAnswerTimeout, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(2));
      m_waitForAnswerTimer->stop();
      m_upsStatusRequestTimer = context->getEventHandler().createTimer(kUpsStatusRequestDelay, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(10));
      m_upsStatusRequestTimer->stop();

      // Create the connection
      createConnection(context);

      // the main loop
      while (true)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command received from Yadoms
               boost::shared_ptr<const yApi::IDeviceCommand> command(context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >());
               YADOMS_LOG(debug) << "Command received :" << command->toString();

               if (boost::iequals(command->getKeyword(), m_upsShutdown.getKeyword()))
                  onCommand(context, command->getBody());
               else
                  YADOMS_LOG(warning) << "Received command for unknown keyword from Yadoms : " << command->toString();

               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               context->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               shared::CDataContainer newConfigurationData = context->getEventHandler().getEventData<shared::CDataContainer>();
               YADOMS_LOG(debug) << "Update configuration...";
               BOOST_ASSERT(!newConfigurationData.empty());  // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Close connection
               CMegatecUpsConfiguration newConfiguration;
               newConfiguration.initializeWith(newConfigurationData);

               // If port has changed, destroy and recreate connection (if any)
               bool needToReconnect = !connectionsAreEqual(m_configuration, newConfiguration) && !!m_port;
               
               if (needToReconnect)
                  destroyConnection();

               // Update configuration
               m_configuration.initializeWith(newConfigurationData);

               if (needToReconnect)
                  createConnection(context);

               context->setPluginState(yApi::historization::EPluginState::kRunning);
               break;
            }
         case kEvtPortConnection:
            {
               if (context->getEventHandler().getEventData<bool>())
                  processConnectionEvent(context);
               else
                  processUnConnectionEvent(context);

               break;
            }
         case kEvtPortDataReceived:
            {
               const shared::communication::CByteBuffer buffer(context->getEventHandler().getEventData<const shared::communication::CByteBuffer>());
               m_logger.logReceived(buffer);

               // Message are in ASCII format
               std::string message(reinterpret_cast<const char*>(buffer.begin()), buffer.size());
               processDataReceived(context, message);
               break;
            }
         case kAnswerTimeout:
            {
               YADOMS_LOG(warning) << "No answer received from UPS (timeout)";
               protocolErrorProcess(context);
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
               createConnection(context);
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

void CMegatecUps::createConnection(boost::shared_ptr<yApi::IYPluginApi> context)
{
   context->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   // Create the port instance
   m_port = CMegatecUpsFactory::constructPort(m_configuration, context->getEventHandler(), kEvtPortConnection, kEvtPortDataReceived);
   m_port->start();
}

void CMegatecUps::destroyConnection()
{
   m_port.reset();
   m_waitForAnswerTimer->stop();
   m_upsStatusRequestTimer->stop();
}

bool CMegatecUps::connectionsAreEqual(const CMegatecUpsConfiguration& conf1, const CMegatecUpsConfiguration& conf2) const
{
   return (conf1.getSerialPort() == conf2.getSerialPort());
}

void CMegatecUps::send(const std::string& message, bool needAnswer, bool answerIsRequired)
{
   shared::communication::CByteBuffer buffer(message.size());
   memcpy(buffer.begin(), message.c_str(), message.size());
   send(buffer, needAnswer, answerIsRequired);
}

void CMegatecUps::send(const shared::communication::CByteBuffer& buffer, bool needAnswer, bool answerIsRequired)
{
   if (!m_port)
      return;

   m_logger.logSent(buffer);
   m_port->send(buffer);
   // Cache the buffer only if not already sending last buffer
   if (&m_lastSentBuffer != &buffer)
      m_lastSentBuffer = buffer;
   m_answerIsRequired = answerIsRequired;
   if (needAnswer)
      m_waitForAnswerTimer->start();
}

void CMegatecUps::onCommand(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& command)
{
   if (!m_port)
      YADOMS_LOG(warning) << "Command not send (UPS is not ready) : " << command;

   m_upsShutdown.set(command);
   if (m_upsShutdown.get())
      sendShtudownCmd();
   else
      sendCancelShtudownCmd();
}

void CMegatecUps::processConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> context)
{
   YADOMS_LOG(debug) << "UPS is now connected";
   context->setPluginState(yApi::historization::EPluginState::kRunning);

   try
   {
      // Ask for UPS informations
      sendGetInformationCmd();
   }
   catch (CProtocolException& e)
   {
      YADOMS_LOG(error) << "Protocol error : " << e.what();
      protocolErrorProcess(context);
   }
   catch (shared::communication::CPortException& e)
   {
      YADOMS_LOG(error) << "Error connecting to UPS : " << e.what();
      // Disconnection will be notified, we just have to wait...
   }
}

void CMegatecUps::protocolErrorProcess(boost::shared_ptr<yApi::IYPluginApi> context)
{
   ++m_protocolErrorCounter;
   if (m_protocolErrorCounter < 3)
   {
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
         declareDevice(context, "noname UPS");

         // Now ask for rating informations
         sendGetRatingInformationCmd();

         return;
      }
   }

   // Retry full connection
   destroyConnection();
   context->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CMegatecUps::processUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> context)
{
   YADOMS_LOG(debug) << "UPS connection was lost";
   context->setPluginState(yApi::historization::EPluginState::kError, "connectionFailed");

   destroyConnection();
}

void CMegatecUps::processDataReceived(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& message)
{
   try
   {
      if (message.length() < 1)
         throw CProtocolException("message size < 1");

      switch(message[0])
      {
      case '(':      // Status
         {
            char sepArray[] = { '(', ' ', MEGATEC_EOF, 0 };
            boost::char_separator<char> sep(sepArray);
            boost::tokenizer<boost::char_separator<char> > tokens(message, sep);

            // Count tokens
            unsigned int tokenCount = 0;
            for (boost::tokenizer<boost::char_separator<char> >::const_iterator itToken = tokens.begin() ; itToken != tokens.end() ; ++itToken)
               ++tokenCount;

            if (tokenCount != 8)
               throw CProtocolException("invalid status message");

            processReceivedStatus(context, tokens);
            break;
         }
      case '#':      // Information or rating information
         {
            char sepArray[] = { '#', ' ', MEGATEC_EOF, 0 };
            boost::char_separator<char> sep(sepArray);
            boost::tokenizer<boost::char_separator<char> > tokens(message, sep);

            // Count tokens
            unsigned int tokenCount = 0;
            for (boost::tokenizer<boost::char_separator<char> >::const_iterator itToken = tokens.begin() ; itToken != tokens.end() ; ++itToken)
               ++tokenCount;

            if (tokenCount == 3)
            {
               processReceivedInformation(context, tokens);

               // Now ask for rating informations
               sendGetRatingInformationCmd();
            }
            else if (tokenCount == 4)
            {
               processReceivedRatingInformation(tokens);

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
            size_t messageStartPos = message.find_first_of ("(#");
            if (messageStartPos == std::string::npos)
            {
               // None of starting characters found, the message is definitively bad...
               throw CProtocolException((boost::format("invalid start byte : %1%") % message[0]).str());
            }

            // Remove bad part of the message, and retry
            processDataReceived(context, message.substr(messageStartPos, message.length() - messageStartPos));
         }
      }

      // Message was recognized, stop timeout
      m_waitForAnswerTimer->stop();
   }
   catch (CProtocolException& e)
   {
      YADOMS_LOG(warning) << "Unable to decode received message : " << e.what();
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
   if (m_configuration.outuputShutdownDelay() < 1)
   {
      // Number <1 should be at format ".2", ".3"

      // No easy solution to format a decimal number without the '0' before the '.'
      std::ostringstream value;
      value << std::setw(2) << std::setprecision(1) << std::fixed << m_configuration.outuputShutdownDelay();
      std::string sValue = value.str();
      cmd << sValue.substr(1, sValue.size()-1);
   }
   else
      cmd << std::setw(2) << std::setfill('0') << m_configuration.outuputShutdownDelay();// Number >=1 should be at format "01", "02", "10"

   cmd << 'R' << std::setw(4) << std::setprecision(0) << std::setfill('0') << m_configuration.outuputRestoreDelay();

   cmd << MEGATEC_EOF;

   m_protocolErrorCounter = 0;
   send(cmd.str());
}

void CMegatecUps::sendCancelShtudownCmd()
{
   std::ostringstream cmd;
   cmd << 'C' << MEGATEC_EOF;
   m_protocolErrorCounter = 0;
   send(cmd.str());
}

void CMegatecUps::processReceivedStatus(boost::shared_ptr<yApi::IYPluginApi> context, const boost::tokenizer<boost::char_separator<char> >& tokens)
{
   boost::tokenizer< boost::char_separator<char> >::const_iterator itToken = tokens.begin();
   m_inputVoltage.set      (upsStr2Double(*itToken)); ++itToken;
   m_inputfaultVoltage.set (upsStr2Double(*itToken)); ++itToken;
   m_outputVoltage.set     (upsStr2Double(*itToken)); ++itToken;
   m_outputLoad.set        (upsStr2Double(*itToken)); ++itToken;
   m_inputFrequency.set    (upsStr2Double(*itToken)); ++itToken;
   m_batteryVoltage.set    (upsStr2Double(*itToken)); ++itToken;
   m_temperature.set       (upsStr2Double(*itToken)); ++itToken;
   
   const std::string& status = *itToken;
   if (status.size() != 8)
      throw CProtocolException("Invalid status size");
   bool utilityFail = status[0] == '1';
   bool batteryLow = status[1] == '1';
   bool bypassActive = status[2] == '1';
   bool upsFailed = status[3] == '1';
   bool upsIsStandby = status[4] == '1';
   bool testInProgress = status[5] == '1';
   bool shutdownActive = status[6] == '1';
   bool beeperOn = status[7] == '1';

   m_batteryLowHistorizer.set(batteryLow);
   m_acPowerHistorizer.set(!utilityFail);

   historizeData(context);

   YADOMS_LOG(debug) << "UPS current informations : inputVoltage=" << m_inputVoltage.get() <<
      ", inputfaultVoltage="  << m_inputfaultVoltage.get() <<
      ", outputVoltage="      << m_outputVoltage.get() <<
      ", m_outputLoad="       << m_outputLoad.get() <<
      ", inputFrequency="     << m_inputFrequency.get() <<
      ", batteryVoltage="     << m_batteryVoltage.get() <<
      ", temperature="        << m_temperature.get();
   YADOMS_LOG(debug) << "UPS status : utilityFail=" << (utilityFail ? "YES" : "NO") <<
      ", batteryLow="         << (batteryLow ? "YES" : "NO") <<
      ", bypassActive="       << (bypassActive ? "YES" : "NO") <<
      ", upsFailed="          << (upsFailed ? "YES" : "NO") <<
      ", upsIsStandby="       << (upsIsStandby ? "YES" : "NO") <<
      ", testInProgress="     << (testInProgress ? "YES" : "NO") <<
      ", shutdownActive="     << (shutdownActive ? "YES" : "NO") <<
      ", beeperOn="           << (beeperOn ? "YES" : "NO");

   // Toggle beep if not in expected state
   if (beeperOn != m_configuration.upsBeepEnable())
      sendToggleBeepCmd();

   // Wait for next status request
   m_upsStatusRequestTimer->start();
}

void CMegatecUps::processReceivedInformation(boost::shared_ptr<yApi::IYPluginApi> context, const boost::tokenizer<boost::char_separator<char> >& tokens)
{
   boost::tokenizer<boost::char_separator<char> >::const_iterator itToken = tokens.begin();
   std::string company(*itToken); ++itToken;
   std::string model  (*itToken); ++itToken;
   std::string version(*itToken);

   YADOMS_LOG(information) << "UPS Informations :";
   YADOMS_LOG(information) << "   - company : " << company;
   YADOMS_LOG(information) << "   - model   : " << model;
   YADOMS_LOG(information) << "   - version : " << version;

   // Declare device/keywords if necessary
   declareDevice(context, company + std::string(" ") + model + std::string(" ") + version);
}

void CMegatecUps::processReceivedRatingInformation(const boost::tokenizer<boost::char_separator<char> >& tokens)
{
   boost::tokenizer<boost::char_separator<char> >::const_iterator itToken = tokens.begin();
   double ratingVoltage = upsStr2Double(*itToken);
   ++itToken;
   double ratingCurrent = upsStr2Double(*itToken);
   ++itToken;
   m_batteryNominalVoltage = upsStr2Double(*itToken);
   ++itToken;
   double frequency = upsStr2Double(*itToken);

   YADOMS_LOG(debug) << "UPS rating informations : voltage=" << ratingVoltage << ", current=" << ratingCurrent << ", batteryVoltage=" << m_batteryNominalVoltage << ", frequency=" << frequency;
}

double CMegatecUps::upsStr2Double(const std::string& str)
{
   double number;
   std::istringstream convert(str);
   convert.imbue(ProtocolFloatFormatingLocale);
   if (!(convert >> number))
   {
      YADOMS_LOG(warning) << "Unable to decode number \"" << str << "\"";
      number = 0.0;
   }
   return number;
}

void CMegatecUps::declareDevice(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& model) const
{
   bool needFirstHistorization = false;

   if (!context->deviceExists(DeviceName))
   {
      context->declareDevice(DeviceName, model);
      needFirstHistorization = true;
   }

   if (!context->keywordExists(DeviceName, m_inputVoltage))          context->declareKeyword(DeviceName, m_inputVoltage);
   if (!context->keywordExists(DeviceName, m_inputfaultVoltage))     context->declareKeyword(DeviceName, m_inputfaultVoltage);
   if (!context->keywordExists(DeviceName, m_outputVoltage))         context->declareKeyword(DeviceName, m_outputVoltage);
   if (!context->keywordExists(DeviceName, m_outputLoad))            context->declareKeyword(DeviceName, m_outputLoad);
   if (!context->keywordExists(DeviceName, m_inputFrequency))        context->declareKeyword(DeviceName, m_inputFrequency);
   if (!context->keywordExists(DeviceName, m_batteryVoltage))        context->declareKeyword(DeviceName, m_batteryVoltage);
   if (!context->keywordExists(DeviceName, m_temperature))           context->declareKeyword(DeviceName, m_temperature);
   if (!context->keywordExists(DeviceName, m_acPowerHistorizer))     context->declareKeyword(DeviceName, m_acPowerHistorizer);
   if (!context->keywordExists(DeviceName, m_batteryLowHistorizer))  context->declareKeyword(DeviceName, m_batteryLowHistorizer);
   if (!context->keywordExists(DeviceName, m_upsShutdown))           context->declareKeyword(DeviceName, m_upsShutdown);

   if (needFirstHistorization)
   {
      // Force a first historization to let Yadoms know the shutdown state
      context->historizeData(DeviceName, m_upsShutdown);
   }
}

void CMegatecUps::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(DeviceName, m_inputVoltage);
   context->historizeData(DeviceName, m_inputfaultVoltage);
   context->historizeData(DeviceName, m_outputVoltage);
   context->historizeData(DeviceName, m_outputLoad);
   context->historizeData(DeviceName, m_inputFrequency);
   context->historizeData(DeviceName, m_batteryVoltage);
   context->historizeData(DeviceName, m_temperature);
   context->historizeData(DeviceName, m_acPowerHistorizer);
   context->historizeData(DeviceName, m_batteryLowHistorizer);
}