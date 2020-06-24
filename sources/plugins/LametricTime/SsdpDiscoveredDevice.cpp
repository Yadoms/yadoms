#include "SsdpDiscoveredDevice.h"

CSsdpDiscoveredDevice::CSsdpDiscoveredDevice()
{
}

CSsdpDiscoveredDevice::CSsdpDiscoveredDevice(const std::vector<std::string>& devicesDescription)
	: m_deviceDescription(devicesDescription)
{
}

std::string CSsdpDiscoveredDevice::findTag(const std::string& tagName, const int& index) const
{
	const boost::regex reg("<" + tagName + ">(.+?)</" + tagName + ">");
	boost::smatch match;

	if (!boost::regex_search(m_deviceDescription[index], match, reg))
	{
		return "";
	}
	return match[1].str();
}

std::string CSsdpDiscoveredDevice::getIp(const int& index) const
{
	const boost::regex reg("(\\d{1,3}(\\.\\d{1,3}){3})");

	boost::smatch match;

	if (!boost::regex_search(m_deviceDescription[index], match, reg))
	{
		return "";
	}
	return match[1].str();
}

std::vector<std::string> CSsdpDiscoveredDevice::getDevicesDescription() const
{
	return m_deviceDescription;
}
