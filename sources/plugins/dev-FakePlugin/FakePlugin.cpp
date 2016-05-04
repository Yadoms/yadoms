#include "stdafx.h"
#include "FakePlugin.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include "FakeSensor.h"
#include "FakeCounter.h"
#include "FakeSwitch.h"
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/tools/Random.h>
#include "FakeController.h"
#include <Poco/Net/NetworkInterface.h>

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
   kSendSensorsStateTimerEventId = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from yApi::IYPluginApi::kPluginFirstEventId
};

void CFakePlugin::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Informs Yadoms about the plugin actual state
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   YADOMS_LOG(debug) << "CFakePlugin is starting...";

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


   // Declare these sensors to Yadoms (devices and associated keywords) ifn ot already declared
   fakeSensor1.declareDevice(api);
   fakeSensor2.declareDevice(api);
   fakeCounter.declareDevice(api);
   fakeOnOffReadOnlySwitch.declareDevice(api);
   fakeOnOffReadWriteSwitch.declareDevice(api);
   fakeDimmableReadOnlySwitch.declareDevice(api);
   fakeDimmableReadWriteSwitch.declareDevice(api);
   fakeController.declareDevice(api);

   // Timer used to send fake sensor states periodically
   api->getEventHandler().createTimer(kSendSensorsStateTimerEventId, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));

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
         YADOMS_LOG(debug) << "Stop requested";
         api->setPluginState(yApi::historization::EPluginState::kStopped);
         return;
      }
      case yApi::IYPluginApi::kEventDeviceCommand:
      {
         // A command was received from Yadoms
         auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
         YADOMS_LOG(debug) << "Command received from Yadoms :" << command->getDevice() << "." << command->getKeyword() << "=" << command->getBody();
         break;
      }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
      {
         // Configuration was updated
         api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
         auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainer>();
         YADOMS_LOG(debug) << "Update configuration...";

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

         YADOMS_LOG(debug) << "Send the periodically sensors state...";
         fakeSensor1.historizeData(api);
         fakeSensor2.historizeData(api);
         fakeCounter.historizeData(api);
         fakeOnOffReadOnlySwitch.historizeData(api);
         fakeDimmableReadOnlySwitch.historizeData(api);
         fakeController.historizeData(api);

         break;
      }

      case yApi::IYPluginApi::kEventManuallyDeviceCreation:
      {
         boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> creation = api->getEventHandler().getEventData< boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> >();
         try
         {
            // Yadoms asks for device creation
            auto sni = creation->getData().getConfiguration().get<std::string>("networkInterface");
            auto dyn = creation->getData().getConfiguration().get<std::string>("dynamicSection.content.interval");

            auto devId = (boost::format("%1%_%2%_0x%3$08X") % sni % dyn % shared::tools::CRandom::generateNbBits(26, false)).str();
            api->declareDevice(devId, "FakeDevice_" + devId, creation->getData().getConfiguration());

            yApi::historization::CSwitch manualSwitch("manualSwitch");
            api->declareKeyword(devId, manualSwitch);

            creation->sendSuccess(devId);

         }
         catch (std::exception &ex)
         {
            creation->sendError(ex.what());
         }
         break;
      }

      case yApi::IYPluginApi::kBindingQuery:
      {
         // Yadoms ask for a binding query 
         boost::shared_ptr<yApi::IBindingQueryRequest> data = api->getEventHandler().getEventData< boost::shared_ptr<yApi::IBindingQueryRequest> >();
         if (data->getData().getQuery() == "test")
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

            data->sendSuccess(result);
         }
         else
         {
            std::string errorMessage = (boost::format("unknown query : %1%") % data->getData().getQuery()).str();
            data->sendError(errorMessage);
            YADOMS_LOG(error) << errorMessage;
            }
            break;
         }

         case yApi::IYPluginApi::kEventExtraCommand:
         {
            // Command was received from Yadoms
            auto extraCommand = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IExtraCommand> >();

            if (extraCommand)
            {
               YADOMS_LOG(debug) << "Extra command received : " << extraCommand->getCommand();

               if (extraCommand->getCommand() == "simpleCommand")
               {
                  YADOMS_LOG(information) << "Simple command received";
               }
               else if (extraCommand->getCommand() == "dataCommand")
               {
                  std::string s = extraCommand->getData().get<std::string>("testValue");
                  YADOMS_LOG(information) << "Command with data received : data=" << s;
               }
               else if (extraCommand->getCommand() == "dataBindingCommand")
               {
                  std::string value = extraCommand->getData().get<std::string>("networkInterface");
                  YADOMS_LOG(information) << "Command with binded data received : value=" << value << " text=" << Poco::Net::NetworkInterface::forName(value).displayName();
               }
               else if (extraCommand->getCommand() == "dataBindingPluginCommand")
               {
                  std::string interval = extraCommand->getData().get<std::string>("dynamicSection.content.interval");
                  YADOMS_LOG(information) << "Command with plugin binded data received : value=" << interval;
               }
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