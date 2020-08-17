#pragma once

class ILightManager
{
public:
   virtual ~ILightManager() = default;

   virtual std::map<int, HueLightInformations> getAllLights() = 0;

   virtual HueLightInformations getLightAttributesAndState(int id) = 0;

   virtual int getLightId(std::string& lightName, std::map<int, HueLightInformations>& detectedLights) = 0;
};