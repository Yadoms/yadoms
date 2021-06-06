#include "GamutC.h"
#include "../../Utils/ColorConverter.h"
#include "shared/http/HttpRestHelpers.h"
#include "shared/Log.h"

const std::string CGamutC::LightState("STATE");
const std::string CGamutC::RgbColor("RgbColor");

CGamutC::CGamutC(boost::shared_ptr<CUrlManager>& urlManager, std::pair<int, CHueLightInformations>& lightInformations)
   : m_urlManager(urlManager),
     m_switch(boost::make_shared<yApi::historization::CSwitch>(LightState)),
     m_rgb(boost::make_shared<yApi::historization::CColorRGB>(
        RgbColor, shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)),
     m_historizables({m_switch, m_rgb}),
     m_lightInformations(lightInformations)
{
}

void CGamutC::lightOn()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightInformations.first);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("on", true);
   setLightState(lightUrl, body);
}


void CGamutC::lightOff()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightInformations.first);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("on", false);
   setLightState(lightUrl, body);
}

void CGamutC::setLightColorUsingXy(const std::string& hexRgb)
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightInformations.first);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   auto rgb = CColorConverter::hexToRgb(hexRgb);
   const auto xy = CColorConverter::rgbToXy(rgb);

   shared::CDataContainer body;
   body.set("on", true);
   body.set("xy.0", xy.getX());
   body.set("xy.1", xy.getY());

   setLightState(lightUrl, body);
}

std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> CGamutC::
getHistorizables()
{
   return m_historizables;
}

void CGamutC::setLightState(const std::string& lightUrl, shared::CDataContainer& body)
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

std::string CGamutC::getName()
{
   return m_lightInformations.second.getName();
}

ELightType CGamutC::getType()
{
   return m_lightInformations.second.getType();
}

std::string CGamutC::getModelId()
{
   return m_lightInformations.second.getModelId();
}

int CGamutC::getDeviceId()
{
   return m_lightInformations.first;
}

bool CGamutC::hasColorControl()
{
   return true;
}

bool CGamutC::hasBrightnessControl()
{
   return true;
}

bool CGamutC::hasTemperatureControl()
{
   return true;
}
