#pragma once
#include "Configuration.h"
#include "Entities/HueInformations.h"

class CUrlManager
{
public:
   CUrlManager(CHueInformations& hueInformations, CConfiguration& configuration);
   CUrlManager(CConfiguration& configuration);

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
      kLightState,
      kUnknown
   };

   std::string getUrlPatternPath(EUrlPattern urlPattern, int id = 0) const;
   std::string getPatternUrl(const std::string& urlPatternPath) const;

   std::string getUsername() const;
   void setUsername(std::string& username);

private:
   static const std::string ApiPath;
   static const std::string GetAllLightsPath;
   static const std::string DescriptionPath;
   static const std::string LightStatePath;

   std::string m_username;
   CHueInformations m_hueInformations;
   CConfiguration m_configuration;
};
