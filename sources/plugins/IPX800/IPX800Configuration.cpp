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
   // Enum type, declare keys labels
   //static const shared::CDataContainer::EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
   //("X8R", kX8R);

   m_data.initializeWith(data);
   /*
   try {

      //Analyze if extensions are present
      auto result = m_data.get<std::vector<shared::CDataContainer> >("extensions");
      std::vector<shared::CDataContainer>::iterator equipmentIterator;

      for (equipmentIterator = result.begin(); equipmentIterator != result.end(); ++equipmentIterator)
      {
         switch (equipmentIterator->getEnumValue<EExtension>("content.equipment", EEnumTypeNames))
         {
            case kX8R:
               m_relayQty =+8;
               break;
            default:
               break;
         }

         std::cout << "equipment: " << equipmentIterator->get<std::string>("content.equipment") << " added" << std::endl;
      }
   }
   catch (...)
   {
      std::cerr << "erreur" << std::endl;
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