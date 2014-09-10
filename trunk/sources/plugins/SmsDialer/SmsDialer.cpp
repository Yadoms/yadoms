#include "stdafx.h"
#include "SmsDialer.h"
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/commands/Switch.h>
#include <shared/plugin/yadomsApi/commands/Message.h>
#include "SmsDialerFactory.h"
#include "PhoneException.hpp"
#include "Sms.h"


IMPLEMENT_PLUGIN(CSmsDialer)


CSmsDialer::CSmsDialer()
{
}

CSmsDialer::~CSmsDialer()
{
}

// Event IDs
enum
{
   kEvtTimerTryToConnectToPhone = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from yApi::IYadomsApi::kPluginFirstEventId
   kEvtTimerCheckForIncommingSms
};

void CSmsDialer::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

      // Create the phone instance
      m_phone = CSmsDialerFactory::constructPhone(m_configuration);

      // the main loop
      YADOMS_LOG(debug) << "CSmsDialer is running...";

      // Timer used to periodically try to connect to phone
      m_connectionTimer = context->getEventHandler().createTimer(kEvtTimerTryToConnectToPhone, shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(1));
      m_connectionTimer->stop();

      // Timer used to periodically check for incoming SMS
      m_incommingSmsPollTimer = context->getEventHandler().createTimer(kEvtTimerCheckForIncommingSms, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(30));
      m_incommingSmsPollTimer->stop();

      m_phone->connect();

      while(1)
      {
         if (!m_phone->isConnected())
         {
            processNotConnectedState(context);
         }
         else
         {
            processConnectedState(context);
         }
      };
   }
   catch (boost::thread_interrupted&)
   {
   }
}

void CSmsDialer::processNotConnectedState(boost::shared_ptr<yApi::IYadomsApi> context)
{
   YADOMS_LOG(info) << "Phone is not connected"  << std::endl;
   context->recordPluginEvent(yApi::IYadomsApi::kInfo, "Phone is not connected");

   m_incommingSmsPollTimer->stop();
   m_connectionTimer->start();

   while (!m_phone->isConnected())
   {
      try
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case yApi::IYadomsApi::kEventDeviceCommand:
            {
               // Ignore all commands
               YADOMS_LOG(warning) << "Not able to process received the command (phone not connected)";
               break;
            }
         case yApi::IYadomsApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               std::string newConfiguration = context->getEventHandler().getEventData<std::string>();
               YADOMS_LOG(debug) << "configuration was updated...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEvtUpdateConfiguration shouldn't be generated

               // Destroy current phone instance to update configuration
               m_phone.reset();

               // Update configuration
               m_configuration.initializeWith(newConfiguration);

               // Create new phone
               m_phone = CSmsDialerFactory::constructPhone(m_configuration);

               break;
            }
         case kEvtTimerTryToConnectToPhone:
            {
               m_phone->connect();
               break;
            }
         case kEvtTimerCheckForIncommingSms:
            {
               // Ignore this event
               break;
            }
         default:
            {
               YADOMS_LOG(error) << "Unknown message id";
               break;
            }
         }
      }
      catch (CPhoneException& e)
      {
         YADOMS_LOG(error) << "Phone critical error : " << e.what();
      }
   }
}

