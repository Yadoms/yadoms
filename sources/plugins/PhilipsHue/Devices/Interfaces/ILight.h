#pragma once
#include "../../Entities/HueLightInformations.h"
#include "shared/plugin/yPluginApi/historization/IHistorizable.h"

class ILight
{
public:
   virtual ~ILight() = default;

   virtual void lightOn() = 0;

   virtual void lightOff() = 0;

   virtual void setLightColorUsingXy(const std::string& hexRgb) = 0;

   virtual std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>
   getHistorizables() = 0;

   virtual std::string getName() = 0;

   virtual ELightType getType() = 0;

   virtual bool getState() = 0;

   virtual std::string getModelId() = 0;

   virtual int getDeviceId() = 0;

   virtual bool hasColorControl() = 0;

   virtual bool hasBrightnessControl() = 0;

   virtual bool hasTemperatureControl() = 0;

   virtual void rename(std::string& newLightName) = 0;
};
