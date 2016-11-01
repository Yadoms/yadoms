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
   return shared::encryption::CXor::decryptBase64(m_data.get<std::string>("Password"));
}

shared::CDataContainer CIPX800Configuration::GetConfiguredEquipments() const
{
   shared::CDataContainer equipments;

   // Base configuration
   equipments.set("relays", "8");
   equipments.set("analog", "4");
   equipments.set("Digital input", "8");

   // TODO : Add configuration for extensions added

   return equipments;
}