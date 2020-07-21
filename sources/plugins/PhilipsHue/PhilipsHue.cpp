#include "stdafx.h"
#include "PhilipsHue.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include "shared/http/ssdp/DiscoverService.h"
#include "Factory.h"

IMPLEMENT_PLUGIN(CPhilipsHue)


const std::string CPhilipsHue::PhilipsHueBridgeName("Philips hue");

CPhilipsHue::CPhilipsHue()
{
}

CPhilipsHue::~CPhilipsHue()
{
}

enum
{
   kCustomEvent = yApi::IYPluginApi::kPluginFirstEventId,
   kEvtKeyStateReceived,
   kEvtKeyStateTimeout
};

void CPhilipsHue::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   m_api = api;
   // Informs Yadoms about the plugin actual state
   m_api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   YADOMS_LOG(information) << "CPhilipsHue is starting...";

   // Load configuration values (provided by database)
   m_configuration.initializeWith(m_api->getConfiguration());

   //m_api->setPluginState(yApi::historization::EPluginState::kRunning);
   m_api->setPluginState(yApi::historization::EPluginState::kCustom, "askToPressBridgeButton");
   init();
   // the main loop
   while (true)
   {
      // Wait for an event
      switch (m_api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested";
            m_api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }

      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            // Configuration was updated
            m_api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            const auto newConfiguration = m_api
                                          ->getEventHandler().getEventData<boost::shared_ptr<shared::CDataContainer>
                                          >();
            YADOMS_LOG(information) << "Update configuration...";

            m_configuration.initializeWith(newConfiguration);

            // Trace the configuration
            m_configuration.trace();

            m_api->setPluginState(yApi::historization::EPluginState::kRunning);

            break;
         }
      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            const auto command = m_api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
            YADOMS_LOG(information) << "Command received from Yadoms : " << yApi::IDeviceCommand::toString(command);

            break;
         }
      case kCustomEvent:
         {
            break;
         }
      case kEvtKeyStateReceived:
         {
            YADOMS_LOG(information) << "key bridge is pressed";
            m_hueService->closeReadingBridgeButtonState();
            break;
         }
      case kEvtKeyStateTimeout:
         {
            YADOMS_LOG(information) << "key bridge : Timeout";
            m_api->setPluginState(yApi::historization::EPluginState::kCustom, "askToPressBridgeButtonTimeout");
            m_hueService->closeReadingBridgeButtonState();
            break;
         }
      default:
         {
            YADOMS_LOG(error) << "Unknown or unsupported message id " << m_api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

void CPhilipsHue::init()
{
   fillHueInformations();
   m_api->setPluginState(yApi::historization::EPluginState::kCustom, "askToPressBridgeButton");
  
   startReadingBridgeButtonState();
}

void CPhilipsHue::fillHueInformations()
{
   try
   {
      auto foundBridges = CHueBridgeDiscovery::FindBridges();
      for (auto& foundBridge : foundBridges)
      {
         m_HueInformations.push_back(foundBridge);
      }
   }
   catch (const std::exception& exception)
   {
      YADOMS_LOG(error) << "cannot found hue bridge :" << exception.what();
      throw;
   }
}

void CPhilipsHue::startReadingBridgeButtonState()
{
   try
   {
      for (auto& foundBridge : m_HueInformations)
      {
         m_urlManager = boost::make_shared<CUrlManager>(foundBridge, m_configuration);
         m_hueService = CFactory::createHueService(m_api->getEventHandler(),
                                                   foundBridge,
                                                   kEvtKeyStateReceived,
                                                   kEvtKeyStateTimeout,
                                                   m_urlManager);

         m_hueService->startReadingBridgeButtonState();
      }
   }
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(error) << "start reading bridge button state thread is interrupted";
   }
   catch (const std::exception& exception)
   {
      YADOMS_LOG(error) << "start reading bridge button state :" << exception.what();
      throw;
   }
   catch (...)
   {
      YADOMS_LOG(error) << "start reading bridge button state : unknown error";
   }
}
