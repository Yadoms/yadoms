#include "GamutB.h"
#include "../../Utils/ColorConverter.h"
#include "shared/http/HttpRestHelpers.h"
#include "shared/Log.h"

const std::string CGamutB::LightState("STATE");
const std::string CGamutB::RgbColor("RgbColor");

CGamutB::CGamutB(boost::shared_ptr<CUrlManager>& urlManager, std::pair<int, CHueLightInformations>& lightInformations)
   : m_urlManager(urlManager),
     m_switch(boost::make_shared<yApi::historization::CSwitch>(LightState)),
     m_rgb(boost::make_shared<yApi::historization::CColorRGB>(
        RgbColor, shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)),
     m_historizables({m_switch, m_rgb}),
     m_lightInformations(lightInformations)
{
}

void CGamutB::lightOn()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightInformations.first);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("on", true);
   setLightState(lightUrl, body);
}


void CGamutB::lightOff()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightInformations.first);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("on", false);
   setLightState(lightUrl, body);
}

void CGamutB::setLightColorUsingXy(const std::string& hexRgb)
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

std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> CGamutB::
getHistorizables()
{
   return m_historizables;
}

void CGamutB::setLightState(const std::string& lightUrl, shared::CDataContainer& body)
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

std::string CGamutB::getName()
{
   return m_lightInformations.second.getName();
}

ELightType CGamutB::getType()
{
   return m_lightInformations.second.getType();
}

std::string CGamutB::getModelId()
{
   return m_lightInformations.second.getModelId();
}

bool CGamutB::getState()
{
   return m_lightInformations.second.getState().isOn();
}

int CGamutB::getDeviceId()
{
   return m_lightInformations.first;
}

bool CGamutB::hasColorControl()
{
   return true;
}

bool CGamutB::hasBrightnessControl()
{
   return true;
}

bool CGamutB::hasTemperatureControl()
{
   return true;
}

void CGamutB::rename(std::string& newLightName)
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kGetLightAttributesAndState, m_lightInformations.first);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("name", newLightName);

   try
   {
      const auto response = shared::http::CHttpRestHelpers::sendJsonPutRequest(lightUrl, body.serialize());
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Put http request or interpret answer \"%1%\" : %2%") % lightUrl
         %
         e.what()).str();
      YADOMS_LOG(error) << "Fail to send Put http request or interpret answer " << lightUrl << " : " << e.what();
      throw;
   }
}
