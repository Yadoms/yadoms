#include "stdafx.h"
#include "FakePlugin.h"
#include <shared/Log.h>
#include <shared/xpl/XplService.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplHelper.h>
#include <boost/shared_ptr.hpp>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values
IMPLEMENT_PLUGIN(CFakePlugin)


CFakePlugin::CFakePlugin()
{
}

CFakePlugin::~CFakePlugin()
{
}

// Event IDs
enum
{
   kEvtXplMessage = shared::event::kUserFirstId,   // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtUpdateConfiguration
};

void CFakePlugin::doWork(int instanceUniqueId, const std::string& configuration, boost::asio::io_service * pluginIOService)
{
   try
   {
      YADOMS_LOG_CONFIGURE(Informations->getName());
      YADOMS_LOG(debug) << "CFakePlugin is starting...";

      // Load configuration values (provided by database)
      m_configuration.set(configuration);
      // Trace the configuration (just for test)
      m_configuration.trace();

      // Register to XPL service
      shared::xpl::CXplService xplService(/* TODO remplacer toVendorIdOrDeviceId(fct à supprimer) par une constante avec gros commentaires (contraintes de nommage, réservation du nom, etc...)*/
         shared::xpl::CXplHelper::toVendorIdOrDeviceId(Informations->getName()),    // Use the plugin name as XPL device ID
         shared::xpl::CXplHelper::toStructuralElement(instanceUniqueId),            // Use the plugin instance id (guaranteed by Yadoms to be unique among all instances of all plugins) as XPL instance id
         pluginIOService,                                                           // Use the provided io service for better performance
         this,                                                                      // Subscribe for XPL message receive event
         kEvtXplMessage);                                                           // Set the event ID to rise when XPL message is received

      // A simple incrementing value, sent on XPL network
      int xplTestData = 0;

      // the main loop
      YADOMS_LOG(debug) << "CFakePlugin is running...";
      while(1)
      {
         // Wait for an event, with timeout
         // Timeout is used here to send periodically a XPL message
         switch(waitForEvents(boost::posix_time::milliseconds(20000)))
         {
         case kEvtXplMessage:
            {
               // Xpl message was received
               shared::xpl::CXplMessage xplMessage = popEvent<shared::xpl::CXplMessage>();
               YADOMS_LOG(debug) << "XPL message event received :" << xplMessage.toString();
               break;
            }
         case kEvtUpdateConfiguration:
            {
               // Configuration was updated
               std::string newConfiguration = popEvent<std::string>();
               YADOMS_LOG(debug) << "configuration was updated...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEvtUpdateConfiguration shouldn't be generated

               // Take into account the new configuration
               // - Restart the plugin if necessary,
               // - Update some resources,
               // - etc...
               m_configuration.set(newConfiguration);

               // Trace the configuration
               m_configuration.trace();

               break;
            }
         case shared::event::kTimeout:
            {
               // Timeout, used here to send a XPL message periodically

               // Create the message
               shared::xpl::CXplMessage msg(
                  shared::xpl::CXplMessage::kXplStat,                            // Message type
                  xplService.getActor(),                                         // Source actor (here : our fake plugin instance)
                  shared::xpl::CXplActor::createActor("logger", "1"),            // Target actor (here : the XPL logger of Yadoms)
                  shared::xpl::CXplMessageSchemaIdentifier("clock", "basic"));   // The message schema

               // Add data to message
               msg.addToBody("value", boost::lexical_cast<std::string>(xplTestData++));

               // Send it
               xplService.sendMessage(msg);

               break;
            }
         default:
            {
               YADOMS_LOG(error) << "Unknown message id";

               // We need to consume this unknown event
               popEvent();

               break;
            }
         }
      };
   }
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(info) << "CFakePlugin is stopping..."  << std::endl;
   }
   catch(...)
   {
   }

}

void CFakePlugin::updateConfiguration(const std::string& configuration)
{
   // This function is called in a Yadoms thread context, so send a event to the CFakePlugin thread
   sendEvent<std::string>(kEvtUpdateConfiguration, configuration);
}
