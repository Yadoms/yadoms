#include "stdafx.h"
#include <shared/Log.h>
#include "SmsDialer.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/exception/EmptyResult.hpp>
#include <shared/plugin/yPluginApi/historization/Message.h>
#include "SmsDialerFactory.h"
#include "PhoneException.hpp"
#include "Sms.h"


IMPLEMENT_PLUGIN(CSmsDialer)


const std::string CSmsDialer::m_phoneFieldName("mobile");


CSmsDialer::CSmsDialer()
   : m_messageKeyword(boost::make_shared<yApi::historization::CMessage>("message",
                                                                        m_phoneFieldName,
                                                                        yApi::EKeywordAccessMode::kGetSet)),
     m_powerKeyword(boost::make_shared<yApi::historization::CSwitch>("power")),
     m_keywords({m_messageKeyword, m_powerKeyword})
{
}

CSmsDialer::~CSmsDialer()
{
}

// Event IDs
enum
{
   kEvtTimerTryToConnectToPhone = yApi::IYPluginApi::kPluginFirstEventId, // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kEvtTimerCheckForIncommingSms
};

void CSmsDialer::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   // the main loop
   YADOMS_LOG(information) << "CSmsDialer is running..." ;

   // Create the phone instance
   m_phone = CSmsDialerFactory::constructPhone(m_configuration);
   m_phone->connect();
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   // Timer used to periodically try to connect to phone
   m_connectionTimer = api->getEventHandler().createTimer(kEvtTimerTryToConnectToPhone,
                                                          shared::event::CEventTimer::kPeriodic,
                                                          boost::posix_time::minutes(1));
   m_connectionTimer->stop();

   // Timer used to periodically check for incoming SMS
   m_incommingSmsPollTimer = api->getEventHandler().createTimer(kEvtTimerCheckForIncommingSms,
                                                                shared::event::CEventTimer::kPeriodic,
                                                                boost::posix_time::seconds(30));
   m_incommingSmsPollTimer->stop();

   auto stopIsRequested = false;
   while (!stopIsRequested)
   {
      if (!m_phone->isConnected())
      {
         stopIsRequested = processNotConnectedState(api);
      }
      else
      {
         stopIsRequested = processConnectedState(api);
      }
   }

   YADOMS_LOG(information) << "Stop requested" ;
   api->setPluginState(yApi::historization::EPluginState::kStopped);
}

bool CSmsDialer::processNotConnectedState(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Phone is not connected";
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connectionFailed");

   m_incommingSmsPollTimer->stop();
   m_connectionTimer->start();

   while (!m_phone->isConnected())
   {
      try
      {
         // Wait for an event
         switch (api->getEventHandler().waitForEvents())
         {
         case yApi::IYPluginApi::kEventStopRequested:
            {
               return true;
            }
         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Ignore all commands
               YADOMS_LOG(information) << "Not able to process received the command (phone not connected)" ;
               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               auto newConfiguration = api->getEventHandler().getEventData<std::string>();
               YADOMS_LOG(information) << "configuration was updated..." ;
               BOOST_ASSERT(!newConfiguration.empty()); // newConfigurationValues shouldn't be empty, or kEvtUpdateConfiguration shouldn't be generated

               // Destroy current phone instance to update configuration
               m_phone.reset();

               // Update configuration
               m_configuration.initializeWith(shared::CDataContainer(newConfiguration));

               // Create new phone
               m_phone = CSmsDialerFactory::constructPhone(m_configuration);
               m_phone->connect();
               api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

               break;
            }
         case kEvtTimerTryToConnectToPhone:
            {
               if (m_phone->connect())
                  api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");
               else
                  api->setPluginState(yApi::historization::EPluginState::kError, "connectionFailed");

               break;
            }
         case kEvtTimerCheckForIncommingSms:
            {
               // Ignore this event
               break;
            }
         default:
            {
               YADOMS_LOG(warning) << "Unknown message id " << api->getEventHandler().getEventId();
               break;
            }
         }
      }
      catch (CPhoneException& e)
      {
         YADOMS_LOG(error) << "Phone critical error : " << e.what() ;
      }
   }

   return false;
}

