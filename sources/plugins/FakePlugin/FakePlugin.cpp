#include "stdafx.h"
#include "FakePlugin.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include "FakeSensor.h"
#include "FakeCounter.h"
#include "FakeSwitch.h"
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/tools/Random.h>
#include <boost/random/independent_bits.hpp>

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
   kSendSensorsStateTimerEventId = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from yApi::IYPluginApi::kPluginFirstEventId
};

void CFakePlugin::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
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
      CFakeCounter fakeCounter("fakeCounter");
      CFakeSwitch fakeOnOffReadOnlySwitch("fakeOnOffReadOnlySwitch", false, false);
      CFakeSwitch fakeOnOffReadWriteSwitch("fakeOnOffReadWriteSwitch", false, true);
      CFakeSwitch fakeDimmableReadOnlySwitch("fakeDimmableReadOnlySwitch", true, false);
      CFakeSwitch fakeDimmableReadWriteSwitch("fakeDimmableReadWriteSwitch", true, true);
      
      // Declare these sensors to Yadoms (devices and associated keywords)
      if (!context->deviceExists(fakeSensor1.getDeviceName()))
      {
         context->declareDevice(fakeSensor1.getDeviceName(), fakeSensor1.getModel());
         fakeSensor1.declareKeywords(context);
      }
      if (!context->deviceExists(fakeSensor2.getDeviceName()))
      {
         context->declareDevice(fakeSensor2.getDeviceName(), fakeSensor2.getModel());
         fakeSensor2.declareKeywords(context);
      }
      if (!context->deviceExists(fakeCounter.getDeviceName()))
      {
         context->declareDevice(fakeCounter.getDeviceName(), fakeCounter.getModel());
         fakeCounter.declareKeywords(context);
      }
      if (!context->deviceExists(fakeOnOffReadOnlySwitch.getDeviceName()))
      {
         context->declareDevice(fakeOnOffReadOnlySwitch.getDeviceName(), fakeOnOffReadOnlySwitch.getModel());
         fakeOnOffReadOnlySwitch.declareKeywords(context);
      }
      if (!context->deviceExists(fakeOnOffReadWriteSwitch.getDeviceName()))
      {
         context->declareDevice(fakeOnOffReadWriteSwitch.getDeviceName(), fakeOnOffReadWriteSwitch.getModel());
         fakeOnOffReadWriteSwitch.declareKeywords(context);
      }
      if (!context->deviceExists(fakeDimmableReadOnlySwitch.getDeviceName()))
      {
         context->declareDevice(fakeDimmableReadOnlySwitch.getDeviceName(), fakeDimmableReadOnlySwitch.getModel());
         fakeDimmableReadOnlySwitch.declareKeywords(context);
      }
      if (!context->deviceExists(fakeDimmableReadWriteSwitch.getDeviceName()))
      {
         context->declareDevice(fakeDimmableReadWriteSwitch.getDeviceName(), fakeDimmableReadWriteSwitch.getModel());
         fakeDimmableReadWriteSwitch.declareKeywords(context);
      }

      // Timer used to send fake sensor states periodically
      context->getEventHandler().createTimer(kSendSensorsStateTimerEventId, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));

      // the main loop
      YADOMS_LOG(debug) << "CFakePlugin is running...";
      context->recordPluginEvent(yApi::IYPluginApi::kInfo, "started");
      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // A command was received from Yadoms
               boost::shared_ptr<const yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
               YADOMS_LOG(debug) << "Command received from Yadoms :" << command->toString();
               break;
            }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               shared::CDataContainer newConfiguration = context->getEventHandler().getEventData<shared::CDataContainer>();
               YADOMS_LOG(debug) << "configuration was updated...";
               context->recordPluginEvent(yApi::IYPluginApi::kInfo, "Configuration was updated");
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
         case kSendSensorsStateTimerEventId:
            {
               // Timer used here to send sensors state to Yadoms periodically

               // Read sensor value and send data to Yadoms (temperatures, battery level, Rssi measure...)
               fakeSensor1.read();
               fakeSensor2.read();
               fakeCounter.read();
               fakeOnOffReadOnlySwitch.read();
               fakeDimmableReadOnlySwitch.read();

               YADOMS_LOG(debug) << "Send the periodically sensors state...";
               fakeSensor1.historizeData(context);
               fakeSensor2.historizeData(context);
               fakeCounter.historizeData(context);
               fakeOnOffReadOnlySwitch.historizeData(context);
               fakeDimmableReadOnlySwitch.historizeData(context);

               break;
            }

         case yApi::IYPluginApi::kEventManuallyDeviceCreation:
         {
            boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> data = context->getEventHandler().getEventData< boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> >();
            try
            {
               // Yadoms asks for device creation
               std::string sni = data->getData().getConfiguration().get<std::string>("networkInterface");
               std::string dyn = data->getData().getConfiguration().get<std::string>("dynamicSection.content.interval");

               std::string devId = (boost::format("%1%_%2%_0x%3$08X") % sni % dyn % shared::tools::CRandom::generateRandomNumber<26>(false)).str();
               context->declareDevice(devId, "FakeDevice_" + devId, data->getData().getConfiguration().serialize());

               yApi::historization::CSwitch manualSwitch("manualSwitch");
               context->declareKeyword(devId, manualSwitch);

               data->sendSuccess(devId);

            }
            catch (std::exception &ex)
            {
               data->sendError(ex.what());
            }
            break;
         }

         case yApi::IYPluginApi::kBindingQuery:
         {
            // Yadoms ask for a binding query 
            boost::shared_ptr<yApi::IBindingQueryRequest> data = context->getEventHandler().getEventData< boost::shared_ptr<yApi::IBindingQueryRequest> >();
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
         default:
            {
               YADOMS_LOG(error) << "Unknown ou unsupported message id " << context->getEventHandler().getEventId();
               break;
            }
         }
      }
   }
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(info) << "Thread is stopping...";
   }

   context->recordPluginEvent(yApi::IYPluginApi::kInfo, "stopped");
}