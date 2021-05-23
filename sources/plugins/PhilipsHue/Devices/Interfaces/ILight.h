#pragma once
#include "../../Entities/HueLightInformations.h"
#include "shared/plugin/yPluginApi/historization/IHistorizable.h"

class ILight
{
public:
   virtual ~ILight() = default;

   virtual void setLightId(std::string& lightName, std::map<int, CHueLightInformations>& detectedLights) = 0;

   virtual void lightOn() = 0;

   virtual void lightOff() = 0;

   virtual void setLightColorUsingXy(const std::string& hexRgb) = 0;

   virtual std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>
   getHistorizables() = 0;
};
