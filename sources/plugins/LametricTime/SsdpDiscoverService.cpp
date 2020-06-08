#include "shared/Log.h"
#include "shared/http/HttpMethods.h"
#include "shared/http/HttpException.hpp"
#include "SsdpClient.h"
#include "SsdpDiscoverService.h"

std::string CSsdpDiscoverService::getDeviceDescription(std::string& descriptionUrl)
{
   if (descriptionUrl.empty())
      return "";
   std::string outXml;
   try
   {
      shared::CHttpMethods::sendGetRequest(descriptionUrl,
                                           [&outXml](const std::map<std::string, std::string>& receivedHeaders,
                                                     const std::string& data)
                                           {
                                              outXml = data;
                                           });
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Get http request or interpret answer \"%1%\" : %2%") %
         descriptionUrl %
         e.what()).str();
      YADOMS_LOG(error) << message;
      throw shared::CHttpException(message);
   }

   return outXml;
}

bool CSsdpDiscoverService::discover(std::string searchTarget,
                                    const boost::function<bool(CSsdpDiscoveredDevice& discoveredDevice)>&
                                    discoveredDeviceHandler)
{
   boost::asio::io_service ios;
   const boost::shared_ptr<CSsdpClient> ssdpClient = boost::make_shared<CSsdpClient>(ios, searchTarget);

   ios.reset();
   ios.run();

   auto descriptionUrl = ssdpClient->getDescriptionUrl();
   auto deviceDescription = getDeviceDescription(descriptionUrl);
   CSsdpDiscoveredDevice discoveredDevice(deviceDescription);

   ios.stop();
   return discoveredDeviceHandler(discoveredDevice);
}
