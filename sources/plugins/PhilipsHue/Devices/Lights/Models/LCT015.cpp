#include "LCT015.h"
#include "../../Utils/ColorConverter.h"
#include "shared/http/HttpRestHelpers.h"
#include "shared/Log.h"

const std::string CLct015::LightState("STATE");
const std::string CLct015::RgbColor("RgbColor");

CLct015::CLct015(boost::shared_ptr<CUrlManager>& urlManager, CHueLightInformations& lightInformations, int& lightId)
   : m_urlManager(urlManager),
     m_lightId(lightId), m_switch(boost::make_shared<yApi::historization::CSwitch>(LightState)),
     m_rgb(boost::make_shared<yApi::historization::CColorRGB>(
        RgbColor, shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)),
     m_historizables({m_switch, m_rgb}),
     m_lightInformations(lightInformations)
{
}

void CLct015::setLightId(std::string& lightName, std::map<int, CHueLightInformations>& detectedLights)
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

void CLct015::lightOn()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightId);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("on", true);
   setLightState(lightUrl, body);
}


void CLct015::lightOff()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightId);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("on", false);
   setLightState(lightUrl, body);
}

void CLct015::setLightColorUsingXy(const std::string& hexRgb)
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

std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> CLct015::
getHistorizables()
{
   return m_historizables;
}

void CLct015::setLightState(const std::string& lightUrl, shared::CDataContainer& body)
{
   try
   {
      const auto response = shared::http::CHttpRestHelpers::sendJsonPutRequest(lightUrl, body.serialize());
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

std::string CLct015::getName()
{
   return m_lightInformations.getName();
}

std::string CLct015::getType()
{
   return m_lightInformations.getType();
}

std::string CLct015::getModelId()
{
   return m_lightInformations.getModelId();
}
