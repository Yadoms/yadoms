#include "stdafx.h"
#include "EmptyPlugin.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>

/* ----------------------------------

Insert here all include files

   ---------------------------------- */


// Use this macro to define all necessary to make your plugin a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
IMPLEMENT_PLUGIN(CEmptyPlugin)


CEmptyPlugin::CEmptyPlugin()
{
}

CEmptyPlugin::~CEmptyPlugin()
{
}

// Event IDs
enum
{
   kSendSensorsStateTimerEventId = yApi::IYPluginApi::kPluginFirstEventId, // Always start from yApi::IYPluginApi::kPluginFirstEventId

   /* ----------------------------------

   Insert here all your events

      ---------------------------------- */
};

void CEmptyPlugin::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Informs Yadoms about the plugin actual state
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   YADOMS_LOG(information) << "CEmptyPlugin is starting...";

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   /* -----------------------------
   
      Create & declare here all your devices and keywords.
      Create timers.

      ----------------------------- */

   api->setPluginState(yApi::historization::EPluginState::kRunning);

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

               /* 

               Treat here the extra query received

               */

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
            auto creation = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
            try {

               /*

               Treat here the manually device creation event (be careful to use declareManuallyCreatedDevice instead of declareDevice for device creation)

               */
               
               std::string devId;
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

            /*

            Treat here the get device configuration request

            */

            break;
         }

      case yApi::IYPluginApi::kSetDeviceConfiguration:
         {
            // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
            auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration>>();

            /*

            Apply here the new configuration to the device name

            */

            break;
         }

      case kSendSensorsStateTimerEventId:
         {
            // Timer used here to read data, ...

            /*

            Read here all information

            */

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

