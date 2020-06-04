#pragma once
#include <string>
#include "SsdpDiscoveredDevice.h"

static std::chrono::duration<long long> SsdpClientTimeout(std::chrono::seconds(45));

class CSsdpDiscoverService
{
public:

	explicit CSsdpDiscoverService(std::string& searchTarget);

	virtual ~CSsdpDiscoverService() = default;

	static bool discover(const boost::function<bool(CSsdpDiscoveredDevice& discoveredDevice)>& discoveredDeviceHandler,
	                     const std::chrono::duration<long long>& timeout = SsdpClientTimeout,
	                     const std::string& searchTarget = "ssdp:all");

private:
	static std::vector<std::string> getDevicesDescription(std::vector<std::string>& descriptionUrl);
};
