#include "shared/Log.h"
#include "shared/http/HttpMethods.h"
#include "shared/http/HttpException.hpp"
#include "SsdpClient.h"
#include "SsdpDiscoverService.h"

CSsdpDiscoverService::CSsdpDiscoverService(std::string& searchTarget)
{
}

std::vector<std::string> CSsdpDiscoverService::getDevicesDescription(const std::vector<std::string>& descriptionUrls)
{
	std::string outXml;
	std::vector<std::string> devicesDescription;
	for (auto& descriptionUrl : descriptionUrls)
	{
		try
		{
			shared::CHttpMethods::sendGetRequest(descriptionUrl,
			                                     [&outXml](const Poco::Net::HTTPResponse& response,
			                                               std::istream& receivedStream)
			                                     {
				                                     outXml = std::string(
					                                     std::istreambuf_iterator<char>(receivedStream), {});
			                                     },
			                                     shared::CDataContainer(),
			                                     shared::CDataContainer(),
			                                     shared::CHttpMethods::kStandard,
			                                     boost::posix_time::seconds(45));
			devicesDescription.push_back(outXml);
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

bool CSsdpDiscoverService::discover(const boost::function<bool(CSsdpDiscoveredDevice& discoveredDevice)>&
                                    discoveredDeviceHandler, const std::chrono::duration<long long>& timeout,
                                    const std::string& searchTarget
)
{
	boost::asio::io_service ios;
	const auto ssdpClient = boost::make_shared<CSsdpClient>(ios, searchTarget, timeout);

	ios.reset();
	ios.run();

	auto descriptionUrls = ssdpClient->getDescriptionUrls();
	auto deviceDescription = getDevicesDescription(descriptionUrls);

	CSsdpDiscoveredDevice discoveredDevice(deviceDescription);

	ios.stop();
	return discoveredDeviceHandler(discoveredDevice);
}
