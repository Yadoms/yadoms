#pragma once
#include <string>

class CSsdpDiscoveredDevice
{
public:
	explicit CSsdpDiscoveredDevice(const std::string& deviceDescription);
	virtual ~CSsdpDiscoveredDevice() = default;
	
	std::string findTag(const std::string& tagName) const;
	std::string getIp() const;
private:
	const std::string m_deviceDescription;
};
