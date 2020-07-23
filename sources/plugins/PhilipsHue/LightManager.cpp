#include "LightManager.h"
#include "shared/Log.h"
#include "shared/http/HttpMethods.h"

CLightManager::CLightManager(boost::shared_ptr<CUrlManager>& urlManager)
   : m_urlManager(urlManager)
{
}

std::vector<HueLightInformations> CLightManager::getAllLights()
{
   HueLightInformations hueLightInformations;
   std::vector<HueLightInformations> hueLightsInformations;
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kGetAllLights);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);
   try
   {
      const auto response = shared::http::CHttpMethods::sendJsonGetRequest(lightUrl);
      auto lightCounter = 1;;
      while (response->exists(std::to_string(lightCounter)))
      {
         hueLightInformations.state.on = response->get<bool>(std::to_string(lightCounter) + ".state.on");
         hueLightInformations.state.bri = response->get<int>(std::to_string(lightCounter) + ".state.bri");
         hueLightInformations.state.hue = response->get<int>(std::to_string(lightCounter) + ".state.hue");
         hueLightInformations.state.sat = response->get<int>(std::to_string(lightCounter) + ".state.sat");
         hueLightInformations.state.effect = response->get<std::string>(std::to_string(lightCounter) + ".state.effect");
         hueLightInformations.state.xy.x = response->get<float>(std::to_string(lightCounter) + ".state.xy.0");
         hueLightInformations.state.xy.y = response->get<float>(std::to_string(lightCounter) + ".state.xy.1");
         hueLightInformations.state.ct = response->get<int>(std::to_string(lightCounter) + ".state.ct");
         hueLightInformations.state.alert = response->get<std::string>(std::to_string(lightCounter) + ".state.alert");
         hueLightInformations.state.colormode = response->get<std::string>(
            std::to_string(lightCounter) + ".state.colormode");
         hueLightInformations.state.mode = response->get<std::string>(std::to_string(lightCounter) + ".state.mode");
         hueLightInformations.state.reachable = response->get<bool>(std::to_string(lightCounter) + ".state.reachable");

         hueLightInformations.swupdate.state = response->get<std::string>(
            std::to_string(lightCounter) + ".swupdate.state");
         hueLightInformations.swupdate.lastinstall = response->get<std::string>(
            std::to_string(lightCounter) + ".swupdate.lastinstall");

         hueLightInformations.type = response->get<std::string>(std::to_string(lightCounter) + ".type");
         hueLightInformations.name = response->get<std::string>(std::to_string(lightCounter) + ".name");
         hueLightInformations.modelid = response->get<std::string>(std::to_string(lightCounter) + ".modelid");
         hueLightInformations.manufacturername = response->get<std::string>(
            std::to_string(lightCounter) + ".manufacturername");
         hueLightInformations.productname = response->get<std::string>(std::to_string(lightCounter) + ".productname");

         hueLightInformations.capabilities.certified = response->get<bool>(
            std::to_string(lightCounter) + ".capabilities.certified");
         hueLightInformations.capabilities.control.mindimlevel = response->get<int>(
            std::to_string(lightCounter) + ".capabilities.control.mindimlevel");
         hueLightInformations.capabilities.control.maxlumen = response->get<int>(
            std::to_string(lightCounter) + ".capabilities.control.maxlumen");
         hueLightInformations.capabilities.control.colorgamuttype = response->get<std::string>(
            std::to_string(lightCounter) + ".capabilities.control.colorgamuttype");
         //TODO : fill COLORGAMUT
         hueLightInformations.capabilities.control.ct.min = response->get<int>(
            std::to_string(lightCounter) + ".capabilities.control.ct.min");
         hueLightInformations.capabilities.control.ct.max = response->get<int>(
            std::to_string(lightCounter) + ".capabilities.control.ct.max");

         hueLightInformations.capabilities.streaming.renderer = response->get<bool>(
            std::to_string(lightCounter) + ".capabilities.streaming.renderer");
         hueLightInformations.capabilities.streaming.proxy = response->get<bool>(
            std::to_string(lightCounter) + ".capabilities.streaming.proxy");

         hueLightInformations.config.archetype = response->get<std::string>(
            std::to_string(lightCounter) + ".config.archetype");
         hueLightInformations.config.function = response->get<std::string>(
            std::to_string(lightCounter) + ".config.function");
         hueLightInformations.config.direction = response->get<std::string>(
            std::to_string(lightCounter) + ".config.direction");

         hueLightInformations.uniqueid = response->get<std::string>(std::to_string(lightCounter) + ".uniqueid");
         hueLightInformations.swversion = response->get<std::string>(std::to_string(lightCounter) + ".swversion");

         hueLightsInformations.push_back(hueLightInformations);
         lightCounter++;
      }
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Post http request or interpret answer \"%1%\" : %2%") % lightUrl
         %
         e.what()).str();
      YADOMS_LOG(error) << "Fail to send Post http request or interpret answer " << lightUrl << " : " << e.what();
      throw;
   }
   return hueLightsInformations;
}
