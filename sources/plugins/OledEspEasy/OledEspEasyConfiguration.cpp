#include "stdafx.h"
#include "OledEspEasyConfiguration.h"
#include <shared/Log.h>

COledEspEasyConfiguration::~COledEspEasyConfiguration()
{
}

void COledEspEasyConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_configuration.initializeWith(data);
}

/*
    You will find here an example how to retrieve enum variables from the configuration
*/

EEnumType COledEspEasyConfiguration::getOledType() const
{
   // Enum type, declare keys labels
   static const shared::CDataContainer::EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
      ("Oled0_66", kOled0_66)
      ;

   return m_configuration.getEnumValue<EEnumType>("OledSize", EEnumTypeNames);
}

std::string COledEspEasyConfiguration::getIPAddress() const
{
	return m_configuration.get<std::string>("ipAddress");
}



void COledEspEasyConfiguration::trace() const
{
   try
   {
      // Get simple parameters
	   YADOMS_LOG(information) << "OledEspEasy configuration, parameter 'ipAddress' is " << (getIPAddress().empty() ? "empty" : getIPAddress());
        ;
      // Enum
      YADOMS_LOG(information) << "OledEspEasy configuration, parameter 'OledSize' is " << getOledType();
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