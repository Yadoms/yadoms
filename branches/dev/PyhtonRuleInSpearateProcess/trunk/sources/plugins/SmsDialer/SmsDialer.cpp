#include "stdafx.h"
#include "SmsDialer.h"
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/historization/Dimmable.h>
#include <shared/plugin/yPluginApi/historization/Message.h>
#include "SmsDialerFactory.h"
#include "PhoneException.hpp"
#include "Sms.h"


IMPLEMENT_PLUGIN(CSmsDialer)


const std::string CSmsDialer::m_phoneFieldName("mobile");


CSmsDialer::CSmsDialer()
   :m_messageKeyword("message", m_phoneFieldName, yApi::EKeywordAccessMode::kGetSet), m_powerKeyword("power")
{
}

CSmsDialer::~CSmsDialer()
{
}

// Event IDs
enum
{
   kEvtTimerTryToConnectToPhone = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kEvtTimerCheckForIncommingSms
};

void CSmsDialer::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
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
      }
   }
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(information) << "Thread is stopping...";
   }
}

void CSmsDialer::processNotConnectedState(boost::shared_ptr<yApi::IYPluginApi> context)
{
   YADOMS_LOG(information) << "Phone is not connected"  << std::endl;
   context->recordPluginEvent(yApi::IYPluginApi::kInfo, "Phone is not connected");

   m_incommingSmsPollTimer->stop();
   m_connectionTimer->start();

   while (!m_phone->isConnected())
   {
      try
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Ignore all commands
               YADOMS_LOG(warning) << "Not able to process received the command (phone not connected)";
               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
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

void CSmsDialer::processConnectedState(boost::shared_ptr<yApi::IYPluginApi> context)
{
   YADOMS_LOG(information) << "Phone is connected"  << std::endl;
   context->recordPluginEvent(yApi::IYPluginApi::kInfo, "Phone is connected");

   m_connectionTimer->stop();

   // First, declare device to Yadoms
   m_device = m_phone->getUniqueId();
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
         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command received
               boost::shared_ptr<const yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
               YADOMS_LOG(debug) << "Command received :" << command->toString();

               if (boost::iequals(command->getKeyword(), m_powerKeyword.getKeyword()))
                  onPowerPhoneRequest(context, command->getBody());
               else if (boost::iequals(command->getKeyword(), m_messageKeyword.getKeyword()))
                  onSendSmsRequest(context, command->getBody());
               else
                  YADOMS_LOG(warning) << "Received command for unknown keyword from Yadoms : " << command->toString();

               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
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

void CSmsDialer::declareDevice(boost::shared_ptr<yApi::IYPluginApi> context)
{
   if (context->deviceExists(m_device))
      return;

   // Declare the device
   context->declareDevice(m_device, m_device);

   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_device, m_powerKeyword);
   context->declareKeyword(m_device, m_messageKeyword);
}

void CSmsDialer::onPowerPhoneRequest(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& powerRequest)
{
   try
   {
      m_powerKeyword.set(powerRequest);
      m_phone->powerOn(m_powerKeyword.get());

      // Update keyword and historize new state
      m_powerKeyword.set(m_phone->isOn());
      context->historizeData(m_device, m_powerKeyword);
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << "Fail to power ON/OFF the phone : " << e.what();
   }
}

void CSmsDialer::onSendSmsRequest(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& sendSmsRequest)
{
   try
   {
      m_messageKeyword.set(sendSmsRequest);
      boost::shared_ptr<ISms> sms(new CSms(getRecipientPhone(context, m_messageKeyword.to()), m_messageKeyword.body()));
      m_phone->send(sms);
      notifyAck(true);
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Invalid SMS sending request \"" << sendSmsRequest << "\" : " << e.what();
      notifyAck(false);
      return;
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << "Error sending SMS : " << e.what();
      notifyAck(false);
   }
}

void CSmsDialer::processIncommingSMS(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Check if incoming SMS
   boost::shared_ptr<std::vector<boost::shared_ptr<ISms> > > incommingSms = m_phone->getIncomingSMS();
   if (incommingSms)
   {
      for (std::vector<boost::shared_ptr<ISms> >::const_iterator it = incommingSms->begin() ; it != incommingSms->end() ; ++it)
      {
         YADOMS_LOG(information) << "SMS received";
         YADOMS_LOG(debug) << "SMS received from " << (*it)->getNumber() << " : " << (*it)->getContent();

         // Send SMS to Yadoms
         try
         {
            m_messageKeyword.set(findRecipientByPhone(context, (*it)->getNumber()), 0, (*it)->getContent());
            context->historizeData(m_device, m_messageKeyword);
         }
         catch (shared::exception::CInvalidParameter& e)
         {
            YADOMS_LOG(error) << "Phone number is unknown " << e.what();
         }
      }
   }
}

void CSmsDialer::notifyAck(bool ok) const
{
   // Command acknowledges are not actually supported (see https://trello.com/c/Yd8N8v3J), just log
   if (ok)
      YADOMS_LOG(information) << "SMS successfully sent";
   else
      YADOMS_LOG(warning) << "Error sending SMS";
}

std::string CSmsDialer::getRecipientPhone(boost::shared_ptr<yApi::IYPluginApi> context, int recipientId) const
{
   try
   {
      return context->getRecipientValue(recipientId, m_phoneFieldName);
   }
   catch (shared::exception::CEmptyResult& e)
   {
      throw shared::exception::CInvalidParameter(std::string("Recipient not found, or phone field not found for recipient : ") + e.what());
   }
}

int CSmsDialer::findRecipientByPhone(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& phoneNumber) const
{
   std::vector<int> recipients = context->findRecipientsFromField(m_phoneFieldName, phoneNumber);

   if (recipients.empty())
      throw shared::exception::CInvalidParameter((boost::format("Recipient with phone number %1% not found in database") % phoneNumber).str());

   if (recipients.size() > 1)
      YADOMS_LOG(warning) << "Several recipients with phone number " << phoneNumber << " were found in database, select the first one, Id=" << recipients[0];

   return recipients[0];
}
