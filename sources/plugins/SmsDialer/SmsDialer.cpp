#include "stdafx.h"
#include "SmsDialer.h"
#include <shared/Log.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplHelper.h>
#include <shared/xpl/XplException.h>
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
   kEvtTimerTryToConnectToPhone,
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
      m_xplService.reset(new shared::xpl::CXplService(
         XplDeviceId,
         shared::xpl::CXplHelper::toInstanceId(instanceUniqueId),
         pluginIOService,
         this,
         kEvtXplMessage));

      // Configure XPL filter to only receive SMS commands from Yadoms
      m_xplService->setFilter(
         "xpl-cmnd",
         m_xplService->getActor().getVendorId(),
         shared::xpl::CXplHelper::WildcardString,
         m_xplService->getActor().getInstanceId(),
         "message",
         "sms");

      // the main loop
      YADOMS_LOG(debug) << "CSmsDialer is running...";

      // Timer used to periodically try to connect to phone
      m_connectionTimer = createTimer(kEvtTimerTryToConnectToPhone, shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(1));
      m_connectionTimer->stop();

      // Timer used to periodically check for incoming SMS
      m_incommingSmsPollTimer = createTimer(kEvtTimerCheckForIncommingSms, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(30));
      m_incommingSmsPollTimer->stop();

      while(1)
      {
         if (!m_phone->isConnected())
         {
            ProcessNotConnectedState();
         }
         else
         {
            // Send connection state message
            sendConnectionState();

            ProcessConnectedState();
         }
      };
   }
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(info) << "CSmsDialer is stopping..."  << std::endl;
   }
}

void CSmsDialer::ProcessNotConnectedState()
{
   YADOMS_LOG(info) << "Phone is not connected"  << std::endl;

   m_incommingSmsPollTimer->stop();
   m_connectionTimer->start();

   while (!m_phone->isConnected())
   {
      try
      {
         // Wait for an event
         switch(waitForEvents())
         {
         case kEvtXplMessage:
            {
               // Ignore all XPL message
               break;
            }
         case kEvtUpdateConfiguration:
            {
               // Configuration was updated
               std::string newConfiguration = getEventData<std::string>();
               YADOMS_LOG(debug) << "configuration was updated...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEvtUpdateConfiguration shouldn't be generated

               // Destroy current phone instance to update configuration
               m_phone.reset();

               // Update configuration
               m_configuration.set(newConfiguration);

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

void CSmsDialer::ProcessConnectedState()
{
   YADOMS_LOG(info) << "Phone is connected"  << std::endl;

   m_connectionTimer->stop();
   m_incommingSmsPollTimer->start();

   while (m_phone->isConnected())
   {
      try
      {
         // Wait for an event
         switch(waitForEvents())
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
         case kEvtTimerTryToConnectToPhone:
            {
               // Ignore this event
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
      }
      catch (CPhoneException& e)
      {
         YADOMS_LOG(error) << "Phone critical error : " << e.what();
      }
   }
}

void CSmsDialer::updateConfiguration(const std::string& configuration)
{
   // This function is called in a Yadoms thread context, so send a event to the CSmsDialer thread
   sendEvent<std::string>(kEvtUpdateConfiguration, configuration);
}

void CSmsDialer::onXplMessageReceived(const shared::xpl::CXplMessage& xplMessage)
{
   //TODO : gérer aussi le sendmsg.basic (pour ça, il faut une mécanique de filtres XPL plus complexe)
   YADOMS_LOG(debug) << "XPL message event received :" << xplMessage.toString();

   BOOST_ASSERT_MSG(xplMessage.getMessageSchemaIdentifier().getClassId() == "message", "Filter doesn't work");
   BOOST_ASSERT_MSG(xplMessage.getMessageSchemaIdentifier().getTypeId() == "sms", "Filter doesn't work");
   BOOST_ASSERT_MSG(!xplMessage.getBodyValue("to").empty(), "SMS recipient is empty");
   BOOST_ASSERT_MSG(!xplMessage.getBodyValue("body").empty(), "SMS message body is empty");

   const bool ackRequired = xplMessage.hasBodyValue("acknowledgment") && xplMessage.getBodyValue("acknowledgment") == "true";

   const std::string to = xplMessage.getBodyValue("to");
   const std::string body = xplMessage.getBodyValue("body");

   try
   {
      boost::shared_ptr<ISms> sms(new CSms(to, body));
      m_phone->send(sms);
      if (ackRequired)
         SendXplAck(true, body);
   }
   catch (shared::xpl::CXplException& e)
   {
      YADOMS_LOG(error) << "Can not send SMS, the XPL message is invalid : " << e.what();
      if (ackRequired)
         SendXplAck(false, body);
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << "Error sending SMS : " << e.what();
      if (ackRequired)
         SendXplAck(false, body);
   }
}

void CSmsDialer::processIncommingSMS()
{
   // Check if incoming SMS

   //TODO
   boost::shared_ptr<std::vector<ISms> > incommingSms = m_phone->getIncomingSMS();
   //if (!incommingSms)

   //TODO : traiter le message reçu
}

void CSmsDialer::sendConnectionState() const
{
   // Send state only if phone is known
   if (m_phone->getUniqueId().empty())
      return;

   shared::xpl::CXplMessage msg(
      shared::xpl::CXplMessage::kXplStat,                            // Message type
      m_xplService->getActor(),                                      // Source actor (here : our plugin instance)
      shared::xpl::CXplActor::createBroadcastActor(),                // Target actor (here : the XPL logger of Yadoms)
      shared::xpl::CXplMessageSchemaIdentifier("message", "sms"));   // The message schema

   // Add data to message
   // - Device ID
   msg.addToBody("device", m_phone->getUniqueId());

   // Send it
   m_xplService->sendMessage(msg);
}

void CSmsDialer::SendXplAck(bool ok, const std::string& sourceMsg) const
{
   // Send state only if phone is known
   if (m_phone->getUniqueId().empty())
      return;

   shared::xpl::CXplMessage msg(
      shared::xpl::CXplMessage::kXplTrigger,                         // Message type
      m_xplService->getActor(),                                      // Source actor (here : our plugin instance)
      shared::xpl::CXplActor::createBroadcastActor(),                // Target actor (here : the XPL logger of Yadoms)
      shared::xpl::CXplMessageSchemaIdentifier("message", "sms"));   // The message schema

   // Add data to message
   // - Device ID
   msg.addToBody("device", m_phone->getUniqueId());
   // - From (doesn't make sense for an acknowledge)
   msg.addToBody("from", shared::CStringExtension::EmptyString);
   // - Type
   msg.addToBody("type", ok ? "acknowledgment" : "error");
   // - Content
   msg.addToBody("content", sourceMsg);

   // Send it
   m_xplService->sendMessage(msg);
}