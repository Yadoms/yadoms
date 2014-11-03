#include "stdafx.h"
#include "SmsDialer.h"
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/historization/Dimmable.h>
#include <shared/plugin/yadomsApi/historization/Message.h>
#include "SmsDialerFactory.h"
#include "PhoneException.hpp"
#include "Sms.h"


IMPLEMENT_PLUGIN(CSmsDialer)


CSmsDialer::CSmsDialer()
   :m_messageKeyword("message", yApi::EKeywordAccessMode::kGetSet), m_powerKeyword("power")
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
         case yApi::IYadomsApi::kEventDeviceCommand:
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
   if (context->deviceExists(m_device))
      return;

   // Declare the device
   context->declareDevice(m_device, m_device);

   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_device, m_powerKeyword);
   context->declareKeyword(m_device, m_messageKeyword);
}

void CSmsDialer::onPowerPhoneRequest(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& powerRequest)
{
   try
   {
      m_powerKeyword.set(powerRequest);
      m_phone->powerOn(m_powerKeyword.isOn());

      // Update keyword and historize new state
      m_powerKeyword.set(m_phone->isOn());
      context->historizeData(m_device, m_powerKeyword);
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
      m_messageKeyword.set(sendSmsRequest);
      boost::shared_ptr<ISms> sms(new CSms(m_messageKeyword.to(), m_messageKeyword.body()));
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
         m_messageKeyword.set((*it)->getNumber(), shared::CStringExtension::EmptyString, (*it)->getContent());
         context->historizeData(m_device, m_messageKeyword);
      }
   }
}

void CSmsDialer::notifyAck(bool ok) const
{
   //TODO
}
