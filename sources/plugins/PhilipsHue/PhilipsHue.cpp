#include "stdafx.h"
#include "PhilipsHue.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include "shared/http/ssdp/DiscoverService.h"


IMPLEMENT_PLUGIN(CPhilipsHue)


const std::string CPhilipsHue::PhilipsHueBridgeName("Philips hue");

CPhilipsHue::CPhilipsHue()
   : m_HueInformations(boost::make_shared<std::vector<CHueBridgeDiscovery::HueInformations>>())
{
}

CPhilipsHue::~CPhilipsHue()
{
}

enum
{
   kCustomEvent = yApi::IYPluginApi::kPluginFirstEventId,
};

void CPhilipsHue::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   m_api = api;
   // Informs Yadoms about the plugin actual state
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   YADOMS_LOG(information) << "CPhilipsHue is starting...";

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());


   api->setPluginState(yApi::historization::EPluginState::kRunning);

   fillHueInformations();
   // the main loop
   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested";
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }

      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            // Configuration was updated
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            const auto newConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<shared::CDataContainer>
            >();
            YADOMS_LOG(information) << "Update configuration...";

            m_configuration.initializeWith(newConfiguration);

            // Trace the configuration
            m_configuration.trace();

            api->setPluginState(yApi::historization::EPluginState::kRunning);

            break;
         }

      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            const auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
            YADOMS_LOG(information) << "Command received from Yadoms : " << yApi::IDeviceCommand::toString(command);

            break;
         }

      case kCustomEvent:
         {
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

void CPhilipsHue::fillHueInformations() const
{
   try
   {
      auto foundBridges = CHueBridgeDiscovery::FindBridges();
      for (const auto& foundBridge : foundBridges)
      {
         m_HueInformations->push_back(foundBridge);
      }
   }
   catch (const std::exception& exception)
   {
      YADOMS_LOG(error) << "cannot found hue bridge :" << exception.what();
      throw;
   }
}
