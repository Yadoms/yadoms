#include "shared/Log.h"
#include "shared/http/HttpMethods.h"
#include "SsdpClient.h"
#include "SsdpDiscoverService.h"

CSsdpDiscoverService::CSsdpDiscoverService(std::string& searchTarget)
{
}

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

bool CSsdpDiscoverService::discover(const std::function<bool(CSsdpDiscoveredDevice& discoveredDevice)>&
                                    discoveredDeviceHandler, const std::chrono::duration<long long>& timeout,
                                    const std::string& searchTarget
)
{
	boost::asio::io_service ios;
	const auto ssdpClient = boost::make_shared<CSsdpClient>(ios, searchTarget, timeout);

	ios.reset();
	ios.run();

   const auto descriptionUrls = ssdpClient->getDescriptionUrls();
	const auto deviceDescription = getDevicesDescription(descriptionUrls);

	CSsdpDiscoveredDevice discoveredDevice(deviceDescription);

	ios.stop();
	return discoveredDeviceHandler(discoveredDevice);
}
