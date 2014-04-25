#include "stdafx.h"
#include "SmsDialer.h"
#include <shared/Log.h>
#include <shared/xpl/XplService.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplHelper.h>
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
   kEvtXplMessage = shared::event::kUserFirstId,   // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtUpdateConfiguration,
   kEvtTimerCheckForIncommingSms
};

// XPL device ID
static const std::string& XplDeviceId("smsdial");


void CSmsDialer::doWork(int instanceUniqueId, const std::string& configuration, boost::asio::io_service * pluginIOService)
{
   try
   {
      YADOMS_LOG_CONFIGURE(Informations->getName());
      YADOMS_LOG(debug) << "CSmsDialer is starting...";

      // Load configuration values (provided by database)
      m_configuration.set(configuration);

      // Create the phone instance
      m_phone = CSmsDialerFactory::constructPhone(m_configuration);

      // Register to XPL service
      shared::xpl::CXplService xplService(
         XplDeviceId,
         shared::xpl::CXplHelper::toInstanceId(instanceUniqueId),
         pluginIOService,
         this,
         kEvtXplMessage);

      // the main loop
      YADOMS_LOG(debug) << "CSmsDialer is running...";

      //TODO à virer
      shared::xpl::CXplMessage temp;
      onXplMessageReceived(temp);

      //TODO gérer une machine d'état pour essayer de se connecter réglièrement, et traiter les envoi/réceptions que si connecté

      // Timer used to periodically check for incomming SMS
      createTimer(kEvtTimerCheckForIncommingSms, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(30));

      while(1)
      {
         // Wait for an event
         switch(waitForEvents(boost::date_time::pos_infin))
         {
         case kEvtXplMessage:
            {
               // Xpl message was received
               onXplMessageReceived(getEventData<shared::xpl::CXplMessage>());
               break;
            }
         case kEvtUpdateConfiguration:
            {
               // Configuration was updated
               std::string newConfiguration = getEventData<std::string>();
               YADOMS_LOG(debug) << "configuration was updated...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEvtUpdateConfiguration shouldn't be generated

               // Close connection with current phone
               m_phone.reset();

               // Update configuration
               m_configuration.set(newConfiguration);

               // Create new phone
               m_phone = CSmsDialerFactory::constructPhone(m_configuration);

               break;
            }
         case kEvtTimerCheckForIncommingSms:
            {
               processIncommingSMS();
               break;
            }
         default:
            {
               YADOMS_LOG(error) << "Unknown message id";
               break;
            }
         }
      };
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << "Phone critical error : " << e.what();
   }
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(info) << "CSmsDialer is stopping..."  << std::endl;
   }
}

void CSmsDialer::updateConfiguration(const std::string& configuration)
{
   // This function is called in a Yadoms thread context, so send a event to the CSmsDialer thread
   sendEvent<std::string>(kEvtUpdateConfiguration, configuration);
}

void CSmsDialer::onXplMessageReceived(const shared::xpl::CXplMessage& xplMessage)
{
   YADOMS_LOG(debug) << "XPL message event received :" << xplMessage.toString();

   // TODO traiter le message comme il faut !
   try
   {
      boost::shared_ptr<ISms> sms(new CSms("0609414394", "1 - Ceci est un suuuuuuuuuupeeeeeeer loooooooooonnnnnnnnng SMS envoyé par le plugin smsDialer de Yadoms, d'ailleurs, pour être bien sûr qu'il fait plusieurs fois cent soixante caractères, je le répète : Ceci est un suuuuuuuuuupeeeeeeer loooooooooonnnnnnnnng SMS envoyé par le plugin smsDialer de Yadoms. Et encore une fois : Ceci est un suuuuuuuuuupeeeeeeer loooooooooonnnnnnnnng SMS envoyé par le plugin smsDialer de Yadoms. Encore : Ceci est un suuuuuuuuuupeeeeeeer loooooooooonnnnnnnnng SMS envoyé par le plugin smsDialer de Yadoms :-)"));//Christelle
      m_phone->send(sms);
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << "Error sending SMS : " << e.what();
   }
}

void CSmsDialer::processIncommingSMS()
{
   // Check if incoming SMS

   //TODO
   //boost::shared_ptr<std::vector<ISms> > incommingSms = m_phone->getIncomingSMS();
   //if (!incommingSms)
      return;  // No new message

   //TODO : traiter le message reçu
}