#include "LightManager.h"
#include "shared/Log.h"
#include "shared/http/HttpMethods.h"
#include "ColorConverter.h"

CLightManager::CLightManager(boost::shared_ptr<CUrlManager>& urlManager)
   : m_urlManager(urlManager),
     m_lightId(0)
{
}

std::map<int, HueLightInformations> CLightManager::getAllLights()
{
   HueLightInformations hueLightInformations;
   std::map<int, HueLightInformations> hueLightsInformations;
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
         hueLightInformations.swupdate.lastinstall = response->isNull(
                                                        std::to_string(lightCounter) + ".swupdate.lastinstall")
                                                        ? "null"
                                                        : response->get<std::string>(
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

         hueLightsInformations.insert({lightCounter, hueLightInformations});
         lightCounter++;
      }
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Get http request or interpret answer \"%1%\" : %2%") % lightUrl
         %
         e.what()).str();
      YADOMS_LOG(error) << "Fail to send Get http request or interpret answer " << lightUrl << " : " << e.what();
      throw;
   }
   return hueLightsInformations;
}

HueLightInformations CLightManager::getLightAttributesAndState(const int id)
{
   HueLightInformations hueLightAttributesAndState;
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kGetLightAttributesAndState, id);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   try
   {
      auto response = shared::http::CHttpMethods::sendJsonGetRequest(lightUrl);

      setHueLightInformationsState(hueLightAttributesAndState, response);
      setHueLightInformationsSwUpdate(hueLightAttributesAndState, response);

      hueLightAttributesAndState.type = response->get<std::string>("type");
      hueLightAttributesAndState.name = response->get<std::string>("name");
      hueLightAttributesAndState.modelid = response->get<std::string>("modelid");
      hueLightAttributesAndState.manufacturername = response->get<std::string>(
         "manufacturername");
      hueLightAttributesAndState.productname = response->get<std::string>("productname");

      setHueLightInformationsCapabilities(hueLightAttributesAndState, response);
      setHueLightInformationsConfig(hueLightAttributesAndState, response);

      hueLightAttributesAndState.uniqueid = response->get<std::string>("uniqueid");
      hueLightAttributesAndState.swversion = response->get<std::string>("swversion");
      hueLightAttributesAndState.swconfigid = response->get<std::string>("swconfigid");
      hueLightAttributesAndState.productid = response->get<std::string>("productid");
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Get http request or interpret answer \"%1%\" : %2%") % lightUrl
         %
         e.what()).str();
      YADOMS_LOG(error) << "Fail to send Get http request or interpret answer " << lightUrl << " : " << e.what();
      throw;
   }
   return hueLightAttributesAndState;
}

void CLightManager::setHueLightInformationsState(HueLightInformations& hueLightAttributesAndState,
                                                 boost::shared_ptr<shared::CDataContainer>& response)
{
   hueLightAttributesAndState.state.on = response->get<bool>("state.on");
   hueLightAttributesAndState.state.bri = response->get<int>("state.bri");
   hueLightAttributesAndState.state.hue = response->get<int>("state.hue");
   hueLightAttributesAndState.state.sat = response->get<int>("state.sat");
   hueLightAttributesAndState.state.effect = response->get<std::string>("state.effect");
   hueLightAttributesAndState.state.xy.x = response->get<float>("state.xy.0");
   hueLightAttributesAndState.state.xy.y = response->get<float>("state.xy.1");
   hueLightAttributesAndState.state.ct = response->get<int>("state.ct");
   hueLightAttributesAndState.state.alert = response->get<std::string>("state.alert");
   hueLightAttributesAndState.state.colormode = response->get<std::string>(
      "state.colormode");
   hueLightAttributesAndState.state.mode = response->get<std::string>("state.mode");
   hueLightAttributesAndState.state.reachable = response->get<bool>("state.reachable");
}

void CLightManager::setHueLightInformationsSwUpdate(HueLightInformations& hueLightAttributesAndState,
                                                    boost::shared_ptr<shared::CDataContainer>& response)
{
   hueLightAttributesAndState.swupdate.state = response->get<std::string>(
      "swupdate.state");
   hueLightAttributesAndState.swupdate.lastinstall = response->get<std::string>(
      "swupdate.lastinstall");
}

