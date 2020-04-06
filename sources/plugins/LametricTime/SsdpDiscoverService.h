#pragma once
#include <string>
#include "SsdpDiscoveredDevice.h"

class CSsdpDiscoverService
{
public:
	
	explicit CSsdpDiscoverService(std::string& searchTarget);

	virtual ~CSsdpDiscoverService() = default;
	
	static bool discover(std::string searchTarget, const boost::function<bool(CSsdpDiscoveredDevice& discoveredDevice)>& discoveredDeviceHandler);

private:
	static std::string getDeviceDescription(std::string& descriptionUrl);
};
