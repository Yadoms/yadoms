#include "stdafx.h"
#include "IPX800Configuration.h"
#include <boost/lexical_cast.hpp>

CIPX800Configuration::CIPX800Configuration()
{}

CIPX800Configuration::~CIPX800Configuration()
{}

void CIPX800Configuration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

Poco::Net::IPAddress CIPX800Configuration::GetIPAddress() const
{ 
   auto value = m_data.get<std::string>("IPAddress");
   return Poco::Net::IPAddress(value);
}

Poco::Net::SocketAddress CIPX800Configuration::GetSocket() const
{
   auto value = m_data.get<std::string>("Port");
   return Poco::Net::SocketAddress(value);
}

std::string CIPX800Configuration::GetAccount() const
{
   return m_data.get<std::string>("Account");
}

std::string CIPX800Configuration::GetPassword() const
{
   return m_data.get<std::string>("Password");
}