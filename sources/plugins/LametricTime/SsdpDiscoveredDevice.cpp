#include "SsdpDiscoveredDevice.h"

CSsdpDiscoveredDevice::CSsdpDiscoveredDevice()
{
}

CSsdpDiscoveredDevice::CSsdpDiscoveredDevice(std::string& deviceDescription)
	: m_deviceDescription(deviceDescription)
{
}

std::string CSsdpDiscoveredDevice::findTag(const std::string& tagName) const
{
	const boost::regex reg("<" + tagName + ">(.+?)</" + tagName + ">");
	boost::smatch match;

	if (!boost::regex_search(m_deviceDescription, match, reg))
	{
		return "";
	}
	return match[1].str();
}

std::string CSsdpDiscoveredDevice::getIp() const
{
	const boost::regex reg("(\\d{1,3}(\\.\\d{1,3}){3})");

	boost::smatch match;

	if (!boost::regex_search(m_deviceDescription, match, reg))
	{
		return "";
	}
	return match[1].str();
}
