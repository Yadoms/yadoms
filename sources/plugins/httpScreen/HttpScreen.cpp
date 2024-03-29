#include "stdafx.h"
#include "HttpScreen.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/dateTime/Format.h>

#include "ScreenDevice.h"

/* ----------------------------------

Insert here all include files

   ---------------------------------- */


// Use this macro to define all necessary to make your plugin a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
IMPLEMENT_PLUGIN(CHttpScreen)


CHttpScreen::CHttpScreen()
{
}

CHttpScreen::~CHttpScreen()
{
}

// Event IDs
enum
{
   // Example of adding a custom event
   kCustomEvent = yApi::IYPluginApi::kPluginFirstEventId, // Always start from yApi::IYPluginApi::kPluginFirstEventId

   /* ----------------------------------

   Insert here all your events

   ---------------------------------- */
};

void CHttpScreen::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Informs Yadoms about the plugin actual state
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   YADOMS_LOG(information) << "CHttpScreen is starting...";

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   /* -----------------------------
   
      Create & declare here all your devices and keywords.
      Create timers.

      ----------------------------- */
   m_controller = boost::make_shared<CHttpController>();
   boost::shared_ptr<CScreenDevice> oledScreen = boost::make_shared<CScreenDevice>(m_configuration, m_controller, "httpScreen");
   oledScreen->declareDevice(api);

   api->setPluginState(yApi::historization::EPluginState::kRunning);

   // the main loop
   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            // Yadoms request the plugin to stop. Note that plugin must be stopped in 10 seconds max, otherwise it will be killed.
            YADOMS_LOG(information) << "Stop requested";
   			api->setPluginState(yApi::historization::EPluginState::kStopped);
	   		return;
         }

      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            // Configuration was updated
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            const auto newConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<shared::CDataContainer>>();
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

      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // A command was received from Yadoms
            const auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
            
			if (boost::iequals(command->getDevice(), oledScreen->getDeviceName()))
			{
				const std::string keywordName = command->getKeyword();
				const std::string value = command->getBody();
				oledScreen->updateScreen(keywordName, value);
			}
			else
			{
				YADOMS_LOG(warning) << "Command received for unknown device : " << yApi::IDeviceCommand::toString(command);
			}
            break;
         }

      case kCustomEvent:
         {
            /*

            Process your custom event

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

