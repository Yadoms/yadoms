#include "stdafx.h"
#include "Lametric.h"
#include <plugin_cpp_api/ImplementationHelper.h>

#include <Poco/Net/HTTPSClientSession.h>
#include <shared/plugin/yPluginApi/historization/MessageFormatter.h>
#include <shared/Log.h>
#include <shared/http/HttpMethods.h>
#include <shared/encryption/Base64.h>

/* ----------------------------------

Insert here all include files

   ---------------------------------- */


// Use this macro to define all necessary to make your plugin a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
IMPLEMENT_PLUGIN(CLametric)


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
   api->setPluginState(yApi::historization::EPluginState::kRunning);

   getWifiState();

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

            /*
   
            Process the command here (to drive a keyword for example)
   
            */

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

void CLametric::getWifiState() const
{
   try
   {
      shared::CDataContainer response;
      shared::CDataContainer headerParameters;
      shared::CDataContainer parameters;

      //format url
      auto uriStr = "http://" + m_configuration.getIPAddress() + ":" + m_configuration.getPort() +
         "/api/v2/device/wifi";

      std::string authorizationHeader = "dev:" + m_configuration.getAPIKey();

      headerParameters.set("Authorization",
                           "Basic " + shared::encryption::CBase64::encode(
                              reinterpret_cast<const unsigned char*>(authorizationHeader.c_str()),
                              authorizationHeader.length()));

      headerParameters.set("User-Agent", "yadoms");
      headerParameters.set("Accept", "application/json");
      headerParameters.set("Connection", "close");

      response = shared::CHttpMethods::sendGetRequest(uriStr,
                                                      headerParameters);

      bool isWifiavailable = response.get<bool>("available");
      std::cout << isWifiavailable << std::endl;
   }
   catch (Poco::Exception& ex)
   {
      YADOMS_LOG(error) << "Error " << ex.displayText();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Error " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Unknown exception in sendSms";
   }
}
