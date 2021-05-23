#pragma once
#include "../../../UrlManager.h"
#include "../../Interfaces/ILight.h"
#include <plugin_cpp_api/IPlugin.h>

class CLct015 : public ILight
{
public:

   CLct015(boost::shared_ptr<CUrlManager>& urlManager, int& lightId);
   virtual ~CLct015() = default;

   void lightOn() override;

   void lightOff() override;

   void setLightColorUsingXy(const std::string& hexRgb) override;

   std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> getHistorizables();
private :
   static void setLightState(const std::string& lightUrl, shared::CDataContainer& body);
   void setLightId(std::string& lightName, std::map<int, CHueLightInformations>& detectedLights);
   boost::shared_ptr<CUrlManager>& m_urlManager;
   int m_lightId;
   static const std::string LightState;
   static const std::string RgbColor;
   boost::shared_ptr<yApi::historization::CSwitch> m_switch;
   boost::shared_ptr<yApi::historization::CColorRGB> m_rgb;
   std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> m_historizables;
};
