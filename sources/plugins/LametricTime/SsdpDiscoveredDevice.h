#pragma once
#include <string>

class CSsdpDiscoveredDevice
{
public:
	explicit CSsdpDiscoveredDevice(const std::vector<std::string>& devicesDescription);
	virtual ~CSsdpDiscoveredDevice() = default;
	
	std::string findTag(const std::string& tagName, const int& index) const;
	std::string getIp(const int& index) const;
	std::vector<std::string> getDevicesDescription() const;
private:
	std::vector<std::string> m_deviceDescription;
};
