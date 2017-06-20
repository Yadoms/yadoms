#include "stdafx.h"
#include "masterDeviceConfiguration.h"
#include <boost/lexical_cast.hpp>
#include <shared/encryption/Xor.h>

CmasterDeviceConfiguration::CmasterDeviceConfiguration()
{}

CmasterDeviceConfiguration::~CmasterDeviceConfiguration()
{}

void CmasterDeviceConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

Poco::Net::SocketAddress CmasterDeviceConfiguration::getIPAddressWithSocket() const
{
   return Poco::Net::SocketAddress(m_data.get<std::string>("IPAddress"), m_data.get<std::string>("Port"));
}

std::string CmasterDeviceConfiguration::getPassword() const
{
   return shared::encryption::CXor::decryptBase64(m_data.get<std::string>("authentication.content.Password"));
}

std::string CmasterDeviceConfiguration::getUser() const
{
   return m_data.get<std::string>("authentication.content.User");
}