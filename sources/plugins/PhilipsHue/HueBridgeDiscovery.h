#pragma once

class CHueBridgeDiscovery
{
public:
   struct HueInformations
   {
      std::string ip;
      std::string friendlyName;
      std::string modelName;
      std::string modelNumber;
   };

   static std::vector<HueInformations> FindBridges();
private:
   static std::string getIpAddress(const std::string& urlBase);
};
