#pragma once
#include <string>

class CSsdpDiscoveredDevice
{
public:
	CSsdpDiscoveredDevice();
	explicit CSsdpDiscoveredDevice(std::string& deviceDescription);
	virtual ~CSsdpDiscoveredDevice() = default;
	
	std::string findTag(const std::string& tagName) const;
	std::string getIp() const;
private:
	std::string m_deviceDescription;
};
