#pragma once
#include "HueBridgeDiscovery.h"
#include "Configuration.h"

class CUrlManager
{
public:
   CUrlManager(CHueBridgeDiscovery::HueInformations& hueInformations, CConfiguration& configuration);

   virtual ~CUrlManager() = default;

   enum EUrlPattern
   {
      kApi,
      kGetAllLights,
      kGetNewLights,
      kSearchNewLights,
      kGetLightAttributesAndState,
      kSetLightAttributes,
      kDescription,
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
   static const std::string DescriptionPath;
   std::string m_username;
   CHueBridgeDiscovery::HueInformations m_hueInformations;
   CConfiguration& m_configuration;
};
