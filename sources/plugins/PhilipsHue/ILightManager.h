#pragma once
#include "Entities/HueLightInformations.h"

class ILightManager
{
public:
   virtual ~ILightManager() = default;

   virtual std::map<int, CHueLightInformations> getAllLights() = 0;

   virtual CHueLightInformations getLightAttributesAndState(int id) = 0;

   virtual void setLightId(std::string& lightName, std::map<int, CHueLightInformations>& detectedLights) = 0;

   virtual void lightOn() = 0;

   virtual void lightOff() = 0;

   virtual void setLightColorUsingXy(const std::string& hexRgb) = 0;

   virtual void searchForNewLights() = 0;

   virtual std::map<int, CHueLightInformations> getNewLights() = 0;
};
