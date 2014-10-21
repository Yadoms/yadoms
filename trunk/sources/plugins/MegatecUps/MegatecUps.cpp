#include "stdafx.h"
#include "MegatecUps.h"
#include <shared/Log.h>
#include <shared/plugin/ImplementationHelper.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include "MegatecUpsFactory.h"
#include <shared/communication/PortException.hpp>
#include "ProtocolException.hpp"

IMPLEMENT_PLUGIN(CMegatecUps)


// Event IDs
enum
{
   kEvtPortConnection = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from yApi::IYadomsApi::kPluginFirstEventId
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
   m_outputCurrent("outputCurrent"), m_inputFrequency("inputFrequency"), m_batteryVoltage("batteryVoltage"),
   m_temperature("temperature"), m_acPowerHistorizer("acPowerActive"), m_upsShutdown("UPS Shutdown"),
   m_acPowerActive(true), m_lowBatteryFlag(false), m_lowBatteryByLevelFlag(false), m_batteryNominalVoltage(0.0)
{
}

CMegatecUps::~CMegatecUps()
{
}

void CMegatecUps::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

      // the main loop
      YADOMS_LOG(debug) << "CMegatecUps is running...";

      m_waitForAnswerTimer = context->getEventHandler().createTimer(kAnswerTimeout, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(5));
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
         case yApi::IYadomsApi::kEventDeviceCommand:
            {
               // Command received from Yadoms
               boost::shared_ptr<const yApi::IDeviceCommand> command(context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >());
               YADOMS_LOG(debug) << "Command received :" << command->toString();

               onCommand(context, command->getBody(), context->getDeviceDetails(command->getTargetDevice()));

               break;
            }
         case yApi::IYadomsApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               shared::CDataContainer newConfigurationData = context->getEventHandler().getEventData<shared::CDataContainer>();
               YADOMS_LOG(debug) << "Configuration was updated...";
               BOOST_ASSERT(!newConfigurationData.empty());  // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Close connection
               CMegatecUpsConfiguration newConfiguration;
               newConfiguration.initializeWith(newConfigurationData);

               // If port has changed, destroy and recreate connection
               bool needToReconnect = !connectionsAreEqual(m_configuration, newConfiguration);
               
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
               YADOMS_LOG(error) << "No answer received, try to reconnect in a moment...";
               errorProcess(context);
               break;
            }
         case kUpsStatusRequestDelay:
            {
               // Ask for status
               send(buildGetStatusCmd(), true);
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

void CMegatecUps::send(const shared::communication::CByteBuffer& buffer, bool needAnswer)
{
   if (!m_port)
      return;

   m_logger.logSent(buffer);
   m_port->send(buffer);
   if (needAnswer)
      m_waitForAnswerTimer->start();
}

void CMegatecUps::onCommand(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
{
   if (!m_port)
      YADOMS_LOG(warning) << "Command not send (UPS is not ready) : " << command;

   m_upsShutdown.set(command);
   send(m_upsShutdown.isOn() ? buildShtudownCmd() : buildCancelShtudownCmd());
}

void CMegatecUps::processConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context)
{
   YADOMS_LOG(debug) << "UPS is now connected";
   context->recordPluginEvent(yApi::IYadomsApi::kInfo, "UPS is now connected");

   try
   {
      // Ask for UPS informations
      send(buildGetInformationCmd(), true);
   }
   catch (CProtocolException& e)
   {
      YADOMS_LOG(error) << "Error resetting UPS : " << e.what();

      // Stop the communication, and try later
      errorProcess(context);
   }
   catch (shared::communication::CPortException& e)
   {
      YADOMS_LOG(error) << "Error connecting to UPS : " << e.what();
      // Disconnection will be notified, we just have to wait...
   }
}

void CMegatecUps::errorProcess(boost::shared_ptr<yApi::IYadomsApi> context)
{
   destroyConnection();
   context->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CMegatecUps::processUnConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context)
{
   YADOMS_LOG(debug) << "UPS connection was lost";
   context->recordPluginEvent(yApi::IYadomsApi::kInfo, "UPS connection was lost");

   destroyConnection();
}

void CMegatecUps::processDataReceived(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& message)
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
               send(buildGetRatingInformationCmd(), true);
            }
            else if (tokenCount == 4)
            {
               processReceivedRatingInformation(tokens);

               // Now ask for status
               send(buildGetStatusCmd(), true);
            }
            else
               throw CProtocolException("invalid information message");
            break;
         }
      default:
         throw CProtocolException((boost::format("invalid start byte : %1%") % message[0]).str());
      }

      // Message was recognized, stop timeout
      m_waitForAnswerTimer->stop();
   }
   catch (CProtocolException& e)
   {
      YADOMS_LOG(warning) << "Unable to decode received message : " << e.what();
   }
}

