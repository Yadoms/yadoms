#include "stdafx.h"
#include "FakePlugin.h"
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
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
   kSendTemperatureTimerEventId = shared::plugin::yadomsApi::IYadomsApi::kPluginFirstEventId,   // Always start from shared::plugin::yadomsApi::IYadomsApi::kPluginFirstEventId
};

void CFakePlugin::doWork(boost::shared_ptr<shared::plugin::yadomsApi::IYadomsApi> yadoms)
{
   try
   {
      YADOMS_LOG_CONFIGURE(Informations->getName());//TODO Informations à mettre dans le package.json ?
      YADOMS_LOG(debug) << "CFakePlugin is starting...";

      // Load configuration values (provided by database)
      m_configuration.set(yadoms->getConfiguration());
      // Trace the configuration (just for test)
      m_configuration.trace();

      // Fake temperature sensor
      CFakeTemperatureSensor temperatureSensor("fakeTempSensor");
      // Declare it if not already exists
      if (!yadoms->deviceExists(temperatureSensor.getDeviceId()))
         yadoms->declareNewDevice(temperatureSensor.getDeviceId(), temperatureSensor.getCapacities());

      // Timer used to send fake sensor states periodically
      yadoms->getEventHandler().createTimer(kSendTemperatureTimerEventId, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));

      // the main loop
      YADOMS_LOG(debug) << "CFakePlugin is running...";
      while(1)
      {
         // Wait for an event
         switch(yadoms->getEventHandler().waitForEvents())
         {
         case shared::plugin::yadomsApi::IYadomsApi::kEventDeviceCommand://TODO : revoir, nom trop long
            {
               // Command was received from Yadoms
               //TODO shared::plugin::yadomsApi::IYadomsApi::CDeviceCommand command = getEventData<shared::plugin::yadomsApi::IYadomsApi::CDeviceCommand>();
               //YADOMS_LOG(debug) << "Command received from Yadoms :" << command.toString();
               break;
            }
         case shared::plugin::yadomsApi::IYadomsApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               std::string newConfiguration = yadoms->getEventHandler().getEventData<std::string>();
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
               yadoms->historizeData(temperatureSensor.getDeviceId(), "temp1", shared::plugin::yadomsApi::CStandardCapacities::getTemperatureSensorCapacity(), boost::lexical_cast<std::string>(temperatureSensor.getTemperature1()));
               yadoms->historizeData(temperatureSensor.getDeviceId(), "temp2", shared::plugin::yadomsApi::CStandardCapacities::getTemperatureSensorCapacity(), boost::lexical_cast<std::string>(temperatureSensor.getTemperature2()));
               yadoms->historizeData(temperatureSensor.getDeviceId(), "battery", shared::plugin::yadomsApi::CStandardCapacities::getBatteryLevelCapacity(), boost::lexical_cast<std::string>(temperatureSensor.getBatteryLevel()));
               yadoms->historizeData(temperatureSensor.getDeviceId(), "Rssi", shared::plugin::yadomsApi::CStandardCapacities::getRssiMeasureCapacity(), boost::lexical_cast<std::string>(temperatureSensor.getRssi()));


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