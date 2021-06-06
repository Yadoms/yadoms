#include "stdafx.h"
#include "PhilipsHue.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include "shared/http/ssdp/DiscoverService.h"
#include "Factory.h"

IMPLEMENT_PLUGIN(CPhilipsHue)


const std::string CPhilipsHue::PhilipsHueBridgeName("Philips hue");
const std::string CPhilipsHue::LightState("STATE");
const std::string CPhilipsHue::RgbColor("RgbColor");


CPhilipsHue::CPhilipsHue()
   : m_switch(boost::make_shared<yApi::historization::CSwitch>(LightState)),
     m_rgb(boost::make_shared<yApi::historization::CColorRGB>(
        RgbColor, shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)),
     m_historizers({m_switch, m_rgb})
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
   YADOMS_LOG(information) << "CPhilipsHue is starting...";
   m_configuration.initializeWith(m_api->getConfiguration());

   init();

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

            init();

            m_configuration.trace();

            break;
         }
      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            const auto command = m_api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
            YADOMS_LOG(information) << "Command received from Yadoms : " << yApi::IDeviceCommand::toString(command);

            auto lightName = command->getDevice();

            const auto deviceDetails = m_api->getDeviceDetails(lightName);
            const auto bridgeId = std::stoi(deviceDetails->get<std::string>("bridgeId"));

            const auto lightId = getLightId(lightName);
            const auto detectedLightId = lightId - 1;
            if (command->getKeyword() == LightState)
            {
               if (command->getBody() == "1")
               {
                  m_detectedLights[detectedLightId]->lightOn();
                  historizeLightState(lightName, true);
               }
               else
               {
                  m_detectedLights[detectedLightId]->lightOff();
                  historizeLightState(lightName, false);
               }
            }
            else if (command->getKeyword() == RgbColor)
            {
               m_detectedLights[detectedLightId]->setLightColorUsingXy(command->getBody());
            }

            break;
         }
      case kCustomEvent:
         {
            break;
         }
      case kEvtKeyStateReceived:
         {
            YADOMS_LOG(information) << "key bridge is pressed";

            closeReadingBridgeButtonState();
            declareLights();
            declareDeviceByBrdige();

            m_api->setPluginState(yApi::historization::EPluginState::kRunning);

            break;
         }
      case kEvtKeyStateTimeout:
         {
            YADOMS_LOG(information) << "key bridge : Timeout";
            closeReadingBridgeButtonState();
            m_api->setPluginState(yApi::historization::EPluginState::kCustom, "askToPressBridgeButtonTimeout");

            break;
         }
      case yApi::IYPluginApi::kEventExtraQuery:
         {
            auto extraQuery = m_api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery>>();

            if (!extraQuery)
            {
               extraQuery->sendError("error content");
            }
            else
            {
               YADOMS_LOG(information) << "Extra command received : " << extraQuery->getData()->query();
               if (extraQuery->getData()->query() == "searchForNewLights")
               {
                  YADOMS_LOG(information) << "search For New Lights command received";
                  try
                  {
                     m_lightsService->searchForNewLights();
                     declareNewLights();
                  }
                  catch (std::exception& exception)
                  {
                     YADOMS_LOG(error) << "start reading bridge button state :" << exception.what();
                     extraQuery->sendError(exception.what());
                     throw;
                  }
                  declareDeviceByBrdige();
                  extraQuery->sendSuccess(shared::CDataContainer::make());
               }
               else if (extraQuery->getData()->query() == "searchForBridge")
               {
                  init();
               }
            }
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
   if (m_configuration.getPairingMode() == kAuto)
   {
      try
      {
         fillHuesInformations();
         m_api->setPluginState(yApi::historization::EPluginState::kCustom, "askToPressBridgeButton");
         startReadingBridgeButtonState();
      }
      catch (const std::exception& exception)
      {
         m_api->setPluginState(yApi::historization::EPluginState::kCustom, "BridgesNotFound");
         const auto message = "cannot found hue bridge :";
         YADOMS_LOG(error) << message << exception.what();
         throw std::runtime_error(message);
      }
   }
   else
   {
      try
      {
         m_urlsManager.push_back(boost::make_shared<CUrlManager>(m_configuration));

         m_hueBridgeDiscovery = CFactory::createHueBridgeDiscovery(m_urlsManager[0]);

         m_hueService = CFactory::createHueService(m_api->getEventHandler(),
                                                   kEvtKeyStateReceived,
                                                   kEvtKeyStateTimeout,
                                                   m_urlsManager[0]);

         m_bridges.push_back(m_hueBridgeDiscovery->getHueInformations());

         m_api->setPluginState(yApi::historization::EPluginState::kCustom, "askToPressBridgeButton");
         m_hueService->startReadingBridgeButtonState();
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
}

void CPhilipsHue::fillHuesInformations()
{
   m_hueBridgeDiscovery = CFactory::createHueBridgeDiscovery();
   try
   {
      m_api->setPluginState(yApi::historization::EPluginState::kCustom, "BridgesSearchInProgress");
      m_bridges = m_hueBridgeDiscovery->FindBridges();

      m_api->setPluginState(yApi::historization::EPluginState::kCustom, "BridgesFound");
   }
   catch (const std::exception& exception)
   {
      m_api->setPluginState(yApi::historization::EPluginState::kCustom, "BridgesNotFound");
      const auto message = "cannot found hue bridge :";
      YADOMS_LOG(error) << message << exception.what();
      throw std::runtime_error(message);
   }
}

void CPhilipsHue::startReadingBridgeButtonState()
{
   try
   {
      for (auto i = 0; i < m_bridges.size(); i++)
      {
         m_urlsManager.push_back(boost::make_shared<CUrlManager>(m_bridges[i], m_configuration));

         m_huesService.push_back(CFactory::createHueService(m_api->getEventHandler(),
                                                            kEvtKeyStateReceived,
                                                            kEvtKeyStateTimeout,
                                                            m_urlsManager[i]));
         m_huesService[i]->startReadingBridgeButtonState();
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

void CPhilipsHue::closeReadingBridgeButtonState()
{
   if (m_configuration.getPairingMode() == EPairingMode::kAuto)
   {
      for (auto i = 0; i < m_bridges.size(); i++)
      {
         m_huesService[i]->closeReadingBridgeButtonState();
      }
   }
   else
   {
      m_hueService->closeReadingBridgeButtonState();
   }
}

void CPhilipsHue::declareDeviceByBrdige()
{
   auto keywordsToHistorize = m_historizers;
   for (auto i = 0; i < m_bridges.size(); i++)
   {
      std::map<std::string, std::string> bridgeId;
      bridgeId.insert(std::pair<std::string, std::string>("bridgeId", std::to_string(i)));

      for (auto& light : m_detectedLights)
      {
         auto lightName = light->getName();
         YADOMS_LOG(information) << "Creating the device :" << lightName;
         if (!m_api->deviceExists(lightName))
         {
            m_api->declareDevice(lightName,
                                 light->getType(),
                                 light->getModelId(),
                                 light->getHistorizables(),
                                 shared::CDataContainer::make(bridgeId));
            historizeLightState(lightName, light->getState());
         }
      }
   }
}

void CPhilipsHue::declareLights()
{
   for (auto i = 0; i < m_bridges.size(); i++)
   {
      m_lightManagers.push_back(CFactory::createLightsService(m_urlsManager[i]));

      for (auto& light : m_lightManagers[i]->getAllLights())
      {
         auto lightPair = std::make_pair(light.first, light.second);
         auto detectedLight = CFactory::createLight(m_urlsManager[i], lightPair);
         m_detectedLights.push_back(detectedLight);
      }
   }
}

void CPhilipsHue::declareNewLights()
{
   for (auto i = 0; i < m_bridges.size(); i++)
   {
      m_lightManagers.push_back(CFactory::createLightsService(m_urlsManager[i]));

      for (auto& light : m_lightsService->getNewLights())
      {
         auto lightPair = std::make_pair(light.first, light.second);
         auto detectedLight = CFactory::createLight(m_urlsManager[i], lightPair);
         m_detectedLights.push_back(detectedLight);
      }
   }
}

int CPhilipsHue::getLightId(std::string& lightName)
{
   const auto it = std::find_if(std::begin(m_detectedLights), std::end(m_detectedLights),
                                [&lightName](auto&& pair)
                                {
                                   return pair->getName() == lightName;
                                });

   if (it == std::end(m_detectedLights))
   {
      YADOMS_LOG(warning) << "Light not found";
      throw std::runtime_error("Light ID is not found");
   }
   YADOMS_LOG(information) << "Light ID = " << it->get()->getDeviceId() << " is found ";

   return it->get()->getDeviceId();
}

void CPhilipsHue::historizeLightState(std::string& lightName, bool state)
{
   m_switch->set(state);
   m_api->historizeData(lightName, m_switch);
}
