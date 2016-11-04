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

   data.printToLog();

   /*
   try {
      std::cout << "slot1:" << data.get<std::string>("translation.position.Slot1") << std::endl;
   }
   catch (...)
   {
      std::cout << "erreur catching creation" << std::endl;
   }*/
}

Poco::Net::IPAddress CIPX800Configuration::getIPAddress() const
{ 
   auto value = m_data.get<std::string>("IPAddress");
   return Poco::Net::IPAddress(value);
}

Poco::Net::SocketAddress CIPX800Configuration::getSocket() const
{
   auto value = m_data.get<std::string>("Port");
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