const shared::communication::CByteBuffer CMegatecUps::buildGetInformationCmd() const
{
   std::ostringstream cmd;
   cmd << 'I' << MEGATEC_EOF;
   return shared::communication::CByteBuffer((unsigned char*) cmd.str().c_str(), cmd.str().size());
}

const shared::communication::CByteBuffer CMegatecUps::buildGetRatingInformationCmd() const
{
   std::ostringstream cmd;
   cmd << 'F' << MEGATEC_EOF;
   return shared::communication::CByteBuffer((unsigned char*) cmd.str().c_str(), cmd.str().size());
}

const shared::communication::CByteBuffer CMegatecUps::buildGetStatusCmd() const
{
   std::ostringstream cmd;
   cmd << "Q1" << MEGATEC_EOF;
   return shared::communication::CByteBuffer((unsigned char*) cmd.str().c_str(), cmd.str().size());
}

const shared::communication::CByteBuffer CMegatecUps::buildToggleBeepCmd() const
{
   std::ostringstream cmd;
   cmd << 'Q' << MEGATEC_EOF;
   return shared::communication::CByteBuffer((unsigned char*) cmd.str().c_str(), cmd.str().size());
}

const shared::communication::CByteBuffer CMegatecUps::buildShtudownCmd() const
{
   std::ostringstream cmd;
   cmd.imbue(ProtocolFloatFormatingLocale);
   cmd << 'S' << std::setprecision(1) << m_configuration.outuputShutdownDelay();
   cmd << 'R' << std::setw(5) << std::setfill('0') << m_configuration.outuputRestoreDelay();
   cmd << MEGATEC_EOF;

   return shared::communication::CByteBuffer((unsigned char*) cmd.str().c_str(), cmd.str().size());
}

const shared::communication::CByteBuffer CMegatecUps::buildCancelShtudownCmd() const
{
   std::ostringstream cmd;
   cmd << 'C' << MEGATEC_EOF;
   return shared::communication::CByteBuffer((unsigned char*) cmd.str().c_str(), cmd.str().size());
}

void CMegatecUps::processReceivedStatus(boost::shared_ptr<yApi::IYadomsApi> context, const boost::tokenizer<boost::char_separator<char> >& tokens)
{
   boost::tokenizer<boost::char_separator<char>>::const_iterator itToken = tokens.begin();
   m_inputVoltage.set      (upsStr2Double(*itToken)); ++itToken;
   m_inputfaultVoltage.set (upsStr2Double(*itToken)); ++itToken;
   m_outputVoltage.set     (upsStr2Double(*itToken)); ++itToken;
   m_outputCurrent.set     (upsStr2Double(*itToken)); ++itToken;
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

   YADOMS_LOG(debug) << "UPS rating informations : inputVoltage=" << m_inputVoltage.get() <<
      ", inputfaultVoltage="  << m_inputfaultVoltage.get() <<
      ", outputVoltage="      << m_outputVoltage.get() <<
      ", outputCurrent="      << m_outputCurrent.get() <<
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
      send(buildToggleBeepCmd());

   // Process AC power status
   processAcPowerStatus(context, utilityFail, batteryLow);

   // Wait for next status request
   m_upsStatusRequestTimer->start();
}