void CSmsDialer::processConnectedState(boost::shared_ptr<yApi::IYadomsApi> context)
{
   YADOMS_LOG(info) << "Phone is connected"  << std::endl;
   context->recordPluginEvent(yApi::IYadomsApi::kInfo, "Phone is connected");

   m_connectionTimer->stop();

   // First, declare device to Yadoms
   declareDevice(context);

   // Next, unlock phone
   try
   { 
      if (m_configuration.hasPINCode())
         m_phone->unlock(m_configuration.getPhonePIN());

      // Check for incoming SMS
      processIncommingSMS(context);
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << "Phone critical error : " << e.what();
   }

   // And start timer for next incoming SMS check
   m_incommingSmsPollTimer->start();

   while (m_phone->isConnected())
   {
      try
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case yApi::IYadomsApi::kEventDeviceCommand:
            {
               // Command received
               boost::shared_ptr<yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<yApi::IDeviceCommand> >();
               YADOMS_LOG(debug) << "Command received :" << command->toString();

               const std::string keyword = command->getKeyword();
               if (keyword == "power")
                  onPowerPhoneRequest(context, command->getBody());
               else if (keyword == "sms")
                  onSendSmsRequest(context, command->getBody());
               else
                  YADOMS_LOG(error) << "Unsupported command received : " << command->toString();

               break;
            }
         case yApi::IYadomsApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               std::string newConfiguration = context->getEventHandler().getEventData<std::string>();
               YADOMS_LOG(debug) << "configuration was updated...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Close connection with current phone
               m_phone.reset();

               // Update configuration
               m_configuration.initializeWith(newConfiguration);

               // Create new phone
               m_phone = CSmsDialerFactory::constructPhone(m_configuration);

               break;
            }
         case kEvtTimerTryToConnectToPhone:
            {
               // Ignore this event
               break;
            }
         case kEvtTimerCheckForIncommingSms:
            {
               processIncommingSMS(context);
               break;
            }
         default:
            {
               YADOMS_LOG(error) << "Unknown message id";
               break;
            }
         }
      }
      catch (CPhoneException& e)
      {
         YADOMS_LOG(error) << "Phone critical error : " << e.what();
      }
   }
}

void CSmsDialer::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_phone->getUniqueId(), m_phone->getUniqueId(), shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_phone->getUniqueId(), "power", yApi::CStandardCapacities::Switch );
   context->declareKeyword(m_phone->getUniqueId(), "sms"  , yApi::CStandardCapacities::Message);
}

void CSmsDialer::onPowerPhoneRequest(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& powerRequest)
{
   yApi::commands::CSwitch cmd(powerRequest);
   BOOST_ASSERT_MSG(cmd.getState() == yApi::commands::CSwitch::EState::kOff || cmd.getState() == yApi::commands::CSwitch::EState::kOn, "Invalid power request");

   try
   {
      m_phone->powerOn(cmd.getState() == yApi::commands::CSwitch::EState::kOn);
      notifyPhonePowerState(context, m_phone->isOn());
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << "Fail to power ON/OFF the phone : " << e.what();
   }
}

void CSmsDialer::onSendSmsRequest(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& sendSmsRequest)
{
   try
   {
      yApi::commands::CMessage msg(sendSmsRequest);
      boost::shared_ptr<ISms> sms(new CSms(msg.to(), msg.body()));
      m_phone->send(sms);
      notifyAck(true);
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Invalid SMS sending request \"" << sendSmsRequest << "\" : " << e.what();
      BOOST_ASSERT_MSG(false, "Invalid SMS sending request");
      notifyAck(false);
      return;
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << "Error sending SMS : " << e.what();
      notifyAck(false);
   }
}

void CSmsDialer::processIncommingSMS(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Check if incoming SMS
   boost::shared_ptr<std::vector<boost::shared_ptr<ISms> > > incommingSms = m_phone->getIncomingSMS();
   if (incommingSms)
   {
      for (std::vector<boost::shared_ptr<ISms> >::const_iterator it = incommingSms->begin() ; it != incommingSms->end() ; ++it)
      {
         YADOMS_LOG(info) << "SMS received";
         YADOMS_LOG(debug) << "SMS received from " << (*it)->getNumber() << " : " << (*it)->getContent();

         // Send SMS to Yadoms
         notifySmsReception(context, *it);
      }
   }
}

void CSmsDialer::notifyAck(bool ok) const
{
   //TODO
}

void CSmsDialer::notifyPhonePowerState(boost::shared_ptr<yApi::IYadomsApi> context, bool on) const
{
   context->historizeData(m_phone->getUniqueId(), "power", std::string(on ? "on" : "off"));
}

void CSmsDialer::notifySmsReception(boost::shared_ptr<yApi::IYadomsApi> context, const boost::shared_ptr<ISms>& sms) const
{
   shared::CDataContainer smsContent;
   smsContent.set("from", sms->getNumber());
   smsContent.set("to", shared::CStringExtension::EmptyString);
   smsContent.set("body", sms->getContent());
   context->historizeData(m_phone->getUniqueId(), "sms", smsContent.serialize());
}
