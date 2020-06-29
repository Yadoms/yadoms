#include "shared/Log.h"
#include "shared/http/HttpMethods.h"
#include "SsdpClient.h"
#include "SsdpDiscoverService.h"


std::vector<boost::shared_ptr<CSsdpDiscoveredDevice>> CSsdpDiscoverService::getDevicesDescription(
   const std::vector<std::string>& descriptionUrls)
{
   std::vector<boost::shared_ptr<CSsdpDiscoveredDevice>> devicesDescription;
   for (const auto& descriptionUrl : descriptionUrls)
   {
      try
      {
         devicesDescription.push_back(
            boost::make_shared<CSsdpDiscoveredDevice>(shared::http::CHttpMethods::sendGetRequest(descriptionUrl)));
      }
      catch (std::exception& e)
      {
         const auto message = (boost::format("Fail to send Get http request or interpret answer \"%1%\"") %
            e.what()).str();
         YADOMS_LOG(error) << message;
      }
   }
   return devicesDescription;
}

std::vector<boost::shared_ptr<CSsdpDiscoveredDevice>> CSsdpDiscoverService::discover(
   const std::string& searchTarget,
   const std::chrono::duration<long long>& timeout)
{
   boost::asio::io_service ios; //TODO transférer dans CSsdpClient ?
   const auto ssdpClient = boost::make_shared<CSsdpClient>(ios, searchTarget, timeout);

   ios.reset();
   ios.run();

   const auto discoveredDevices(getDevicesDescription(ssdpClient->getDescriptionUrls()));

   ios.stop();

   return discoveredDevices;
}
