#include "LightManager.h"
#include "shared/Log.h"
#include "shared/http/HttpMethods.h"
#include "ColorConverter.h"
#include "Entities/HueStreaming.h"

CLightManager::CLightManager(boost::shared_ptr<CUrlManager>& urlManager)
   : m_urlManager(urlManager),
     m_lightId(0)
{
}

std::map<int, CHueLightInformations> CLightManager::getAllLights()
{
   std::map<int, CHueLightInformations> hueLightsInformations;
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kGetAllLights);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   try
   {
      const auto response = shared::http::CHttpMethods::sendJsonGetRequest(lightUrl);
      auto lightCounter = 1;;
      while (response->exists(std::to_string(lightCounter)))
      {
         CHueLightInformations hueLightInformations;
         CHueState state;
         // Fill State
         state.setOn(response->isNull(std::to_string(lightCounter) + ".state.on")
                        ? false
                        : response->get<bool>(std::to_string(lightCounter) + ".state.on"));

         state.setBri(response->isNull(std::to_string(lightCounter) + ".state.bri")
                         ? 0
                         : response->get<int>(std::to_string(lightCounter) + ".state.bri"));

         state.setHue(response->isNull(std::to_string(lightCounter) + ".state.hue")
                         ? 0
                         : response->get<int>(std::to_string(lightCounter) + ".state.hue"));

         state.setSat(response->isNull(std::to_string(lightCounter) + ".state.sat")
                         ? 0
                         : response->get<int>(std::to_string(lightCounter) + ".state.sat"));

         state.setEffect(response->isNull(std::to_string(lightCounter) + ".state.effect")
                            ? ""
                            : response->get<std::string>(std::to_string(lightCounter) + ".state.effect"));

         CXy xy;
         // Fill XY
         xy.setX(response->isNull(std::to_string(lightCounter) + ".state.xy.0")
                    ? 0.0
                    : response->get<float>(std::to_string(lightCounter) + ".state.xy.0"));

         xy.setY(response->isNull(std::to_string(lightCounter) + ".state.xy.1")
                    ? 0.0
                    : response->get<float>(std::to_string(lightCounter) + ".state.xy.1"));

         state.setXy(xy);

         state.setCt(response->isNull(std::to_string(lightCounter) + ".state.ct")
                        ? 0
                        : response->get<int>(std::to_string(lightCounter) + ".state.ct"));

         state.setAlert(response->isNull(std::to_string(lightCounter) + ".state.alert")
                           ? ""
                           : response->get<std::string>(std::to_string(lightCounter) + ".state.alert"));

         state.setColorMode(response->isNull(std::to_string(lightCounter) + ".state.colormode")
                               ? ""
                               : response->get<std::string>(std::to_string(lightCounter) + ".state.colormode"));

         state.setReachable(response->isNull(std::to_string(lightCounter) + ".state.reachable")
                               ? false
                               : response->get<bool>(std::to_string(lightCounter) + ".state.reachable"));

         hueLightInformations.setState(state);

         CHueSwUpdate swUpdate;
         // Fill SwUpdate
         swUpdate.setState(response->isNull(std::to_string(lightCounter) + ".swupdate.state")
                              ? ""
                              : response->get<std::string>(std::to_string(lightCounter) + ".swupdate.state"));

         swUpdate.setLastInstall(response->isNull(std::to_string(lightCounter) + ".swupdate.lastinstall")
                                    ? ""
                                    : response->get<std::string>(
                                       std::to_string(lightCounter) + ".swupdate.lastinstall"));

         hueLightInformations.setSwUpdate(swUpdate);

         hueLightInformations.setType(response->isNull(std::to_string(lightCounter) + ".type")
                                         ? ""
                                         : response->get<std::string>(
                                            std::to_string(lightCounter) + ".type"));

         hueLightInformations.setName(response->isNull(std::to_string(lightCounter) + ".name")
                                         ? ""
                                         : response->get<std::string>(
                                            std::to_string(lightCounter) + ".name"));

         hueLightInformations.setModelId(response->isNull(std::to_string(lightCounter) + ".modelid")
                                            ? ""
                                            : response->get<std::string>(
                                               std::to_string(lightCounter) + ".modelid"));

         hueLightInformations.setManufacturerName(response->isNull(std::to_string(lightCounter) + ".manufacturername")
                                                     ? ""
                                                     : response->get<std::string>(
                                                        std::to_string(lightCounter) + ".manufacturername"));

         hueLightInformations.setProductName(response->isNull(std::to_string(lightCounter) + ".productname")
                                                ? ""
                                                : response->get<std::string>(
                                                   std::to_string(lightCounter) + ".productname"));
         // Fill Capabilities
         CHueCapabilities capabilities;
         capabilities.setCertified(response->isNull(std::to_string(lightCounter) + ".capabilities.certified")
                                      ? false
                                      : response->get<bool>(std::to_string(lightCounter) + ".capabilities.certified"));
         // Fill control
         CHueControl control;
         control.setMinDimLevel(response->isNull(std::to_string(lightCounter) + ".capabilities.control.mindimlevel")
                                   ? 0
                                   : response->get<int>(
                                      std::to_string(lightCounter) + ".capabilities.control.mindimlevel"));

         control.setMaxLumen(response->isNull(std::to_string(lightCounter) + ".capabilities.control.maxlumen")
                                ? 0
                                : response->get<int>(std::to_string(lightCounter) + ".capabilities.control.maxlumen"));

         control.setColorGamutType(
            response->isNull(std::to_string(lightCounter) + ".capabilities.control.colorgamuttype")
               ? ""
               : response->get<std::string>(
                  std::to_string(lightCounter) + ".capabilities.control.colorgamuttype"));

         //TODO : fill COLORGAMUT

         // Fill ct
         CHueCt ct;
         ct.setMin(response->isNull(std::to_string(lightCounter) + ".capabilities.control.ct.min")
                      ? 0
                      : response->get<int>(std::to_string(lightCounter) + ".capabilities.control.ct.min"));

         ct.setMax(response->isNull(std::to_string(lightCounter) + ".capabilities.control.ct.max")
                      ? 0
                      : response->get<int>(std::to_string(lightCounter) + ".capabilities.control.ct.max"));

         control.setCt(ct);

         capabilities.setControl(control);

         // FILL streaming
         CHueStreaming streaming;
         streaming.setRenderer(response->isNull(std::to_string(lightCounter) + ".capabilities.streaming.renderer")
                                  ? false
                                  : response->get<bool>(
                                     std::to_string(lightCounter) + ".capabilities.streaming.renderer"));

         streaming.setProxy(response->isNull(std::to_string(lightCounter) + ".capabilities.streaming.proxy")
                               ? false
                               : response->get<bool>(std::to_string(lightCounter) + ".capabilities.streaming.proxy"));

         capabilities.setStreaming(streaming);

         hueLightInformations.setCapabilities(capabilities);

         // Fill config
         CHueConfig config;
         config.setArchetype(response->isNull(std::to_string(lightCounter) + ".config.archetype")
                                ? ""
                                : response->get<std::string>(
                                   std::to_string(lightCounter) + ".config.archetype"));
         config.setFunction(response->isNull(std::to_string(lightCounter) + ".config.function")
                               ? ""
                               : response->get<std::string>(
                                  std::to_string(lightCounter) + ".config.function"));

         config.setDirection(response->isNull(std::to_string(lightCounter) + ".config.direction")
                                ? ""
                                : response->get<std::string>(
                                   std::to_string(lightCounter) + ".config.direction"));

         hueLightInformations.setConfig(config);

         hueLightInformations.setUniqueId(response->isNull(std::to_string(lightCounter) + ".uniqueid")
                                             ? ""
                                             : response->get<std::string>(
                                                std::to_string(lightCounter) + ".uniqueid"));
         hueLightInformations.setSwVersion(response->isNull(std::to_string(lightCounter) + ".swversion")
                                              ? ""
                                              : response->get<std::string>(
                                                 std::to_string(lightCounter) + ".swversion"));

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

CHueLightInformations CLightManager::getLightAttributesAndState(const int id)
{
   CHueLightInformations hueLightAttributesAndState;
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kGetLightAttributesAndState, id);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   try
   {
      auto response = shared::http::CHttpMethods::sendJsonGetRequest(lightUrl);

      hueLightAttributesAndState.setState(getHueLightInformationsState(response));
      hueLightAttributesAndState.setSwUpdate(getHueLightInformationsSwUpdate(response));

      if (!response->isNull("type"))
      {
         hueLightAttributesAndState.setType(response->get<std::string>("type"));
      }
      if (!response->isNull("name"))
      {
         hueLightAttributesAndState.setName(response->get<std::string>("name"));
      }

      if (!response->isNull("modelid"))
      {
         hueLightAttributesAndState.setModelId(response->get<std::string>("modelid"));
      }

      if (!response->isNull("manufacturername"))
      {
         hueLightAttributesAndState.setManufacturerName(response->get<std::string>("manufacturername"));
      }

      if (!response->isNull("productname"))
      {
         hueLightAttributesAndState.setProductName(response->get<std::string>("productname"));
      }

      hueLightAttributesAndState.setCapabilities(getHueLightInformationsCapabilities(response));
      hueLightAttributesAndState.setConfig(getHueLightInformationsConfig(response));

      if (!response->isNull("uniqueid"))
      {
         hueLightAttributesAndState.setUniqueId(response->get<std::string>("uniqueid"));
      }

      if (!response->isNull("swversion"))
      {
         hueLightAttributesAndState.setSwVersion(response->get<std::string>("swversion"));
      }

      if (!response->isNull("swconfigid"))
      {
         hueLightAttributesAndState.setSwConfigId(response->get<std::string>("swconfigid"));
      }

      if (!response->isNull("productid"))
      {
         hueLightAttributesAndState.setProductId(response->get<std::string>("productid"));
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
   return hueLightAttributesAndState;
}

CHueState CLightManager::getHueLightInformationsState(boost::shared_ptr<shared::CDataContainer>& response)
{
   CHueState state;
   if (!response->isNull("state.on"))
   {
      state.setOn(response->get<bool>("state.on"));
   }
   if (!response->isNull("state.bri"))
   {
      state.setBri(response->get<int>("state.bri"));
   }
   if (!response->isNull("state.hue"))
   {
      state.setHue(response->get<int>("state.hue"));
   }
   if (!response->isNull("state.sat"))
   {
      state.setSat(response->get<int>("state.sat"));
   }
   if (!response->isNull("state.effect"))
   {
      state.setEffect(response->get<std::string>("state.effect"));
   }
   CXy xy;
   if (!response->isNull("state.xy.0"))
   {
      xy.setX(response->get<float>("state.xy.0"));
   }
   if (!response->isNull("state.xy.1"))
   {
      xy.setY(response->get<float>("state.xy.1"));
   }
   state.setXy(xy);

   if (!response->isNull("state.ct"))
   {
      state.setCt(response->get<int>("state.ct"));
   }

   if (!response->isNull("state.alert"))
   {
      state.setAlert(response->get<std::string>("state.alert"));
   }

   if (!response->isNull("state.colormode"))
   {
      state.setColorMode(response->get<std::string>("state.colormode"));
   }

   if (!response->isNull("state.mode"))
   {
      state.setMode(response->get<std::string>("state.mode"));
   }

   if (!response->isNull("state.reachable"))
   {
      state.setReachable(response->get<bool>("state.reachable"));
   }
   return state;
}

CHueSwUpdate CLightManager::getHueLightInformationsSwUpdate(boost::shared_ptr<shared::CDataContainer>& response)
{
   CHueSwUpdate swUpdate;
   if (!response->isNull("swupdate.state"))
   {
      swUpdate.setState(response->get<std::string>("swupdate.state"));
   }
   if (!response->isNull("swupdate.lastinstall"))
   {
      swUpdate.setLastInstall(response->get<std::string>("swupdate.lastinstall"));
   }
   return swUpdate;
}

CHueCapabilities CLightManager::getHueLightInformationsCapabilities(boost::shared_ptr<shared::CDataContainer>& response)
{
   CHueCapabilities capabilities;
   if (!response->isNull("capabilities.certified"))
   {
      capabilities.setCertified(response->get<bool>("capabilities.certified"));
   }
   CHueControl control;
   if (!response->isNull("capabilities.control.mindimlevel"))
   {
      control.setMinDimLevel(response->get<int>("capabilities.control.mindimlevel"));
   }

   if (!response->isNull("capabilities.control.maxlumen"))
   {
      control.setMaxLumen(response->get<int>("capabilities.control.maxlumen"));
   }

   if (!response->isNull("capabilities.control.colorgamuttype"))
   {
      control.setColorGamutType(response->get<std::string>("capabilities.control.colorgamuttype"));
   }
   //TODO : fill COLORGAMUT
   CHueCt ct;
   if (!response->isNull("capabilities.control.ct.min"))
   {
      ct.setMin(response->get<int>("capabilities.control.ct.min"));
   }
   if (!response->isNull("capabilities.control.ct.max"))
   {
      ct.setMax(response->get<int>("capabilities.control.ct.max"));
   }
   control.setCt(ct);
   capabilities.setControl(control);

   CHueStreaming streaming;
   if (!response->isNull("capabilities.streaming.renderer"))
   {
      streaming.setRenderer(response->get<bool>("capabilities.streaming.renderer"));
   }
   if (!response->isNull("capabilities.streaming.proxy"))
   {
      streaming.setProxy(response->get<bool>("capabilities.streaming.proxy"));
   }
   capabilities.setStreaming(streaming);
   return capabilities;
}

CHueConfig CLightManager::getHueLightInformationsConfig(boost::shared_ptr<shared::CDataContainer>& response)
{
   CHueConfig config;
   if (!response->isNull("config.archetype"))
   {
      config.setArchetype(response->get<std::string>("config.archetype"));
   }

   if (!response->isNull("config.function"))
   {
      config.setFunction(response->get<std::string>("config.function"));
   }

   if (!response->isNull("config.direction"))
   {
      config.setDirection(response->get<std::string>("config.direction"));
   }
   CHueStartUp startUp;
   if (!response->isNull("config.startup.mode"))
   {
      startUp.setMode(response->get<std::string>("config.startup.mode"));
   }

   if (!response->isNull("config.startup.configured"))
   {
      startUp.setConfigured(response->get<bool>("config.startup.configured"));
   }
   config.setStartUp(startUp);
   return config;
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

void CLightManager::setLightId(std::string& lightName, std::map<int, CHueLightInformations>& detectedLights)
{
   const auto it = std::find_if(std::begin(detectedLights), std::end(detectedLights),
                                [&lightName](auto&& pair)
                                {
                                   return pair.second.getName() == lightName;
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
   body.set("xy.0", xy.getX());
   body.set("xy.1", xy.getY());

   setLightState(lightUrl, body);
}

void CLightManager::searchForNewLights()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kGetAllLights);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);
   const std::string body;
   std::map<int, CHueLightInformations> newHueLights;
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

std::map<int, CHueLightInformations> CLightManager::getNewLights()
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
