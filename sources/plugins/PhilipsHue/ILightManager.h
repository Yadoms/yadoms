#pragma once
#include "Hue.h"
class ILightManager
{
public:
   virtual ~ILightManager() = default;

   virtual std::map<int, HueLightInformations> getAllLights() = 0;

   virtual HueLightInformations getLightAttributesAndState(int id) = 0;

   virtual void setLightId(std::string& lightName, std::map<int, HueLightInformations>& detectedLights) = 0;

   virtual void lightOn() = 0;

   virtual void lightOff() = 0;

   virtual void setLightColorUsingXy(const std::string& hexRgb) = 0;

   virtual void searchForNewLights() = 0;

   virtual std::map<int, HueLightInformations> getNewLights() = 0;
};