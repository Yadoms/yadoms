#include "stdafx.h"
#include "PhilipsHue.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include "shared/http/ssdp/DiscoverService.h"
#include "Factory.h"
#include "ColorConverter.h"

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
            if (m_configuration.getPairingMode() == kAuto)
            {
               const auto deviceDetails = m_api->getDeviceDetails(lightName);
               const auto bridgeId = std::stoi(deviceDetails->get<std::string>("bridgeId"));

               m_lightManagers[bridgeId]->setLightId(lightName, m_detectedLightsByBridge[bridgeId]);
               if (command->getKeyword() == LightState)
               {
                  if (command->getBody() == "1")
                  {
                     m_lightManagers[bridgeId]->lightOn();
                  }
                  else
                  {
                     m_lightManagers[bridgeId]->lightOff();
                  }
               }
               else if (command->getKeyword() == RgbColor)
               {
                  m_lightManagers[bridgeId]->setLightColorUsingXy(command->getBody());
               }
            }
            else
            {
               m_lightManager->setLightId(lightName, m_detectedLights);
               if (command->getKeyword() == LightState)
               {
                  if (command->getBody() == "1")
                  {
                     m_lightManager->lightOn();
                  }
                  else
                  {
                     m_lightManager->lightOff();
                  }
               }
               else if (command->getKeyword() == RgbColor)
               {
                  m_lightManager->setLightColorUsingXy(command->getBody());
               }
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
            if (m_configuration.getPairingMode() == kAuto)
            {
               for (auto i = 0; i < m_HuesInformations.size(); i++)
               {
                  m_lightManagers.push_back(CFactory::createLightManager(m_urlsManager[i]));
                  m_detectedLightsByBridge.push_back(m_lightManagers[i]->getAllLights());
                  declareDeviceByBrdige();
               }
            }
            else
            {
               m_lightManager = CFactory::createLightManager(m_urlManager);
               m_detectedLights = m_lightManager->getAllLights();
               declareDevice();
            }

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
                     m_lightManager->searchForNewLights();
                     m_detectedLights = m_lightManager->getNewLights();
                  }
                  catch (std::exception& exception)
                  {
                     YADOMS_LOG(error) << "start reading bridge button state :" << exception.what();
                     extraQuery->sendError(exception.what());
                     throw;
                  }
                  declareDevice();
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
         m_urlManager = boost::make_shared<CUrlManager>(m_configuration);
         m_hueBridgeDiscovery = CFactory::createHueBridgeDiscovery(m_urlManager);

         m_hueService = CFactory::createHueService(m_api->getEventHandler(),
                                                   kEvtKeyStateReceived,
                                                   kEvtKeyStateTimeout,
                                                   m_urlManager);

         m_HueInformations = m_hueBridgeDiscovery->getHueInformations();

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
      auto foundBridges = m_hueBridgeDiscovery->FindBridges();
      for (auto& foundBridge : foundBridges)
      {
         m_HuesInformations.push_back(foundBridge);
      }
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
      for (auto i = 0; i < m_HuesInformations.size(); i++)
      {
         m_urlsManager.push_back(boost::make_shared<CUrlManager>(m_HuesInformations[i], m_configuration));

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
      for (auto i = 0; i < m_HuesInformations.size(); i++)
      {
         m_huesService[i]->closeReadingBridgeButtonState();
      }
   }
   else
   {
      m_hueService->closeReadingBridgeButtonState();
   }
}

void CPhilipsHue::declareDevice()
{
   for (auto& detectedLight : m_detectedLights)
   {
      YADOMS_LOG(information) << "Creating the device :" << detectedLight.second.getName();
      if (!m_api->deviceExists(detectedLight.second.getName()))
         m_api->declareDevice(detectedLight.second.getName(),
                              detectedLight.second.getType(),
                              detectedLight.second.getModelId(),
                              m_historizers);
   }
}

void CPhilipsHue::declareDeviceByBrdige()
{
   for (auto i = 0; i < m_detectedLightsByBridge.size(); i++)
   {
      for (auto j = 1; j <= m_detectedLightsByBridge[i].size(); j++)
      {
         std::map<std::string, std::string> bridgeId;
         bridgeId.insert(std::pair<std::string, std::string>("bridgeId", std::to_string(i)));

         YADOMS_LOG(information) << "Creating the device :" << m_detectedLightsByBridge[i].at(j).getName();
         if (!m_api->deviceExists(m_detectedLightsByBridge[i].at(j).getName()))
            m_api->declareDevice(m_detectedLightsByBridge[i].at(j).getName(),
                                 m_detectedLightsByBridge[i].at(j).getType(),
                                 m_detectedLightsByBridge[i].at(j).getModelId(),
                                 m_historizers,
                                 shared::CDataContainer::make(bridgeId));
      }
   }
}