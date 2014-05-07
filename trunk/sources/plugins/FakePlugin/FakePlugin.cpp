#include "stdafx.h"
#include "FakePlugin.h"
#include <shared/Log.h>
#include <shared/xpl/XplService.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplHelper.h>
#include <shared/event/EventTimer.h>
#include "FakeTemperatureSensor.h"

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
   kEvtUpdateConfiguration,
   kEvtTimerSendMessage
};

// XPL device ID : use to identify this plugin over the XPL network.
// Must match XPL rules (see http://xplproject.org.uk/wiki/index.php/XPL_Specification_Document) :
//   - alphanumerical characters only : [0-9][a-z]
//   - letters must be lower case
//   - no characters '.', '-', '_', or so...
//   - 8 characters max
// NOTE : To avoid DeviceId conflicts, YADOMS-TEAM manage a device ID reservation table. Please contact to reserve your device ID.
// You can check that your device ID mach Xpl rules calling shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kDeviceId, your_device_id)
static const std::string& XplDeviceId("fakeplug");

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
      shared::xpl::CXplService xplService(
         XplDeviceId,                                                // XPL device ID : use to identify this plugin over the XPL network
         shared::xpl::CXplHelper::toInstanceId(instanceUniqueId),    // Use the plugin instance id (guaranteed by Yadoms to be unique among all instances of all plugins) as XPL instance id
         pluginIOService,                                            // Use the provided io service for better performance
         this,                                                       // Subscribe for XPL message receive event
         kEvtXplMessage);                                            // Set the event ID to rise when XPL message is received

      // Configure XPL filter to only receive some messages from Yadoms
      xplService.setFilter(
         "xpl-cmnd",                                                 // Only commands
         xplService.getActor().getVendorId(),                        // From Yadoms
         shared::xpl::CXplHelper::WildcardString,                    // From any devices
         xplService.getActor().getInstanceId(),                      // From current Yadoms instance
         shared::xpl::CXplHelper::WildcardString,                    // From any device classes
         "basic");                                                   // From any devices type

      // Fake temperature sensor
      CFakeTemperatureSensor temperatureSensor("fakeTempSensor");

      // Timer used to send a XPL message periodically
      createTimer(kEvtTimerSendMessage, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));

      // the main loop
      YADOMS_LOG(debug) << "CFakePlugin is running...";
      while(1)
      {
         // Wait for an event
         switch(waitForEvents())
         {
         case kEvtXplMessage:
            {
               // Xpl message was received
               shared::xpl::CXplMessage xplMessage = getEventData<shared::xpl::CXplMessage>();
               YADOMS_LOG(debug) << "XPL message event received :" << xplMessage.toString();
               break;
            }
         case kEvtUpdateConfiguration:
            {
               // Configuration was updated
               std::string newConfiguration = getEventData<std::string>();
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
         case kEvtTimerSendMessage:
            {
               // Timer used here to send a XPL message periodically

               // First read the sensor value
               temperatureSensor.read();

               // Create the message
               shared::xpl::CXplMessage msg(
                  shared::xpl::CXplMessage::kXplStat,                            // Message type
                  xplService.getActor(),                                         // Source actor (here : our fake plugin instance)
                  shared::xpl::CXplActor::createBroadcastActor(),            // Target actor (here : the XPL logger of Yadoms)
                  shared::xpl::CXplMessageSchemaIdentifier("sensor", "basic"));   // The message schema

               // Add data to message
               // - Device ID
			      msg.addToBody("device", temperatureSensor.getDeviceId());
               // - Sensor type
               msg.addToBody("type", "temp");
               // - Unit
               msg.addToBody("units", "°C");
               // - Signal strength
               msg.addToBody("rssi", boost::lexical_cast<std::string>(temperatureSensor.getRssi()));
               // - Current temperature
               std::ostringstream ss;
               ss << std::fixed << std::setprecision(2) << temperatureSensor.getTemperature();
               msg.addToBody("current", ss.str());
               // - Battery level
			      msg.addToBody("battery", boost::lexical_cast<std::string>(temperatureSensor.getBatteryLevel()));

               // Send it
               xplService.sendMessage(msg);

               YADOMS_LOG(debug) << "Send the periodically temperature message...";

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
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(info) << "CFakePlugin is stopping..."  << std::endl;
   }
}

void CFakePlugin::updateConfiguration(const std::string& configuration)
{
   // This function is called in a Yadoms thread context, so send a event to the CFakePlugin thread
   sendEvent<std::string>(kEvtUpdateConfiguration, configuration);
}
