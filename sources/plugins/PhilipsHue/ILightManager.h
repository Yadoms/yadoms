#pragma once

class ILightManager
{
public:
   virtual ~ILightManager() = default;

   virtual std::vector<HueLightInformations> getAllLights() = 0;
};