void CMegatecUps::processReceivedInformation(boost::shared_ptr<yApi::IYadomsApi> context, const boost::tokenizer<boost::char_separator<char> >& tokens)
{
   boost::tokenizer<boost::char_separator<char>>::const_iterator itToken = tokens.begin();
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
   boost::tokenizer<boost::char_separator<char>>::const_iterator itToken = tokens.begin();
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

void CMegatecUps::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& model) const
{
   if (!context->deviceExists(DeviceName))
   {
      context->declareDevice(DeviceName, model);

      context->declareKeyword(DeviceName, m_inputVoltage);
      context->declareKeyword(DeviceName, m_inputfaultVoltage);
      context->declareKeyword(DeviceName, m_outputVoltage);
      context->declareKeyword(DeviceName, m_outputCurrent);
      context->declareKeyword(DeviceName, m_inputFrequency);
      context->declareKeyword(DeviceName, m_batteryVoltage);
      context->declareKeyword(DeviceName, m_temperature);
      context->declareKeyword(DeviceName, m_acPowerHistorizer);
      context->declareKeyword(DeviceName, m_upsShutdown);
   }
}

void CMegatecUps::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(DeviceName, m_inputVoltage);
   context->historizeData(DeviceName, m_inputfaultVoltage);
   context->historizeData(DeviceName, m_outputVoltage);
   context->historizeData(DeviceName, m_outputCurrent);
   context->historizeData(DeviceName, m_inputFrequency);
   context->historizeData(DeviceName, m_batteryVoltage);
   context->historizeData(DeviceName, m_temperature);
}

void CMegatecUps::processAcPowerStatus(boost::shared_ptr<yApi::IYadomsApi> context, bool acPowerActive, bool lowBatteryFlag)
{
   switch(m_configuration.powerFailureManagement())
   {
   case IMegatecUpsConfiguration::kNotifyImmediately:
      {
         if (acPowerActive != m_acPowerActive)
            notifyPowerState(context, acPowerActive);
         m_acPowerActive = acPowerActive;
         break;
      }
   case IMegatecUpsConfiguration::kFilter:
      {
         if (acPowerActive != m_acPowerActive && !acPowerActive)
            m_filterTimer->start(boost::posix_time::seconds(m_configuration.powerFailureFilterDelay()));
         m_acPowerActive = acPowerActive;
         break;
      }
   case IMegatecUpsConfiguration::kLowBattery:
      {
         // No mater if AC power is OK or not
         if (lowBatteryFlag != m_lowBatteryFlag)
            notifyPowerState(context, !lowBatteryFlag);
         m_lowBatteryFlag = lowBatteryFlag;
         break;
      }
   case IMegatecUpsConfiguration::kRemainingBattery:
      {
         // No mater if AC power is OK or not
         bool oldLowBatteryByLevelFlag = m_lowBatteryByLevelFlag;
         getLowBatteryByLevelFlagState(m_lowBatteryByLevelFlag);
         if (m_lowBatteryByLevelFlag != oldLowBatteryByLevelFlag)
            notifyPowerState(context, m_lowBatteryByLevelFlag);
         break;
      }
   default:
      {
         BOOST_ASSERT_MSG(false, "Unsupported powerFailureManagement value");
         break;
      }
   }
}

void CMegatecUps::notifyPowerState(boost::shared_ptr<yApi::IYadomsApi> context, bool powerState)
{
   m_acPowerHistorizer.set(powerState);
   context->historizeData(DeviceName, m_acPowerHistorizer);
}

void CMegatecUps::getLowBatteryByLevelFlagState(bool& flag) const
{
   if (getBatteryLevel() < m_configuration.powerFailureRemainingBatteryThreshold())
      flag = true;   // Battery becomes low
   else if (getBatteryLevel() > m_configuration.powerFailureRemainingBatteryThreshold())
      flag = false;  // Battery exited low state
   // else (batteryLevel == threshold) ==> Flag not changed (make a kind of hysteresis)
}

unsigned int CMegatecUps::getBatteryLevel() const
{
   if (m_batteryNominalVoltage == 0.0)
      return 100; // Nominal voltage is not initialized

   //TODO convertir en %
   return m_batteryVoltage;
}