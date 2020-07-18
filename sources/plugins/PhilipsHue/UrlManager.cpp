#include "UrlManager.h"

const std::string CUrlManager::ApiPath("/api");
const std::string CUrlManager::GetAllLightsPath("/lights");

CUrlManager::CUrlManager(CHueBridgeDiscovery::HueInformations& hueInformations)
   : m_hueInformations(hueInformations)
{
}


std::string CUrlManager::getUrlPatternPath(EUrlPattern urlPattern) const
{
   std::string urlPatternPath;
   switch (urlPattern)
   {
   case kApi:
      urlPatternPath = ApiPath;
      break;
   case kGetAllLights:
      urlPatternPath = ApiPath + "/" + m_username + GetAllLightsPath;
      break;
   default:
      urlPatternPath = "";
   }

   return urlPatternPath;
}

std::string CUrlManager::getPatternUrl(const std::string& urlPatternPath) const
{
   const auto protocolType = "http://";

   return protocolType + m_hueInformations.ip + urlPatternPath;
}
