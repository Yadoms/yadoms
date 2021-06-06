#include "GamutNone.h"

#include "../../Utils/ColorConverter.h"
#include "shared/http/HttpRestHelpers.h"
#include "shared/Log.h"

const std::string CGamutNone::LightState("STATE");

CGamutNone::CGamutNone(boost::shared_ptr<CUrlManager>& urlManager,
                       std::pair<int, CHueLightInformations>& lightInformations)
   : m_urlManager(urlManager),
     m_switch(boost::make_shared<yApi::historization::CSwitch>(LightState)),
     m_historizables({m_switch}),
     m_lightInformations(lightInformations)
{
}

void CGamutNone::lightOn()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightInformations.first);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("on", true);
   setLightState(lightUrl, body);
}


void CGamutNone::lightOff()
{
   const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kLightState, m_lightInformations.first);
   const auto lightUrl = m_urlManager->getPatternUrl(urlPatternPath);

   shared::CDataContainer body;
   body.set("on", false);
   setLightState(lightUrl, body);
}

void CGamutNone::setLightColorUsingXy(const std::string& hexRgb)
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

std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> CGamutNone::
getHistorizables()
{
   return m_historizables;
}

void CGamutNone::setLightState(const std::string& lightUrl, shared::CDataContainer& body)
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

std::string CGamutNone::getName()
{
   return m_lightInformations.second.getName();
}

ELightType CGamutNone::getType()
{
   return m_lightInformations.second.getType();
}

std::string CGamutNone::getModelId()
{
   return m_lightInformations.second.getModelId();
}

int CGamutNone::getDeviceId()
{
   return m_lightInformations.first;
}

bool CGamutNone::hasColorControl()
{
   return false;
}

bool CGamutNone::hasBrightnessControl()
{
   if (getType() == ELightType::kDIMMABLE_LIGHT || getType() == ELightType::kDIMMABLE_PLUGIN_IN_UNIT)
   {
      return true;
   }
   return false;
}

bool CGamutNone::hasTemperatureControl()
{
   return false;
}
