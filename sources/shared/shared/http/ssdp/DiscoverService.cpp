#include "stdafx.h"
#include "shared/Log.h"
#include "Client.h"
#include "DiscoverService.h"
#include "DiscoveredDevice.h"
#include "shared/http/HttpRestHelpers.h"

namespace shared
{
	namespace http
	{
		namespace ssdp
		{
			std::vector<boost::shared_ptr<IDiscoveredDevice>> CDiscoverService::getDevicesDescription(
				const std::set<std::string>& descriptionUrls)
			{
				std::vector<boost::shared_ptr<IDiscoveredDevice>> devicesDescription;
				for (const auto& descriptionUrl : descriptionUrls)
				{
					try
					{
						devicesDescription.emplace_back(
							boost::make_shared<CDiscoveredDevice>(CHttpRestHelpers::sendGetRequest(descriptionUrl)));
					}
					catch (std::exception& e)
					{
						YADOMS_LOG(error) << "Fail to send Get http request or interpret answer : " << e.what();
					}
				}
				return devicesDescription;
			}

			std::vector<boost::shared_ptr<IDiscoveredDevice>> CDiscoverService::discover(
				const std::string& searchTarget,
				const std::chrono::duration<long long>& timeout)
			{
				boost::asio::io_context io;
				const auto ssdpClient = boost::make_shared<CClient>(io, searchTarget, timeout);

				io.run();

				const auto discoveredDevices(getDevicesDescription(ssdpClient->getDescriptionUrls()));

				io.stop();

				return discoveredDevices;
			}
		}
	}
}
