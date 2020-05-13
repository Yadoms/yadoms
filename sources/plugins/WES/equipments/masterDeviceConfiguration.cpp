#include "stdafx.h"
#include "masterDeviceConfiguration.h"
#include <boost/lexical_cast.hpp>
#include <shared/encryption/Xor.h>

CmasterDeviceConfiguration::CmasterDeviceConfiguration()
   :m_data(shared::CDataContainer::make())
{}

CmasterDeviceConfiguration::~CmasterDeviceConfiguration()
{}

void CmasterDeviceConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
{
   m_data->initializeWith(data);
}

Poco::Net::SocketAddress CmasterDeviceConfiguration::getIPAddressWithSocket() const
{
   return Poco::Net::SocketAddress(m_data->get<std::string>("IPAddress"), m_data->get<std::string>("Port"));
}

std::string CmasterDeviceConfiguration::getPassword() const
{
   return shared::encryption::CXor::decryptBase64(m_data->get<std::string>("authentication.content.Password"));
}

bool CmasterDeviceConfiguration::credentialActivated() const {
	return m_data->getWithDefault<bool>("authentication.checkbox", true);
}

std::string CmasterDeviceConfiguration::getUser() const
{
   return m_data->get<std::string>("authentication.content.User");
}

bool CmasterDeviceConfiguration::isAnalogInputsActivated() const{
	return m_data->exists("analog");
}

std::string CmasterDeviceConfiguration::analogInputsType(int index) const{
   return m_data->get<std::string>("analog.content.ana" + boost::lexical_cast<std::string>(index));
}