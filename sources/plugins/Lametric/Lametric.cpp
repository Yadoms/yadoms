#include "stdafx.h"
#include "Lametric.h"
#include <plugin_cpp_api/ImplementationHelper.h>

#include <Poco/Net/HTTPSClientSession.h>
#include <shared/plugin/yPluginApi/historization/MessageFormatter.h>
#include <shared/Log.h>
#include "shared/http/HttpException.hpp"

/* ----------------------------------

Insert here all include files

   ---------------------------------- */


// Use this macro to define all necessary to make your plugin a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
IMPLEMENT_PLUGIN(CLametric)

const std::string CLametric::DeviceName("Lametric display");
const std::string CLametric::TextKeywordName("Message");

// Event IDs
enum
{
   // Example of adding a custom event
   kCustomEvent = yApi::IYPluginApi::kPluginFirstEventId,
   // Always start from yApi::IYPluginApi::kPluginFirstEventId

   /* ----------------------------------

   Insert here all your events

   ---------------------------------- */
};

CLametric::CLametric()
   : m_text(boost::make_shared<yApi::historization::CText>(TextKeywordName,
                                                           yApi::EKeywordAccessMode::kGetSet))
{
}

void CLametric::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Informs Yadoms about the plugin actual state
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   YADOMS_LOG(information) << "CLametric is starting...";

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   /* -----------------------------

      Create & declare here all your devices and keywords.
      Create timers.

      ----------------------------- */
   m_lametricManager = boost::make_shared<CUrlManager>(m_configuration);
   // TODO : Timer in error case
   DeviceInformation deviceInformation;
   if (isDeviceActive(api))
   {
      api->setPluginState(yApi::historization::EPluginState::kRunning);

      fillDeviceInformation(&deviceInformation);

      declareDevice(api, &deviceInformation);

      declareKeyword(api, &deviceInformation);
   }
   else
   {
      api->setPluginState(yApi::historization::EPluginState::kError);
   }


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
            const auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainer>();
            YADOMS_LOG(information) << "Update configuration...";

            // Take into account the new configuration
            // - Restart the plugin if necessary,
            // - Update some resources,
            // - etc...
            m_configuration.initializeWith(newConfiguration);
            m_lametricManager = boost::make_shared<CUrlManager>(m_configuration);

            // Trace the configuration
            m_configuration.trace();

            api->setPluginState(yApi::historization::EPluginState::kRunning);

            break;
         }

      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // A command was received from Yadoms
            const auto command =
               api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
            YADOMS_LOG(information) << "Command received from Yadoms : " << yApi::IDeviceCommand::toString(command);


            if (boost::iequals(command->getDevice(), deviceInformation.deviceName))
            {
               YADOMS_LOG(information) << "Texte à envoyer : " << command->getBody();
               m_lametricManager->displayText(command->getBody());
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

void CLametric::declareDevice(boost::shared_ptr<yApi::IYPluginApi>& api, DeviceInformation* deviceInformation)
{
   if (!api->deviceExists(deviceInformation->deviceName))
      api->declareDevice(deviceInformation->deviceName, deviceInformation->deviceType, deviceInformation->deviceModel);
}

void CLametric::declareKeyword(boost::shared_ptr<yApi::IYPluginApi>& api, DeviceInformation* deviceInformation) const
{
   if (!api->keywordExists(deviceInformation->deviceName, m_text))
      api->declareKeyword(deviceInformation->deviceName, m_text);
}

void CLametric::fillDeviceInformation(DeviceInformation* deviceInformation) const
{
   deviceInformation->deviceName = "Lametric";
   deviceInformation->deviceModel = m_lametricManager->getDeviceState().get<std::string>("model");
   deviceInformation->deviceType = m_lametricManager->getDeviceState().get<std::string>("name");
}

bool CLametric::isDeviceActive(boost::shared_ptr<yApi::IYPluginApi>& api)
{
   try
   {
      const auto response = m_lametricManager->getWifiState();
      return response.get<bool>("active");;
   }
   catch (std::exception& e)
   {
      // TODO : translate in .json
      api->setPluginState(yApi::historization::EPluginState::kCustom, "cannot reach Lametric device");
      const auto message = (boost::format("Fail to send GET http request \"%1%\"") % e.
         what()).str();
      YADOMS_LOG(error) << message;
   }
   return false;
}
