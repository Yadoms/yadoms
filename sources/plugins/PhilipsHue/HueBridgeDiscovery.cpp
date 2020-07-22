#include "HueBridgeDiscovery.h"
#include "shared/http/ssdp/DiscoverService.h"
#include "shared/http/HttpMethods.h"
#include "shared/http/ssdp/DiscoveredDevice.h"
#include "shared/Log.h"

std::vector<CHueBridgeDiscovery::HueInformations> CHueBridgeDiscovery::FindBridges()
{
   auto foundBridges = shared::http::ssdp::
      CDiscoverService::discover("service:hue", std::chrono::seconds(30));

   if (foundBridges.empty())
   {
      throw std::runtime_error("No response from the device: wrong ip or no device listening on this network");
   }
   std::vector<HueInformations> hueInformations;
   HueInformations bridgeInformations;
   for (const auto& foundBridge : foundBridges)
   {
      bridgeInformations.ip = getIpAddress(foundBridge->xmlContent()->get<std::string>("root.URLBase"));
      bridgeInformations.friendlyName = foundBridge->xmlContent()->get<std::string>("root.device.friendlyName");
      bridgeInformations.modelName = foundBridge->xmlContent()->get<std::string>("root.device.modelName");
      bridgeInformations.modelNumber = foundBridge->xmlContent()->get<std::string>("root.device.modelNumber");
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
