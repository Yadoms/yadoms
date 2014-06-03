#include "stdafx.h"
#include "FakePlugin.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
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
   kSendTemperatureTimerEventId = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from yApi::IYadomsApi::kPluginFirstEventId
};

void CFakePlugin::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      YADOMS_LOG_CONFIGURE(context->getInformation().getName());
      YADOMS_LOG(debug) << "CFakePlugin is starting...";

      // Load configuration values (provided by database)
      m_configuration.set(context->getConfiguration());
      // Trace the configuration (just for test)
      m_configuration.trace();

      // Fake temperature sensor
      CFakeTemperatureSensor temperatureSensor("fakeTempSensor");
      // Declare it if not already exists
      if (!context->deviceExists(temperatureSensor.getDeviceId()))
         context->declareNewDevice(temperatureSensor.getDeviceId(), temperatureSensor.getCapacities());

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
               // Command was received from Yadoms
               yApi::IYadomsApi::CDeviceCommand command = context->getEventHandler().getEventData<yApi::IYadomsApi::CDeviceCommand>();
               YADOMS_LOG(debug) << "Command received from Yadoms :" << command.toString();
               break;
            }
         case yApi::IYadomsApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               std::string newConfiguration = context->getEventHandler().getEventData<std::string>();
               YADOMS_LOG(debug) << "configuration was updated...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Take into account the new configuration
               // - Restart the plugin if necessary,
               // - Update some resources,
               // - etc...
               m_configuration.set(newConfiguration);

               // Trace the configuration
               m_configuration.trace();

               break;
            }
         case kSendTemperatureTimerEventId:
            {
               // Timer used here to send the temperature to Yadoms periodically

               // First read the sensor value
               temperatureSensor.read();

               // Send data to Yadoms : read temperature, battery level and Rssi measure
               context->historizeData(temperatureSensor.getDeviceId(), "temp1", yApi::CStandardCapacities::getTemperatureSensorCapacity(), boost::lexical_cast<std::string>(temperatureSensor.getTemperature1()));
               context->historizeData(temperatureSensor.getDeviceId(), "temp2", yApi::CStandardCapacities::getTemperatureSensorCapacity(), boost::lexical_cast<std::string>(temperatureSensor.getTemperature2()));
               context->historizeData(temperatureSensor.getDeviceId(), "battery", yApi::CStandardCapacities::getBatteryLevelCapacity(), boost::lexical_cast<std::string>(temperatureSensor.getBatteryLevel()));
               context->historizeData(temperatureSensor.getDeviceId(), "Rssi", yApi::CStandardCapacities::getRssiMeasureCapacity(), boost::lexical_cast<std::string>(temperatureSensor.getRssi()));


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