#include "stdafx.h"
#include "IPX800Configuration.h"
#include <boost/lexical_cast.hpp>
#include <shared/encryption/Xor.h>

CIPX800Configuration::CIPX800Configuration()
{}

CIPX800Configuration::~CIPX800Configuration()
{}

void CIPX800Configuration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

Poco::Net::SocketAddress CIPX800Configuration::getIPAddressWithSocket() const
{
   auto value = m_data.get<std::string>("IPAddress");
   value.append(":");
   value.append(m_data.get<std::string>("Port"));

   return Poco::Net::SocketAddress(value);
}

bool CIPX800Configuration::isPasswordActivated() const
{
   return m_data.get<bool>("authentication.checkbox");
}

std::string CIPX800Configuration::getPassword() const
{
   return shared::encryption::CXor::decryptBase64(m_data.get<std::string>("authentication.content.Password"));
}