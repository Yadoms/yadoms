#include "stdafx.h"
#include "FakePlugin.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "FakeSensor.h"
#include "FakeCounter.h"
#include "FakeSwitch.h"
#include "FakeController.h"
#include "FakeConfigurableDevice.h"
#include "FakeCurtain.h"
#include <Poco/Net/NetworkInterface.h>
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/configuration/File.h>

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

   // Create fake sensors
   CFakeSensor fakeSensor1("fakeSensor1");
   CFakeSensor fakeSensor2("fakeSensor2");
   CFakeCounter fakeCounter("fakeCounter");
   CFakeSwitch fakeOnOffReadOnlySwitch("fakeOnOffReadOnlySwitch", false, false);
   CFakeSwitch fakeOnOffReadWriteSwitch("fakeOnOffReadWriteSwitch", false, true);
   CFakeSwitch fakeDimmableReadOnlySwitch("fakeDimmableReadOnlySwitch", true, false);
   CFakeSwitch fakeDimmableReadWriteSwitch("fakeDimmableReadWriteSwitch", true, true);
   CFakeController fakeController("fakeController1");
   CFakeConfigurableDevice configurableDevice("configurableDevice");
   CFakeDynamicallyConfigurableDevice dynamicallyConfigurableDevice("dynamicallyConfigurableDevice",
                                                                    shared::CDataContainer());
   CFakeCurtain fakeCurtain("fakeCurtain");


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
   api->declareDevice(dynamicallyConfigurableDevice.getDeviceName(),
                      dynamicallyConfigurableDevice.getType(),
                      dynamicallyConfigurableDevice.getModel(),
                      dynamicallyConfigurableDevice.historizers());
   fakeCurtain.declareDevice(api);

   // Timer used to send fake sensor states periodically
   api->getEventHandler().createTimer(kSendSensorsStateTimerEventId,
                                      shared::event::CEventTimer::kPeriodic,
                                      boost::posix_time::seconds(10));

   api->setPluginState(yApi::historization::EPluginState::kRunning);

   //Retrieve previously created devices and instance them in plugin
   for (const auto& devName : api->getAllDevices())
   {
      if (devName != fakeSensor1.getDeviceName() &&
         devName != fakeSensor2.getDeviceName() &&
         devName != fakeCounter.getDeviceName() &&
         devName != fakeOnOffReadOnlySwitch.getDeviceName() &&
         devName != fakeOnOffReadWriteSwitch.getDeviceName() &&
         devName != fakeDimmableReadOnlySwitch.getDeviceName() &&
         devName != fakeDimmableReadWriteSwitch.getDeviceName() &&
         devName != fakeController.getDeviceName() &&
         devName != configurableDevice.getDeviceName() &&
         devName != fakeCurtain.getDeviceName())
      {
         auto type = api->getDeviceType(devName);
         if (type == CFakeAnotherConfigurableDevice::getType())
         {
            auto createdDevice = boost::make_shared<CFakeAnotherConfigurableDevice>(devName,
                                                                                    api->getDeviceConfiguration(devName));
            m_manuallyCreatedStaticConfDevices.push_back(createdDevice);
         }
         else if (type == CFakeDynamicallyConfigurableDevice::getType())
         {
            auto createdDevice = boost::make_shared<CFakeDynamicallyConfigurableDevice>(devName,
                                                                                        api->getDeviceConfiguration(devName));
            m_manuallyCreatedDynaConfDevices.push_back(createdDevice);
         }
      }
   }

   // the main loop
   while (true)
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
               YADOMS_LOG(information) << "Extra command received : " << extraQuery->getData()->query();

               if (extraQuery->getData()->query() == "simpleCommand")
               {
                  YADOMS_LOG(information) << "Simple command received";
               }
               else if (extraQuery->getData()->query() == "dataCommand")
               {
                  auto s = extraQuery->getData()->data().get<std::string>("testValue");
                  YADOMS_LOG(information) << "Command with data received : data=" << s;
               }
               else if (extraQuery->getData()->query() == "dataBindingCommand")
               {
                  auto value = extraQuery->getData()->data().get<std::string>("networkInterface");
                  YADOMS_LOG(information) << "Command with binded data received : value=" << value << " text=" << Poco::Net::NetworkInterface::forName(value).displayName();
               }
               else if (extraQuery->getData()->query() == "dataBindingPluginCommand")
               {
                  auto interval = extraQuery->getData()->data().get<std::string>("dynamicSection.content.interval");
                  YADOMS_LOG(information) << "Command with plugin binded data received : value=" << interval;
               }
               else if (extraQuery->getData()->query() == "changePluginStateMessage")
               {
                  auto message = extraQuery->getData()->data().get<std::string>("newStateMessage");
                  api->setPluginState(shared::plugin::yPluginApi::historization::EPluginState::kCustom, "newCustomStateMessage", {{"messageFromExtraQuery", message}});
               }
               else if (extraQuery->getData()->query() == "asyncEQwithProgression")
               {
                  auto fileFromClient = extraQuery->getData()->data().get<yApi::configuration::CFile>("fileContent");
                  auto firmwareContent = fileFromClient.getContent();

                  YADOMS_LOG(information) << "File received from extra command";
                  YADOMS_LOG(information) << "    File name = " << fileFromClient.getFileName();
                  YADOMS_LOG(information) << "    File size = " << fileFromClient.getSize();
                  YADOMS_LOG(information) << "    File type = " << fileFromClient.getMimeType();
                  YADOMS_LOG(information) << "    File date = " << fileFromClient.getLastModificationDate().getBoostDateTime();
                  YADOMS_LOG(information) << "    content = " << fileFromClient.getContent();

                  for (int i = 0; i < 100; ++i)
                  {
                     if (i < 25)
                        extraQuery->reportProgress(i * 1.0f, "customLabels.asyncEQwithProgression.step1");
                     else if (i < 50)
                        extraQuery->reportProgress(i * 1.0f, "customLabels.asyncEQwithProgression.step2");
                     else if (i < 75)
                        extraQuery->reportProgress(i * 1.0f, "customLabels.asyncEQwithProgression.step3");
                     else
                        extraQuery->reportProgress(i * 1.0f, "customLabels.asyncEQwithProgression.step4");
                     boost::this_thread::sleep(boost::posix_time::milliseconds(200));
                  }
               }

               // Extra-query can return success or error indicator. In case of success, can also return data.
               // Return here a success without data (=empty container)
               extraQuery->sendSuccess(shared::CDataContainer());
            }
            else
            {
               extraQuery->sendError("error content");
            }


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
            YADOMS_LOG(information) << "Standard command to a keyword (used to drive a switch, a thermostat...)";
            break;
         }

      case yApi::IYPluginApi::kEventManuallyDeviceCreation:
         {
            // A new device was manually created. Instance it in the plugin, and declare its keywords.
            auto creation = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
            try
            {
               YADOMS_LOG(debug) << "Receive event kEventManuallyDeviceCreation : \n"
                  << "\tDevice name : " << creation->getData().getDeviceName();
               creation->getData().getConfiguration().printToLog(YADOMS_LOG(debug));

               if (creation->getData().getDeviceType() == CFakeConfigurableDevice::getType())
               {
                  YADOMS_LOG(error) << "CFakeConfigurableDevice can not be manually created";
                  creation->sendError("CFakeConfigurableDevice can not be manually created");
               }
               else if (creation->getData().getDeviceType() == CFakeAnotherConfigurableDevice::getType())
               {
                  YADOMS_LOG(information) << "CFakeConfigurableDevice config : CounterDivider2 : " << creation->getData().getConfiguration().get<std::string>("CounterDivider2");
                  YADOMS_LOG(information) << "CFakeConfigurableDevice config : MySection/SubIntParameter : " << creation->getData().getConfiguration().get<int>("MySection.content.SubIntParameter");
                  YADOMS_LOG(information) << "CFakeConfigurableDevice config : MySection/SubStringParameter : " << creation->getData().getConfiguration().get<std::string>("MySection.content.SubStringParameter");

                  auto createdDevice = boost::make_shared<CFakeAnotherConfigurableDevice>(creation->getData().getDeviceName(),
                                                                                          creation->getData().getConfiguration());
                  api->declareKeywords(creation->getData().getDeviceName(),
                                       createdDevice->historizers());

                  m_manuallyCreatedStaticConfDevices.push_back(createdDevice);
                  creation->sendSuccess();
               }
               else if (creation->getData().getDeviceType() == CFakeDynamicallyConfigurableDevice::getType())
               {
                  YADOMS_LOG(information) << "CFakeDynamicallyConfigurableDevice config : CounterDivider : " << creation->getData().getConfiguration().get<std::string>("CounterDivider");

                  auto createdDevice = boost::make_shared<CFakeDynamicallyConfigurableDevice>(creation->getData().getDeviceName(),
                                                                                              creation->getData().getConfiguration());
                  api->declareKeywords(creation->getData().getDeviceName(),
                                       createdDevice->historizers());

                  m_manuallyCreatedDynaConfDevices.push_back(createdDevice);
                  creation->sendSuccess();
               }
               else
               {
                  //any other model (don't need a local instance)
                  creation->getData().getConfiguration().printToLog(YADOMS_LOG(information));
                  api->declareKeyword(creation->getData().getDeviceName(),
                                      boost::make_shared<yApi::historization::CSwitch>("manualSwitch"));

                  creation->sendSuccess();
               }
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

            if (deviceConfigurationSchemaRequest->device() == dynamicallyConfigurableDevice.getDeviceName())
            {
               YADOMS_LOG(information) << "This device is dynamically configurable, return its configuration schema to device configuration schema request";
               deviceConfigurationSchemaRequest->sendSuccess(dynamicallyConfigurableDevice.getDynamicConfigurationSchema());
            }
            else
            {
               auto mcds = std::find_if(m_manuallyCreatedDynaConfDevices.begin(), m_manuallyCreatedDynaConfDevices.end(), [deviceConfigurationSchemaRequest](boost::shared_ptr<CFakeDynamicallyConfigurableDevice> item)
                                        {
                                           return item->getDeviceName() == deviceConfigurationSchemaRequest->device();
                                        });
               if (mcds != m_manuallyCreatedDynaConfDevices.end())
               {
                  YADOMS_LOG(information) << "This device is dynamically configurable, return its configuration schema to device configuration schema request";
                  deviceConfigurationSchemaRequest->sendSuccess((*mcds)->getDynamicConfigurationSchema());
               }
               else
               {
                  YADOMS_LOG(information) << "This device is not dynamically configurable, return an empty schema to device configuration schema request";
                  deviceConfigurationSchemaRequest->sendSuccess(shared::CDataContainer());
               }
            }

            break;
         }

      case yApi::IYPluginApi::kSetDeviceConfiguration:
         {
            // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
            auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration>>();

            if (deviceConfiguration->name() == configurableDevice.getDeviceName())
            {
               configurableDevice.setConfiguration(deviceConfiguration->configuration());
            }
            else if (deviceConfiguration->name() == dynamicallyConfigurableDevice.getDeviceName())
            {
               dynamicallyConfigurableDevice.setConfiguration(deviceConfiguration->configuration());
            }
            else
            {
               //search in manually created devices
               auto mcds = std::find_if(m_manuallyCreatedDynaConfDevices.begin(), m_manuallyCreatedDynaConfDevices.end(), [deviceConfiguration](boost::shared_ptr<CFakeDynamicallyConfigurableDevice> const& item)
                                        {
                                           return item->getDeviceName() == deviceConfiguration->name();
                                        });
               if (mcds != m_manuallyCreatedDynaConfDevices.end())
               {
                  (*mcds)->setConfiguration(deviceConfiguration->configuration());
               }
               else
               {
                  auto mcss = std::find_if(m_manuallyCreatedStaticConfDevices.begin(), m_manuallyCreatedStaticConfDevices.end(), [deviceConfiguration](boost::shared_ptr<CFakeAnotherConfigurableDevice> const& item)
                                           {
                                              return item->getDeviceName() == deviceConfiguration->name();
                                           });
                  if (mcss != m_manuallyCreatedStaticConfDevices.end())
                  {
                     (*mcss)->setConfiguration(deviceConfiguration->configuration());
                  }
                  else
                  {
                     auto errorMessage = "Try to apply a device configuration to an unconfigurable device";
                     YADOMS_LOG(error) << errorMessage;
                  }
               }
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
            fakeCurtain.read();

            YADOMS_LOG(information) << "Send the periodically sensors state...";
            fakeSensor1.historizeData(api);
            fakeSensor2.historizeData(api);
            fakeCounter.historizeData(api);
            fakeOnOffReadOnlySwitch.historizeData(api);
            fakeDimmableReadOnlySwitch.historizeData(api);
            fakeController.historizeData(api);
            configurableDevice.historizeData(api);
            fakeCurtain.historizeData(api);

            for (const auto& device : m_manuallyCreatedStaticConfDevices)
            {
               device->read();
               device->historizeData(api);
            }
            for (const auto& device : m_manuallyCreatedDynaConfDevices)
            {
               device->read();
               device->historizeData(api);
            }

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

