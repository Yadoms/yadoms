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
   kPowerFailureNotificationDelay,
};


const std::string CMegatecUps::DeviceName("UPS");


// Declare the decimal separator for the Megatec protocol (to not depend on locale)
class CMegatecDecimalSeparator : public std::numpunct<char>
{
protected:
   char do_decimal_point() const { return '.'; }
};
static const std::locale ProtocolFloatFormatingLocale(std::locale(), new CMegatecDecimalSeparator());



CMegatecUps::CMegatecUps():
   m_inputVoltage("inputVoltage"), m_inputfaultVoltage("inputfaultVoltage"), m_outputVoltage("outputVoltage"),
   m_outputLoad("outputLoad"), m_inputFrequency("inputFrequency"), m_batteryVoltage("batteryVoltage"),
   m_temperature("temperature"), m_acPowerHistorizer("acPowerActive", yApi::EKeywordAccessMode::kGet), m_upsShutdown("UpsShutdown"),
   m_acPowerActive(true), m_lowBatteryFlag(false), m_lowBatteryByLevelFlag(false),
   m_protocolErrorCounter(0), m_batteryNominalVoltage(0.0), m_answerIsRequired(true), m_firstNotification(true)
{
}

CMegatecUps::~CMegatecUps()
{
}

void CMegatecUps::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

      // the main loop
      YADOMS_LOG(debug) << "CMegatecUps is running...";

      m_waitForAnswerTimer = context->getEventHandler().createTimer(kAnswerTimeout, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(2));
      m_waitForAnswerTimer->stop();
      m_upsStatusRequestTimer = context->getEventHandler().createTimer(kUpsStatusRequestDelay, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(10));
      m_upsStatusRequestTimer->stop();
      m_filterTimer = context->getEventHandler().createTimer(kPowerFailureNotificationDelay);

      // Create the connection
      createConnection(context->getEventHandler());

      while(1)
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
               shared::CDataContainer newConfigurationData = context->getEventHandler().getEventData<shared::CDataContainer>();
               YADOMS_LOG(debug) << "Configuration was updated...";
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
                  createConnection(context->getEventHandler());

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
               std::string message((const char*)(buffer.content()), buffer.size());
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
         case kPowerFailureNotificationDelay:
            {
               if(m_configuration.powerFailureManagement() == IMegatecUpsConfiguration::kFilter)
                  notifyPowerState(context, false);
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

void CMegatecUps::createConnection(shared::event::CEventHandler& eventHandler)
{
   // Create the port instance
   m_port = CMegatecUpsFactory::constructPort(m_configuration, eventHandler, kEvtPortConnection, kEvtPortDataReceived);
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

void CMegatecUps::send(const shared::communication::CByteBuffer& buffer, bool needAnswer, bool answerIsRequired)
{
   if (!m_port)
      return;

   m_logger.logSent(buffer);
   m_port->send(buffer);
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
   context->recordPluginEvent(yApi::IYPluginApi::kInfo, "UPS is now connected");

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
      if (m_lastSentBuffer.content()[0] == 'I')
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
   context->recordPluginEvent(yApi::IYPluginApi::kInfo, "UPS connection was lost");

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
   send(shared::communication::CByteBuffer((unsigned char*) cmd.str().c_str(), cmd.str().size()), true, false);
}

void CMegatecUps::sendGetRatingInformationCmd()
{
   std::ostringstream cmd;
   cmd << 'F' << MEGATEC_EOF;
   m_protocolErrorCounter = 0;
   send(shared::communication::CByteBuffer((unsigned char*) cmd.str().c_str(), cmd.str().size()), true);
}

void CMegatecUps::sendGetStatusCmd()
{
   std::ostringstream cmd;
   cmd << "Q1" << MEGATEC_EOF;
   m_protocolErrorCounter = 0;
   send(shared::communication::CByteBuffer((unsigned char*) cmd.str().c_str(), cmd.str().size()), true);
}

void CMegatecUps::sendToggleBeepCmd()
{
   std::ostringstream cmd;
   cmd << 'Q' << MEGATEC_EOF;
   m_protocolErrorCounter = 0;
   send(shared::communication::CByteBuffer((unsigned char*) cmd.str().c_str(), cmd.str().size()));
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
   send(shared::communication::CByteBuffer((unsigned char*) cmd.str().c_str(), cmd.str().size()));
}

void CMegatecUps::sendCancelShtudownCmd()
{
   std::ostringstream cmd;
   cmd << 'C' << MEGATEC_EOF;
   m_protocolErrorCounter = 0;
   send(shared::communication::CByteBuffer((unsigned char*) cmd.str().c_str(), cmd.str().size()));
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

   // Process AC power status
   processAcPowerStatus(context, !utilityFail, batteryLow);

   // Wait for next status request
   m_upsStatusRequestTimer->start();
}

void CMegatecUps::processReceivedInformation(boost::shared_ptr<yApi::IYPluginApi> context, const boost::tokenizer<boost::char_separator<char> >& tokens)
{
   boost::tokenizer<boost::char_separator<char> >::const_iterator itToken = tokens.begin();
   std::string company(*itToken); ++itToken;
   std::string model  (*itToken); ++itToken;
   std::string version(*itToken);

   YADOMS_LOG(info) << "UPS Informations :";
   YADOMS_LOG(info) << "   - company : " << company;
   YADOMS_LOG(info) << "   - model   : " << model;
   YADOMS_LOG(info) << "   - version : " << version;

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
   if (!context->deviceExists(DeviceName))
   {
      context->declareDevice(DeviceName, model);

      context->declareKeyword(DeviceName, m_inputVoltage);
      context->declareKeyword(DeviceName, m_inputfaultVoltage);
      context->declareKeyword(DeviceName, m_outputVoltage);
      context->declareKeyword(DeviceName, m_outputLoad);
      context->declareKeyword(DeviceName, m_inputFrequency);
      context->declareKeyword(DeviceName, m_batteryVoltage);
      context->declareKeyword(DeviceName, m_temperature);
      context->declareKeyword(DeviceName, m_acPowerHistorizer);
      context->declareKeyword(DeviceName, m_upsShutdown);

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
}

void CMegatecUps::processAcPowerStatus(boost::shared_ptr<yApi::IYPluginApi> context, bool acPowerActive, bool lowBatteryFlag)
{
   switch(m_configuration.powerFailureManagement())
   {
   case IMegatecUpsConfiguration::kNotifyImmediately:
      {
         if (m_firstNotification || (acPowerActive != m_acPowerActive))
            notifyPowerState(context, acPowerActive);
         m_acPowerActive = acPowerActive;
         break;
      }
   case IMegatecUpsConfiguration::kFilter:
      {
         if (m_firstNotification)
            notifyPowerState(context, acPowerActive);
         if (acPowerActive != m_acPowerActive && !acPowerActive)
            m_filterTimer->start(boost::posix_time::seconds(m_configuration.powerFailureFilterDelay()));
         m_acPowerActive = acPowerActive;
         break;
      }
   case IMegatecUpsConfiguration::kLowBattery:
      {
         // No mater if AC power is OK or not
         if (m_firstNotification || (lowBatteryFlag != m_lowBatteryFlag))
            notifyPowerState(context, !lowBatteryFlag);
         m_lowBatteryFlag = lowBatteryFlag;
         break;
      }
   case IMegatecUpsConfiguration::kRemainingBattery:
      {
         // No mater if AC power is OK or not
         bool oldLowBatteryByLevelFlag = m_lowBatteryByLevelFlag;
         getLowBatteryByLevelFlagState(m_lowBatteryByLevelFlag);
         if (m_firstNotification || (m_lowBatteryByLevelFlag != oldLowBatteryByLevelFlag))
            notifyPowerState(context, m_lowBatteryByLevelFlag);
         break;
      }
   default:
      {
         BOOST_ASSERT_MSG(false, "Unsupported powerFailureManagement value");
         break;
      }
   }
   m_firstNotification = false;
}

void CMegatecUps::notifyPowerState(boost::shared_ptr<yApi::IYPluginApi> context, bool powerState)
{
   m_acPowerHistorizer.set(powerState);
   context->historizeData(DeviceName, m_acPowerHistorizer);
}

void CMegatecUps::getLowBatteryByLevelFlagState(bool& flag) const
{
   flag = (m_batteryVoltage < m_configuration.powerFailureRemainingBatteryThreshold());
}

