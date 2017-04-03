#include "stdafx.h"
#include "WESConfiguration.h"
#include <boost/lexical_cast.hpp>
#include <shared/encryption/Xor.h>

CWESConfiguration::CWESConfiguration()
{}

CWESConfiguration::~CWESConfiguration()
{}

void CWESConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

Poco::Net::SocketAddress CWESConfiguration::getIPAddressWithSocket() const
{
   /*
   auto value = m_data.get<std::string>("IPAddress");
   value.append(":");
   value.append(m_data.get<std::string>("Port"));
   */
   return Poco::Net::SocketAddress("192.168.1.12:80");
}

bool CWESConfiguration::isPasswordActivated() const
{
   return false;// m_data.get<bool>("authentication.checkbox");
}

std::string CWESConfiguration::getPassword() const
{
   return "";// shared::encryption::CXor::decryptBase64(m_data.get<std::string>("authentication.content.Password"));
}