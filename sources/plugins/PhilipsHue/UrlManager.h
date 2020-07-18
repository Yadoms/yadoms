#pragma once
#include "HueBridgeDiscovery.h"

class CUrlManager
{
public:
   CUrlManager(CHueBridgeDiscovery::HueInformations& hueInformations);

   virtual ~CUrlManager() = default;

   enum EUrlPattern
   {
      kApi,
      kGetAllLights,
      kGetNewLights,
      kSearchNewLights,
      kGetLightAttributesAndState,
      kSetLightAttributes,
      kUnknown
   };

   std::string getUrlPatternPath(EUrlPattern urlPattern) const;
   std::string getPatternUrl(const std::string& urlPatternPath) const;

   std::string getUsername()
   {
      return m_username;
   }

   void setUsername(std::string& username)
   {
      m_username = username;
   }

private:
   static const std::string ApiPath;
   static const std::string GetAllLightsPath;

   std::string m_username;
   CHueBridgeDiscovery::HueInformations m_hueInformations;
};