void CLightManager::setHueLightInformationsCapabilities(HueLightInformations& hueLightAttributesAndState,
                                                        boost::shared_ptr<shared::CDataContainer>& response)
{
   hueLightAttributesAndState.capabilities.certified = response->get<bool>(
      "capabilities.certified");
   hueLightAttributesAndState.capabilities.control.mindimlevel = response->get<int>(
      "capabilities.control.mindimlevel");
   hueLightAttributesAndState.capabilities.control.maxlumen = response->get<int>(
      "capabilities.control.maxlumen");
   hueLightAttributesAndState.capabilities.control.colorgamuttype = response->get<std::string>(
      "capabilities.control.colorgamuttype");
   //TODO : fill COLORGAMUT
   hueLightAttributesAndState.capabilities.control.ct.min = response->get<int>(
      "capabilities.control.ct.min");
   hueLightAttributesAndState.capabilities.control.ct.max = response->get<int>(
      "capabilities.control.ct.max");

   hueLightAttributesAndState.capabilities.streaming.renderer = response->get<bool>(
      "capabilities.streaming.renderer");
   hueLightAttributesAndState.capabilities.streaming.proxy = response->get<bool>(
      "capabilities.streaming.proxy");
}

void CLightManager::setHueLightInformationsConfig(HueLightInformations& hueLightAttributesAndState,
                                                  boost::shared_ptr<shared::CDataContainer>& response)
{
   hueLightAttributesAndState.config.archetype = response->get<std::string>(
      "config.archetype");
   hueLightAttributesAndState.config.function = response->get<std::string>(
      "config.function");
   hueLightAttributesAndState.config.direction = response->get<std::string>(
      "config.direction");
   hueLightAttributesAndState.config.startup.mode = response->get<std::string>(
      "config.startup.mode");
   hueLightAttributesAndState.config.startup.configured = response->get<bool>(
      "config.startup.configured");
}

void CLightManager::setLightState(const std::string& lightUrl, shared::CDataContainer& body)
{
   try
   {
      const auto response = shared::http::CHttpMethods::sendJsonPutRequest(lightUrl, body.serialize());
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Get http request or interpret answer \"%1%\" : %2%") % lightUrl
         %
         e.what()).str();
      YADOMS_LOG(error) << "Fail to send Get http request or interpret answer " << lightUrl << " : " << e.what();
      throw;
   }
}

void CLightManager::setNewLights(const boost::system::error_code& errorCode)
{
   // boost::asio::error::operation_aborted : Normal behaviour-Invoked when we cancelled the timer
   if (errorCode && errorCode != boost::asio::error::operation_aborted)
      throw std::runtime_error("Error trying to get new lights");
   m_newLights = getAllLights();

   closeReadingNewLights();
}

void CLightManager::setLightId(std::string& lightName, std::map<int, HueLightInformations>& detectedLights)
{
   const auto it = std::find_if(std::begin(detectedLights), std::end(detectedLights),
                                [&lightName](auto&& pair)
                                {
                                   return pair.second.name == lightName;
                                });

   if (it == std::end(detectedLights))
   {
      YADOMS_LOG(warning) << "Light not found";
      throw std::runtime_error("Light ID is not found");
   }
   YADOMS_LOG(information) << "Light ID = " << m_lightId << " is found ";
   m_lightId = it->first;
}

void CLightManager::lightOn()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightId);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("on", true);
   setLightState(lightUrl, body);
}


void CLightManager::lightOff()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightId);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("on", false);
   setLightState(lightUrl, body);
}

void CLightManager::setLightColorUsingXy(const std::string& hexRgb)
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightId);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   auto rgb = CColorConverter::hexToRgb(hexRgb);
   const auto xy = CColorConverter::rgbToXy(rgb);

   shared::CDataContainer body;
   body.set("on", true);
   body.set("xy.0", xy.x);
   body.set("xy.1", xy.y);

   setLightState(lightUrl, body);
}

void CLightManager::searchForNewLights()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kGetAllLights);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);
   const std::string body;
   std::map<int, HueLightInformations> newHueLights;
   try
   {
      const auto response = shared::http::CHttpMethods::sendJsonPostRequest(lightUrl, body);

      startReadingNewLights();
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Get http request or interpret answer \"%1%\" : %2%") % lightUrl
         %
         e.what()).str();
      YADOMS_LOG(error) << "Fail to send Get http request or interpret answer " << lightUrl << " : " << e.what();
      throw std::runtime_error(message);
   }
}

std::map<int, HueLightInformations> CLightManager::getNewLights()
{
   return m_newLights;
}

void CLightManager::startReadingNewLights()
{
   boost::asio::steady_timer timer(m_ios, boost::asio::chrono::seconds(40));
   timer.async_wait(boost::bind(&CLightManager::setNewLights, this,
                                boost::asio::placeholders::error));

   m_ios.run();
}

void CLightManager::closeReadingNewLights()
{
   m_ios.stop();
}