bool CSmsDialer::processConnectedState(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Phone is connected" ;
   api->setPluginState(yApi::historization::EPluginState::kCustom, "phoneConnected");

   m_connectionTimer->stop();

   // First, declare device to Yadoms
   m_device = m_phone->getUniqueId();
   declareDevice(api);

   // Next, unlock phone
   try
   {
      if (m_configuration.hasPINCode())
         m_phone->unlock(m_configuration.getPhonePIN());

      // Check for incoming SMS
      processIncommingSMS(api);
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << "Phone critical error : " << e.what() ;
   }

   // And start timer for next incoming SMS check
   m_incommingSmsPollTimer->start();

   api->setPluginState(yApi::historization::EPluginState::kRunning);

   while (m_phone->isConnected())
   {
      try
      {
         // Wait for an event
         switch (api->getEventHandler().waitForEvents())
         {
         case yApi::IYPluginApi::kEventStopRequested:
            {
               return true;
            }
         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command received
               auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
               YADOMS_LOG(information) << "Command received :" << yApi::IDeviceCommand::toString(command);

               try
               {
                  if (boost::iequals(command->getKeyword(), m_powerKeyword->getKeyword()))
                     onPowerPhoneRequest(api, command->getBody());
                  else if (boost::iequals(command->getKeyword(), m_messageKeyword->getKeyword()))
                     onSendSmsRequest(api, command->getBody());
                  else
                  YADOMS_LOG(information) << "Received command for unknown keyword from Yadoms : " << yApi::IDeviceCommand::toString(command) ;
               }
               catch (std::exception& e)
               {
                  YADOMS_LOG(error) << "Fail to send command " << yApi::IDeviceCommand::toString(command) << ", error : " << e.what();
               }

               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               auto newConfiguration = api->getEventHandler().getEventData<std::string>();
               YADOMS_LOG(information) << "Update configuration..." ;
               BOOST_ASSERT(!newConfiguration.empty()); // newConfigurationValues shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Close connection with current phone
               m_phone.reset();

               // Update configuration
               m_configuration.initializeWith(shared::CDataContainer(newConfiguration));

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
               processIncommingSMS(api);
               break;
            }
         default:
            {
               YADOMS_LOG(warning) << "Unknown message id " << api->getEventHandler().getEventId();
               break;
            }
         }
      }
      catch (CPhoneException& e)
      {
         YADOMS_LOG(error) << "Phone critical error : " << e.what() ;
      }
   }

   return false;
}

void CSmsDialer::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (api->deviceExists(m_device))
      return;

   api->declareDevice(m_device, m_device, m_keywords);
}

void CSmsDialer::onPowerPhoneRequest(boost::shared_ptr<yApi::IYPluginApi> api,
                                     const std::string& powerRequest) const
{
   try
   {
      m_powerKeyword->setCommand(powerRequest);
      m_phone->powerOn(m_powerKeyword->get());

      // Update keyword and historize new state
      m_powerKeyword->set(m_phone->isOn());
      api->historizeData(m_device, m_powerKeyword);
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << "Fail to power ON/OFF the phone : " << e.what() ;
   }
}

void CSmsDialer::onSendSmsRequest(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const std::string& sendSmsRequest) const
{
   try
   {
      m_messageKeyword->setCommand(sendSmsRequest);
      auto sms = boost::make_shared<CSms>(getRecipientPhone(api, m_messageKeyword->to()), m_messageKeyword->body());
      m_phone->send(sms);
      notifyAck(true);
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Invalid SMS sending request \"" << sendSmsRequest << "\" : " << e.what() ;
      notifyAck(false);
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << "Error sending SMS : " << e.what() ;
      notifyAck(false);
   }
}

void CSmsDialer::processIncommingSMS(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   // Check if incoming SMS
   auto incommingSms = m_phone->getIncomingSMS();
   if (incommingSms)
   {
      for (auto it = incommingSms->begin(); it != incommingSms->end(); ++it)
      {
         YADOMS_LOG(information) << "SMS received" ;
         YADOMS_LOG(information) << "SMS received from " << (*it)->getNumber() << " : " << (*it)->getContent() ;

         // Send SMS to Yadoms
         try
         {
            m_messageKeyword->set(findRecipientByPhone(api, (*it)->getNumber()), 0, (*it)->getContent());
            api->historizeData(m_device, m_messageKeyword);
         }
         catch (shared::exception::CInvalidParameter& e)
         {
            YADOMS_LOG(error) << "Phone number is unknown " << e.what() ;
         }
      }
   }
}

void CSmsDialer::notifyAck(bool ok)
{
   // Command acknowledges are not actually supported (see https://trello.com/c/Yd8N8v3J), just log
   if (ok)
      YADOMS_LOG(information) << "SMS successfully sent" ;
   else
      YADOMS_LOG(information) << "Error sending SMS" ;
}

std::string CSmsDialer::getRecipientPhone(boost::shared_ptr<yApi::IYPluginApi> api, int recipientId)
{
   try
   {
      return api->getRecipientValue(recipientId, m_phoneFieldName);
   }
   catch (shared::exception::CEmptyResult& e)
   {
      throw shared::exception::CInvalidParameter(std::string("Recipient not found, or phone field not found for recipient : ") + e.what());
   }
}

int CSmsDialer::findRecipientByPhone(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& phoneNumber)
{
   auto recipients = api->findRecipientsFromField(m_phoneFieldName, phoneNumber);

   if (recipients.empty())
      throw shared::exception::CInvalidParameter((boost::format("Recipient with phone number %1% not found in database") % phoneNumber).str());

   if (recipients.size() > 1)
      YADOMS_LOG(information) << "Several recipients with phone number " << phoneNumber << " were found in database, select the first one, Id=" << recipients[0] ;

   return recipients[0];
}

