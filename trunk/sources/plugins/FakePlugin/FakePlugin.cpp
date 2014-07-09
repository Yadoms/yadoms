#include "stdafx.h"
#include "FakePlugin.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include "FakeSensor.h"

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
   kSendTemperatureTimerEventId = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from yApi::IYadomsApi::kPluginFirstEventId
};

void CFakePlugin::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());
      // Trace the configuration (just for test)
      m_configuration.trace();

      // Create 2 fake sensors
      CFakeSensor fakeSensor1("fakeSensor1");
      CFakeSensor fakeSensor2("fakeSensor2");
      
      // Declare these sensors to Yadoms
      fakeSensor1.declareDevice(context);
      fakeSensor2.declareDevice(context);

      // Timer used to send fake sensor states periodically
      context->getEventHandler().createTimer(kSendTemperatureTimerEventId, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));

      // the main loop
      YADOMS_LOG(debug) << "CFakePlugin is running...";
      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case yApi::IYadomsApi::kEventDeviceCommand:
            {
               // A command was received from Yadoms
            boost::shared_ptr<const yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
               YADOMS_LOG(debug) << "Command received from Yadoms :" << command->toString();
               break;
            }
         case yApi::IYadomsApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               shared::CDataContainer newConfiguration = context->getEventHandler().getEventData<shared::CDataContainer>();
               YADOMS_LOG(debug) << "configuration was updated...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Take into account the new configuration
               // - Restart the plugin if necessary,
               // - Update some resources,
               // - etc...
               m_configuration.initializeWith(newConfiguration);

               // Trace the configuration
               m_configuration.trace();

               break;
            }
         case kSendTemperatureTimerEventId:
            {
               // Timer used here to send the temperature to Yadoms periodically

               // Read sensor value and send data to Yadoms (temperatures, battery level, Rssi measure...)
               fakeSensor1.read();
               fakeSensor1.historizeData(context);
               fakeSensor2.read();
               fakeSensor2.historizeData(context);

               YADOMS_LOG(debug) << "Send the periodically sensors state...";

               break;
            }
         default:
            {
               YADOMS_LOG(error) << "Unknown ou unsupported message id " << context->getEventHandler().getEventId();
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
   }
}