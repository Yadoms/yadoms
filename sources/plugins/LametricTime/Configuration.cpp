#include "stdafx.h"
#include "Configuration.h"
#include <shared/Log.h>

void CConfiguration::initializeWith(const shared::CDataContainer& data)
{
	m_configuration.initializeWith(data);
}


void CConfiguration::trace() const
{
	try
	{
		// Get simple parameters
		YADOMS_LOG(information) << "Lametric Plugin configuration, parameter 'StringParameter' is "
			<< (m_configuration.get<std::string>("StringParameter").empty()
				    ? "empty"
				    : m_configuration.get<std::string>("StringParameter"));
	}
	catch (const shared::exception::CInvalidParameter& e)
	{
		YADOMS_LOG(error) << "Parameter not found : " << e.what();
	}
	catch (const shared::exception::COutOfRange& e)
	{
		YADOMS_LOG(error) << "Parameter value out of range : " << e.what();
	}
}

std::string CConfiguration::getIPAddress() const
{
	if (getPairingMode() != kAuto)
		return m_configuration.get<std::string>("PairingMode.content.Manual.content.IPAddress");
	return m_ipAddress;
}

void CConfiguration::setIPAddress(std::string ipAddress)
{
	m_ipAddress = ipAddress;
}

EPortType CConfiguration::getPort() const
{
	static const shared::CDataContainer::EnumValuesNames EPortNumber = boost::assign::map_list_of("Http", kHttp)(
		"Https", kHttps);

	return getPairingMode() == kAuto
		       ? m_configuration.getEnumValue<EPortType>("PairingMode.content.Automatic.content.Port", EPortNumber)
		       : m_configuration.getEnumValue<EPortType>("PairingMode.content.Manual.content.Port", EPortNumber);
}

std::string CConfiguration::getAPIKey() const
{
	return m_configuration.get<std::string>("APIKey");
}

EPairingMode CConfiguration::getPairingMode() const
{
	return m_configuration.get<std::string>("PairingMode.activeSectionText") == "Auto" ? kAuto : kManual;
}
