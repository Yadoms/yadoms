#pragma once
#include "../../../UrlManager.h"
#include "../../Interfaces/ILight.h"
#include <plugin_cpp_api/IPlugin.h>

class CGamutNone : public ILight
{
public:

   CGamutNone(boost::shared_ptr<CUrlManager>& urlManager, std::pair<int, CHueLightInformations>& lightInformations);
   virtual ~CGamutNone() = default;

   void lightOn() override;

   void lightOff() override;

   void setLightColorUsingXy(const std::string& hexRgb) override;

   std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> getHistorizables()
   override;

   std::string getName() override;

   ELightType getType() override;

   std::string getModelId() override;

   int getDeviceId() override;

   bool hasColorControl() override;

   bool hasBrightnessControl() override;

   bool hasTemperatureControl() override;

private :
   static void setLightState(const std::string& lightUrl, shared::CDataContainer& body);
   boost::shared_ptr<CUrlManager> m_urlManager;
   static const std::string LightState;
   boost::shared_ptr<yApi::historization::CSwitch> m_switch;
   std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> m_historizables;
   std::pair<int, CHueLightInformations> m_lightInformations;
};
