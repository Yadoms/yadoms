#include "stdafx.h"
#include "Rfxcom.h"
#include <shared/Log.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplHelper.h>
#include <shared/xpl/XplException.h>
#include <shared/exception/EmptyResult.hpp>
#include "RfxcomFactory.h"

IMPLEMENT_PLUGIN(CRfxcom)


CRfxcom::CRfxcom()
{
}

CRfxcom::~CRfxcom()
{
}

// Event IDs
enum
{
   kEvtXplMessage = shared::event::kUserFirstId,   // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtUpdateConfiguration,
   kEvtPortConnection,
   kEvtPortDataReceived
};

// XPL device ID
static const std::string& XplDeviceId("rfxcom");


void CRfxcom::doWork(int instanceUniqueId, const std::string& configuration, boost::asio::io_service& pluginIOService)
{
   try
   {
      YADOMS_LOG_CONFIGURE(Informations->getName());
      YADOMS_LOG(debug) << "CRfxcom is starting...";

      // Load configuration values (provided by database)
      m_configuration.set(configuration);

      // Create the port instance
      m_port = CRfxcomFactory::constructPort(m_configuration, pluginIOService, shared_from_this(), kEvtPortConnection, kEvtPortDataReceived);

      // Register to XPL service
      m_xplService.reset(new shared::xpl::CXplService(
         XplDeviceId,
         shared::xpl::CXplHelper::toInstanceId(instanceUniqueId),
         &pluginIOService));

      // Configure XPL filter to only receive commands from Yadoms
      m_xplService->subscribeForMessages(
         "xpl-cmnd",
         m_xplService->getActor().getVendorId(),
         shared::xpl::CXplHelper::WildcardString,
         m_xplService->getActor().getInstanceId(),
         shared::xpl::CXplHelper::WildcardString,
         shared::xpl::CXplHelper::WildcardString,
         this,
         kEvtXplMessage);

      // the main loop
      YADOMS_LOG(debug) << "CRfxcom is running...";

      while(1)
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

               // Close connection
               m_port.reset();

               // Update configuration
               m_configuration.set(newConfiguration);

               // Create new connection
               m_port = CRfxcomFactory::constructPort(m_configuration, pluginIOService, shared_from_this(), kEvtPortConnection, kEvtPortDataReceived);

               break;
            }
         case kEvtPortConnection:
            {
               if (getEventData<bool>())
                  processRfxcomConnectionEvent();
               else
                  processRfxcomUnConnectionEvent();

               break;
            }
         case kEvtPortDataReceived:
            {
               YADOMS_LOG(debug) << "RFXCom >>> " << getEventData<std::string>();
               //TODO traiter les données reçues
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
      YADOMS_LOG(info) << "CRfxcom is stopping..."  << std::endl;
   }
}

void CRfxcom::updateConfiguration(const std::string& configuration)
{
   // This function is called in a Yadoms thread context, so send a event to the CRfxcom thread
   sendEvent<std::string>(kEvtUpdateConfiguration, configuration);
}

void CRfxcom::onXplMessageReceived(const shared::xpl::CXplMessage& xplMessage)
{
   YADOMS_LOG(debug) << "XPL message event received :" << xplMessage.toString();

   BOOST_ASSERT_MSG(xplMessage.getTypeIdentifier() == shared::xpl::CXplMessage::kXplCommand, "Filter doesn't work, messages must be xpl-cmnd");

   if (m_transceiver)
      m_transceiver->send(xplMessage);
}

void CRfxcom::processRfxcomConnectionEvent()
{
   YADOMS_LOG(debug) << "RFXCom is now connected";
   LogEvent("RFXCom is now connected");

   BOOST_ASSERT_MSG(!m_transceiver, "RFXCom was already connected");
   m_transceiver.reset();

   m_transceiver = CRfxcomFactory::constructTransceiver(m_port);

   // Reset the RFXCom state
   m_transceiver->sendReset();
}

void CRfxcom::processRfxcomUnConnectionEvent()
{
   YADOMS_LOG(debug) << "RFXCom connection was lost";
   LogEvent("RFXCom connection was lost");

   m_transceiver.reset();
}

void CRfxcom::LogEvent(const std::string& reason)
{
   try
   {
      // Add event into plugin event logger table
      //TODO 
      //m_pluginLogger->addEvent(
      //   getInformation().getName(),
      //   getInformation().getVersion(),
      //   getInformation().getReleaseType(),
      //   kInfo,
      //   reason);
   }
   catch (shared::exception::CEmptyResult& e)
   {
      // Just log the error
      YADOMS_LOG(error) << "Error when adding plugin event in database : " << e.what();
   }
}