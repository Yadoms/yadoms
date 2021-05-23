#pragma once
#include "../../Entities/HueLightInformations.h"

class ILightsService
{
public:
   virtual ~ILightsService() = default;

   virtual std::map<int, CHueLightInformations> getAllLights() = 0;

   virtual CHueLightInformations getLightAttributesAndState(int id) = 0;

   virtual void searchForNewLights() = 0;

   virtual std::map<int, CHueLightInformations> getNewLights() = 0;
};
