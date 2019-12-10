#include "stdafx.h"
#include "deviceConfiguration.h"
#include <boost/lexical_cast.hpp>
#include <shared/encryption/Xor.h>

CdeviceConfiguration::CdeviceConfiguration()
{}

CdeviceConfiguration::~CdeviceConfiguration()
{}

void CdeviceConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

Poco::Net::SocketAddress CdeviceConfiguration::getIPAddressWithSocket() const
{
   return Poco::Net::SocketAddress(m_data.get<std::string>("IPAddress"), m_data.get<std::string>("Port"));
}

std::string CdeviceConfiguration::getPassword() const
{
   return shared::encryption::CXor::decryptBase64(m_data.get<std::string>("authentication.content.Password"));
}

std::string CdeviceConfiguration::getUser() const
{
   return m_data.get<std::string>("authentication.content.User");
}