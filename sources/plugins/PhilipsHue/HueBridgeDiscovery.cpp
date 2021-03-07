#include "HueBridgeDiscovery.h"
#include "shared/http/ssdp/DiscoverService.h"
#include "shared/http/HttpMethods.h"
#include "shared/http/ssdp/DiscoveredDevice.h"
#include "shared/Log.h"

CHueBridgeDiscovery::CHueBridgeDiscovery(boost::shared_ptr<CUrlManager>& urlManager)
   : m_urlManager(urlManager)
{
}

CHueBridgeDiscovery::CHueBridgeDiscovery()
{
}

std::vector<CHueInformations> CHueBridgeDiscovery::FindBridges()
{
   auto foundBridges = shared::http::ssdp::
      CDiscoverService::discover("service:hue", std::chrono::seconds(30));

   if (foundBridges.empty())
   {
      throw std::runtime_error("No response from the device: wrong ip or no device listening on this network");
   }
   std::vector<CHueInformations> hueInformations;
   CHueInformations bridgeInformations;
   for (const auto& foundBridge : foundBridges)
   {
      bridgeInformations.setIp(getIpAddress(foundBridge->xmlContent()->get<std::string>("root.URLBase")));
      bridgeInformations.setFriendlyName(foundBridge->xmlContent()->get<std::string>("root.device.friendlyName"));
      bridgeInformations.setModelName(foundBridge->xmlContent()->get<std::string>("root.device.modelName"));
      bridgeInformations.setModelNumber(foundBridge->xmlContent()->get<std::string>("root.device.modelNumber"));
      hueInformations.push_back(bridgeInformations);
   }
   return hueInformations;
}

std::string CHueBridgeDiscovery::getIpAddress(const std::string& urlBase)
{
   const boost::regex reg(R"((\d{1,3}(\.\d{1,3}){3}))");
   boost::smatch match;
   if (!regex_search(urlBase, match, reg) || match.length() < 2)
      throw std::runtime_error("Invalid IP found");

   return match[1].str();
}

CHueInformations CHueBridgeDiscovery::getHueInformations()
{
   CHueInformations bridgeInformations;
   try
   {
      const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kDescription);
      const auto descriptionUrl = m_urlManager->getPatternUrl(urlPatternPath);

      boost::shared_ptr<shared::http::ssdp::IDiscoveredDevice> devicesDescription =
         boost::make_shared<shared::http::ssdp::CDiscoveredDevice>(
            shared::http::CHttpMethods::sendGetRequest(descriptionUrl));

      bridgeInformations.setIp(getIpAddress(devicesDescription->xmlContent()->get<std::string>("root.URLBase")));
      bridgeInformations.
         setFriendlyName(devicesDescription->xmlContent()->get<std::string>("root.device.friendlyName"));
      bridgeInformations.setModelName(devicesDescription->xmlContent()->get<std::string>("root.device.modelName"));
      bridgeInformations.setModelNumber(devicesDescription->xmlContent()->get<std::string>("root.device.modelNumber"));
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Get http request or interpret answer \"%1%\"") %
         e.what()).str();
      YADOMS_LOG(error) << message;
   }
   return bridgeInformations;
}
