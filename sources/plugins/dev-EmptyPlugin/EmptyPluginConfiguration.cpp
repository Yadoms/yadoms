#include "stdafx.h"
#include "EmptyPluginConfiguration.h"
#include <shared/Log.h>

CEmptyPluginConfiguration::~CEmptyPluginConfiguration()
{
}

void CEmptyPluginConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_configuration.initializeWith(data);
}

/*
    You will find here an example how to retrieve enum variables from the configuration
*/

EEnumType CEmptyPluginConfiguration::getEnumParameter() const
{
   // Enum type, declare keys labels
   static const shared::CDataContainer::EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
      ("EnumValue1", kEnumValue1)
      ("EnumValue2", kEnumValue2)
      ("EnumValue3", kEnumValue3);

   return m_configuration.getEnumValue<EEnumType>("EnumParameter", EEnumTypeNames);
}

void CEmptyPluginConfiguration::trace() const
{
   try
   {
      // Get simple parameters
      YADOMS_LOG(information) << "EmptyPlugin configuration, parameter 'StringParameter' is "
         << (m_configuration.get<std::string>("StringParameter").empty() ? "empty" : m_configuration.get<std::string>("StringParameter"))
        ;
      YADOMS_LOG(information) << "EmptyPlugin configuration, parameter 'BoolParameter' is " << m_configuration.get<bool>("BoolParameter");
      YADOMS_LOG(information) << "EmptyPlugin configuration, parameter 'DecimalParameter' is " << m_configuration.get<double>("DecimalParameter");
      YADOMS_LOG(information) << "EmptyPlugin configuration, parameter 'IntParameter' is " << m_configuration.get<int>("IntParameter");

      // Enum
      YADOMS_LOG(information) << "EmptyPlugin configuration, parameter 'EnumParameter' is " << getEnumParameter();
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