#include "stdafx.h"
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
   :m_messageKeyword(boost::make_shared<yApi::historization::CMessage>("message",
      m_phoneFieldName,
      yApi::EKeywordAccessMode::kGetSet)),
   m_powerKeyword(boost::make_shared<yApi::historization::CSwitch>("power")),
   m_keywords({ m_messageKeyword, m_powerKeyword })
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

void CSmsDialer::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   // the main loop
   std::cout << "CSmsDialer is running..." << std::endl;

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

   std::cout << "Stop requested" << std::endl;
   api->setPluginState(yApi::historization::EPluginState::kStopped);
}

bool CSmsDialer::processNotConnectedState(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "Phone is not connected" << std::endl << std::endl;
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
               std::cout << "Not able to process received the command (phone not connected)" << std::endl;
               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               std::string newConfiguration = api->getEventHandler().getEventData<std::string>();
               std::cout << "configuration was updated..." << std::endl;
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
               std::cerr << "Unknown message id" << std::endl;
               break;
            }
         }
      }
      catch (CPhoneException& e)
      {
         std::cerr << "Phone critical error : " << e.what() << std::endl;
      }
   }

   return false;
}

bool CSmsDialer::processConnectedState(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "Phone is connected" << std::endl;
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
      std::cerr << "Phone critical error : " << e.what() << std::endl;
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
               boost::shared_ptr<const yApi::IDeviceCommand> command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
               std::cout << "Command received :" << yApi::IDeviceCommand::toString(command);

               if (boost::iequals(command->getKeyword(), m_powerKeyword->getKeyword()))
                  onPowerPhoneRequest(api, command->getBody());
               else if (boost::iequals(command->getKeyword(), m_messageKeyword->getKeyword()))
                  onSendSmsRequest(api, command->getBody());
               else
                  std::cout << "Received command for unknown keyword from Yadoms : " << yApi::IDeviceCommand::toString(command) << std::endl;

               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               std::string newConfiguration = api->getEventHandler().getEventData<std::string>();
               std::cout << "Update configuration..." << std::endl;
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
               std::cerr << "Unknown message id" << std::endl;
               break;
            }
         }
      }
      catch (CPhoneException& e)
      {
         std::cerr << "Phone critical error : " << e.what() << std::endl;
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
      std::cerr << "Fail to power ON/OFF the phone : " << e.what() << std::endl;
   }
}

void CSmsDialer::onSendSmsRequest(boost::shared_ptr<yApi::IYPluginApi> api,
   const std::string& sendSmsRequest) const
{
   try
   {
      m_messageKeyword->setCommand(sendSmsRequest);
      boost::shared_ptr<ISms> sms(new CSms(getRecipientPhone(api, m_messageKeyword->to()), m_messageKeyword->body()));
      m_phone->send(sms);
      notifyAck(true);
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      std::cerr << "Invalid SMS sending request \"" << sendSmsRequest << "\" : " << e.what() << std::endl;
      notifyAck(false);
      return;
   }
   catch (CPhoneException& e)
   {
      std::cerr << "Error sending SMS : " << e.what() << std::endl;
      notifyAck(false);
   }
}

void CSmsDialer::processIncommingSMS(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   // Check if incoming SMS
   auto incommingSms = m_phone->getIncomingSMS();
   if (incommingSms)
   {
      for (auto it = incommingSms->begin() ; it != incommingSms->end() ; ++it)
      {
         std::cout << "SMS received" << std::endl;
         std::cout << "SMS received from " << (*it)->getNumber() << " : " << (*it)->getContent() << std::endl;

         // Send SMS to Yadoms
         try
         {
            m_messageKeyword->set(findRecipientByPhone(api, (*it)->getNumber()), 0, (*it)->getContent());
            api->historizeData(m_device, m_messageKeyword);
         }
         catch (shared::exception::CInvalidParameter& e)
         {
            std::cerr << "Phone number is unknown " << e.what() << std::endl;
         }
      }
   }
}

void CSmsDialer::notifyAck(bool ok)
{
   // Command acknowledges are not actually supported (see https://trello.com/c/Yd8N8v3J), just log
   if (ok)
      std::cout << "SMS successfully sent" << std::endl;
   else
      std::cout << "Error sending SMS" << std::endl;
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
      std::cout << "Several recipients with phone number " << phoneNumber << " were found in database, select the first one, Id=" << recipients[0] << std::endl;

   return recipients[0];
}
