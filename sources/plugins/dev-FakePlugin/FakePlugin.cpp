#include "stdafx.h"
#include "FakePlugin.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "FakeSensor.h"
#include "FakeCounter.h"
#include "FakeSwitch.h"
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IDeviceConfigurationSchemaRequest.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/ISetDeviceConfiguration.h>
#include <shared/tools/Random.h>
#include "FakeController.h"
#include "FakeConfigurableDevice.h"
#include <Poco/Net/NetworkInterface.h>

#include <shared/Log.h>

// Use this macro to define all necessary to make your plugin a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
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
   kSendSensorsStateTimerEventId = yApi::IYPluginApi::kPluginFirstEventId, // Always start from yApi::IYPluginApi::kPluginFirstEventId
};

void CFakePlugin::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Informs Yadoms about the plugin actual state
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   YADOMS_LOG(information) << "CFakePlugin is starting...";

   YADOMS_LOG(information) << "The instance data path : " << api->getDataPath().string();

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());
   // Trace the configuration (just for test)
   m_configuration.trace();

   // Create 2 fake sensors
   CFakeSensor fakeSensor1("fakeSensor1");
   CFakeSensor fakeSensor2("fakeSensor2");
   CFakeCounter fakeCounter("fakeCounter");
   CFakeSwitch fakeOnOffReadOnlySwitch("fakeOnOffReadOnlySwitch", false, false);
   CFakeSwitch fakeOnOffReadWriteSwitch("fakeOnOffReadWriteSwitch", false, true);
   CFakeSwitch fakeDimmableReadOnlySwitch("fakeDimmableReadOnlySwitch", true, false);
   CFakeSwitch fakeDimmableReadWriteSwitch("fakeDimmableReadWriteSwitch", true, true);
   CFakeController fakeController("fakeController1");
   CFakeConfigurableDevice configurableDevice("configurableDevice");


   // Declare these sensors to Yadoms (devices and associated keywords) if not already declared
   fakeSensor1.declareDevice(api);
   fakeSensor2.declareDevice(api);
   fakeCounter.declareDevice(api);
   fakeOnOffReadOnlySwitch.declareDevice(api);
   fakeOnOffReadWriteSwitch.declareDevice(api);
   fakeDimmableReadOnlySwitch.declareDevice(api);
   fakeDimmableReadWriteSwitch.declareDevice(api);
   fakeController.declareDevice(api);
   configurableDevice.declareDevice(api);

   // Timer used to send fake sensor states periodically
   api->getEventHandler().createTimer(kSendSensorsStateTimerEventId,
                                      shared::event::CEventTimer::kPeriodic,
                                      boost::posix_time::seconds(10));

   api->setPluginState(yApi::historization::EPluginState::kRunning);

   // the main loop
   while (1)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            // Yadoms request the plugin to stop. Note that plugin must be stop in 10 seconds max, otherwise it will be killed.
            YADOMS_LOG(information) << "Stop requested";
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }

      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            // Configuration was updated
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainer>();
            YADOMS_LOG(information) << "Update configuration...";

            // Take into account the new configuration
            // - Restart the plugin if necessary,
            // - Update some resources,
            // - etc...
            m_configuration.initializeWith(newConfiguration);

            // Trace the configuration
            m_configuration.trace();

            api->setPluginState(yApi::historization::EPluginState::kRunning);

            break;
         }

      case yApi::IYPluginApi::kEventExtraQuery:
         {
            // Extra-command was received from Yadoms
            auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery>>();

            if (extraQuery)
            {
               YADOMS_LOG(information) << "Extra command received : " << extraQuery->getData().query();

               if (extraQuery->getData().query() == "simpleCommand")
               {
                  YADOMS_LOG(information) << "Simple command received";
               }
               else if (extraQuery->getData().query() == "dataCommand")
               {
                  auto s = extraQuery->getData().data().get<std::string>("testValue");
                  YADOMS_LOG(information) << "Command with data received : data=" << s;
               }
               else if (extraQuery->getData().query() == "dataBindingCommand")
               {
                  auto value = extraQuery->getData().data().get<std::string>("networkInterface");
                  YADOMS_LOG(information) << "Command with binded data received : value=" << value << " text=" << Poco::Net::NetworkInterface::forName(value).displayName();
               }
               else if (extraQuery->getData().query() == "dataBindingPluginCommand")
               {
                  auto interval = extraQuery->getData().data().get<std::string>("dynamicSection.content.interval");
                  YADOMS_LOG(information) << "Command with plugin binded data received : value=" << interval;
               }
               else if (extraQuery->getData().query() == "changePluginStateMessage")
               {
                  auto message = extraQuery->getData().data().get<std::string>("newStateMessage");
                  api->setPluginState(shared::plugin::yPluginApi::historization::EPluginState::kCustom, "newCustomStateMessage", {{"messageFromExtraQuery", message}});
               }
            }

            // Extra-query can return success or error indicator. In case of success, can also return data.
            // Return here a success without data (=empty container)
            extraQuery->sendSuccess(shared::CDataContainer());
            break;
         }

      case yApi::IYPluginApi::kBindingQuery:
         {
            // Yadoms ask for a binding query 
            auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IBindingQueryRequest>>();
            if (request->getData().getQuery() == "test")
            {
               shared::CDataContainer ev;
               ev.set("HOUR", "1 hour");
               ev.set("DAY", "1 day");
               ev.set("WEEK", "1 week");
               ev.set("MONTH", "1 month");
               ev.set("HALF_YEAR", "6 months");
               ev.set("YEAR", "1 year");

               shared::CDataContainer en;
               en.set("name", "Interval of the chart");
               en.set("description", "Permit to change the interval of all the chart");
               en.set("type", "enum");
               en.set("values", ev);
               en.set("defaultValue", "DAY");

               shared::CDataContainer result;
               result.set("interval", en);

               request->sendSuccess(result);
            }
            else
            {
               auto errorMessage = (boost::format("unknown query : %1%") % request->getData().getQuery()).str();
               request->sendError(errorMessage);
               YADOMS_LOG(error) << errorMessage;
            }
            break;
         }

      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // A command was received from Yadoms
            auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
            YADOMS_LOG(information) << "Command received from Yadoms : " << yApi::IDeviceCommand::toString(command);
            if (command->getKeyword().empty())
               YADOMS_LOG(information) << "Specific command for a device (can be used for any purpose, unless keyword driving, device configuration (see kGetDeviceConfigurationSchemaRequest and kSetDeviceConfiguration documentation) and deletion";
            else
               YADOMS_LOG(information) << "Standard command to a keyword (used to drive a switch, a thermostat...";
            break;
         }

      case yApi::IYPluginApi::kEventManuallyDeviceCreation:
         {
            auto creation = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
            try
            {
               // Yadoms asks for device creation
               auto sni = creation->getData().getConfiguration().get<std::string>("networkInterface");
               auto dyn = creation->getData().getConfiguration().get<std::string>("dynamicSection.content.interval");

               auto devId = (boost::format("%1%_%2%_0x%3$08X") % sni % dyn % shared::tools::CRandom::generateNbBits(26, false)).str();
               api->declareDevice(devId,
                                  "FakeDevice_" + devId,
                                  std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>(),
                                  creation->getData().getConfiguration());

               api->declareKeyword(devId, boost::make_shared<yApi::historization::CSwitch>("manualSwitch"));

               creation->sendSuccess(devId);
            }
            catch (std::exception& ex)
            {
               creation->sendError(ex.what());
            }
            break;
         }

      case yApi::IYPluginApi::kEventDeviceRemoved:
         {
            auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved>>();
            YADOMS_LOG(information) << device->device() << " was removed";
            break;
         }

      case yApi::IYPluginApi::kGetDeviceConfigurationSchemaRequest:
         {
            // Yadoms ask for device configuration schema
            // Schema can come from package.json, or built by code. In this example,
            // we just take the schema from package.json, in case of configuration is supported by device.
            auto deviceConfigurationSchemaRequest = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IDeviceConfigurationSchemaRequest>>();

            if (deviceConfigurationSchemaRequest->device() == configurableDevice.getDeviceName())
            {
               YADOMS_LOG(information) << "This device is configurable, return its configuration schema to device configuration schema request";
               deviceConfigurationSchemaRequest->sendSuccess(configurableDevice.getDynamicConfiguration());
            }
            else
            {
               YADOMS_LOG(information) << "This device is not configurable, return an empty schema to device configuration schema request";
               deviceConfigurationSchemaRequest->sendSuccess(shared::CDataContainer());
            }

            break;
         }

      case yApi::IYPluginApi::kSetDeviceConfiguration:
         {
            // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
            auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration>>();

            if (deviceConfiguration->device() == configurableDevice.getDeviceName())
            {
               configurableDevice.setConfiguration(deviceConfiguration->configuration());
            }
            else
            {
               auto errorMessage = "Try to apply a device configuration to an unconfigurable device";
               YADOMS_LOG(error) << errorMessage;
            }

            break;
         }

      case kSendSensorsStateTimerEventId:
         {
            // Timer used here to send sensors state to Yadoms periodically

            // Read sensor value and send data to Yadoms (temperatures, battery level, Rssi measure...)
            fakeSensor1.read();
            fakeSensor2.read();
            fakeCounter.read();
            fakeOnOffReadOnlySwitch.read();
            fakeDimmableReadOnlySwitch.read();
            fakeController.read();
            configurableDevice.read();

            YADOMS_LOG(information) << "Send the periodically sensors state...";
            fakeSensor1.historizeData(api);
            fakeSensor2.historizeData(api);
            fakeCounter.historizeData(api);
            fakeOnOffReadOnlySwitch.historizeData(api);
            fakeDimmableReadOnlySwitch.historizeData(api);
            fakeController.historizeData(api);
            configurableDevice.historizeData(api);

            break;
         }

      default:
         {
            YADOMS_LOG(error) << "Unknown or unsupported message id " << api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

