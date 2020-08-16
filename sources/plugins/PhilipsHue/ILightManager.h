#pragma once

class ILightManager
{
public:
   virtual ~ILightManager() = default;

   virtual std::map<int, HueLightInformations> getAllLights() = 0;

   virtual HueLightInformations getLightAttributesAndState(int id) = 0;
};