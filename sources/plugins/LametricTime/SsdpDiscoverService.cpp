#include "shared/Log.h"
#include "shared/http/HttpMethods.h"
#include "SsdpClient.h"
#include "SsdpDiscoverService.h"


std::vector<std::string> CSsdpDiscoverService::getDevicesDescription(const std::vector<std::string>& descriptionUrls)
{
   std::vector<std::string> devicesDescription;
   for (const auto& descriptionUrl : descriptionUrls)
   {
      try
      {
         devicesDescription.push_back(shared::http::CHttpMethods::sendGetRequest(descriptionUrl));
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

CSsdpDiscoveredDevice CSsdpDiscoverService::discover(const std::string& searchTarget,
                                                     const std::chrono::duration<long long>& timeout)
{
   boost::asio::io_service ios;
   const auto ssdpClient = boost::make_shared<CSsdpClient>(ios, searchTarget, timeout);

   ios.reset();
   ios.run();

   CSsdpDiscoveredDevice discoveredDevice(getDevicesDescription(ssdpClient->getDescriptionUrls()));

   ios.stop();

   return discoveredDevice;
}
