#include "UrlManager.h"

const std::string CUrlManager::ApiPath("/api");
const std::string CUrlManager::GetAllLightsPath("/lights");
const std::string CUrlManager::DescriptionPath("/description.xml");
const std::string CUrlManager::LightStatePath("/state");

CUrlManager::CUrlManager(CHueInformations& hueInformations, CConfiguration& configuration)
   : m_hueInformations(hueInformations),
     m_configuration(configuration)
{
}

CUrlManager::CUrlManager(CConfiguration& configuration)
   : m_configuration(configuration)
{
}


std::string CUrlManager::getUrlPatternPath(EUrlPattern urlPattern, const int id) const
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
   case kDescription:
      urlPatternPath = DescriptionPath;
      break;
   case kGetLightAttributesAndState:
      urlPatternPath = ApiPath + "/" + m_username + GetAllLightsPath + "/" + std::to_string(id);
      break;
   case kLightState:
      urlPatternPath = ApiPath + "/" + m_username + GetAllLightsPath + "/" + std::to_string(id) + LightStatePath;
      break;
   default:
      urlPatternPath = "";
   }

   return urlPatternPath;
}

std::string CUrlManager::getPatternUrl(const std::string& urlPatternPath) const
{
   const auto protocolType = "http://";
   const auto ipAddress = m_configuration.getPairingMode() == EPairingMode::kAuto
                             ? m_hueInformations.getIp()
                             : m_configuration.getIPAddress();

   return protocolType + ipAddress + urlPatternPath;
}

std::string CUrlManager::getUsername() const
{
   return m_username;
}

void CUrlManager::setUsername(std::string& username)
{
   m_username = username;
}
