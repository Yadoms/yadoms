#include "stdafx.h"
#include "FakePlugin.h"
#include <shared/Log.h>
#include <shared/xpl/XplService.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplConstants.h>
#include <shared/exception/BadConversion.hpp>
#include <shared/StringExtension.h>


// Use this macro to define some basic informations about the plugin
// Defines the static PluginInformations value that can be used by plugin to retrieve values
IMPLEMENT_PLUGIN(
   CFakePlugin,                                 // IPlugin implementation class
   "FakePlugin",                                // Plugin name (std::string)
   "0.1",                                       // Current plugin version (std::string)
   shared::plugin::information::kBeta,          // Current release state (shared::plugin::information::EReleaseType)
   "yadoms-team",                               // Author name (std::string)
   "http://sourceforge.net/projects/yadoms/")   // Url of author web site (std::string)


CFakePlugin::CFakePlugin()
{
   // Build the schema
   m_Configuration.buildSchema();
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

void CFakePlugin::doWork(int instanceUniqueId, const std::string& configurationValues, boost::asio::io_service * pluginIOService)
{
   try
   {
      YADOMS_LOG_CONFIGURE("FakePlugin");
      YADOMS_LOG(debug) << "CFakePlugin is starting...";

      // Load configuration values (provided by database)
      m_Configuration.setValues(configurationValues);
      // Trace the configuration (just for test)
      m_Configuration.trace();

      // Register to XPL service
      // Note that we use plugin instance id (guaranteed by Yadoms to be unique among all instances of all plugins)
      // as XPL device instance ID
      shared::xpl::CXplService xplService(PluginInformations.getName(), boost::lexical_cast<std::string>(instanceUniqueId),
         pluginIOService, this, kEvtXplMessage);

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
               std::string newConfigurationValues = popEvent<std::string>();
               YADOMS_LOG(debug) << "configuration was updated...";
               BOOST_ASSERT(!newConfigurationValues.empty());  // newConfigurationValues shouldn't be empty, or kEvtUpdateConfiguration shouldn't be generated

               // Take into account the new configuration
               // - Restart the plugin if necessary,
               // - Update some resources,
               // - etc...
               m_Configuration.setValues(newConfigurationValues);

               // Trace the configuration
               m_Configuration.trace();

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

void CFakePlugin::updateConfiguration(const std::string& configurationValues)
{
   // This function is called in a Yadoms thread context, so send a event to the CFakePlugin thread
   sendEvent<std::string>(kEvtUpdateConfiguration, configurationValues);
}
