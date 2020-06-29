#include "shared/Log.h"
#include "shared/http/HttpMethods.h"
#include "Client.h"
#include "DiscoverService.h"

namespace shared
{
   namespace http
   {
      namespace ssdp
      {
         std::vector<boost::shared_ptr<CDiscoveredDevice>> CDiscoverService::getDevicesDescription(
            const std::vector<std::string>& descriptionUrls)
         {
            std::vector<boost::shared_ptr<CDiscoveredDevice>> devicesDescription;
            for (const auto& descriptionUrl : descriptionUrls)
            {
               try
               {
                  devicesDescription.push_back(
                     boost::make_shared<CDiscoveredDevice>(CHttpMethods::sendGetRequest(descriptionUrl)));
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

         std::vector<boost::shared_ptr<CDiscoveredDevice>> CDiscoverService::discover(
            const std::string& searchTarget,
            const std::chrono::duration<long long>& timeout)
         {
            boost::asio::io_service ios; //TODO transférer dans CClient ?
            const auto ssdpClient = boost::make_shared<CClient>(ios, searchTarget, timeout);

            ios.reset();
            ios.run();

            const auto discoveredDevices(getDevicesDescription(ssdpClient->getDescriptionUrls()));

            ios.stop();

            return discoveredDevices;
         }
      }
   }
}
