#include "stdafx.h"
#include "ConfigurationSchemaFactory.h"
#include <shared/exception/NotSupported.hpp>
#include "OpenZWaveNodeKeywordBase.h"
#include "OpenZWaveNodeKeywordGeneric.h"
#include "OpenZWaveNodeKeywordDimmable.h"

shared::CDataContainer CConfigurationSchemaFactory::generateForHistorizer(boost::shared_ptr<IOpenZWaveNodeKeyword> historizer)
{
   auto kwDouble = boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<double> >(historizer);
   if (kwDouble)
   {
      return CConfigurationSchemaFactory::generateForDouble(kwDouble->getKeyword()->getKeyword()->getTypeInfo());
   }

   auto kwInt8 = boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<Poco::Int8> >(historizer);
   if (kwInt8)
   {
      return CConfigurationSchemaFactory::generateForInteger(kwInt8->getKeyword()->getKeyword()->getTypeInfo());
   }

   auto kwShort = boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<short> >(historizer);
   if (kwShort)
   {
      return CConfigurationSchemaFactory::generateForInteger(kwShort->getKeyword()->getKeyword()->getTypeInfo());
   }
   
   auto kwInt32 = boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<Poco::Int32> >(historizer);
   if (kwInt32)
   {
      return CConfigurationSchemaFactory::generateForInteger(kwInt32->getKeyword()->getKeyword()->getTypeInfo());
   }
   
   auto kwU8 = boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<Poco::UInt8> >(historizer);
   if (kwU8)
   {
      return CConfigurationSchemaFactory::generateForInteger(kwU8->getKeyword()->getKeyword()->getTypeInfo());
   }
   
   auto kwBool = boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<bool> >(historizer);
   if (kwBool)
   {
      return CConfigurationSchemaFactory::generateForBool(kwBool->getKeyword()->getKeyword()->getTypeInfo());
   }
   
   auto kwString = boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<std::string> >(historizer);
   if (kwString)
   {
      return CConfigurationSchemaFactory::generateForString(kwString->getKeyword()->getKeyword()->getTypeInfo());
   }
   
   auto kwEnum = boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<COpenZWaveEnumHandler> >(historizer);
   if (kwEnum)
   {
      return CConfigurationSchemaFactory::generateForString(kwEnum->getKeyword()->getKeyword()->getTypeInfo());
   }
   
   throw shared::exception::CNotSupported("This historizer");
}

std::string CConfigurationSchemaFactory::generateValidKeyName(const std::string & val)
{
   return boost::replace_all_copy(val, ".", "_");
}

shared::CDataContainer CConfigurationSchemaFactory::generateForDouble(shared::CDataContainer zwaveTypeInfo)
{
/*
   "CurrentCoefficient" : {
      "type" : "decimal",
      "name" : "correction coefficient",
      "description" : "Set a correction coefficient to adjust the value",
      "defaultValue" : "0.0",
      "maximumValue" : "1.0",
      "minimumValue" : "-1.0",
      "precision" : "2"
   }
*/
   shared::CDataContainer options;
   options.set("type", "decimal");
   options.set("name", zwaveTypeInfo.get("name"));

   if (zwaveTypeInfo.containsValue("description"))
      options.set("description", zwaveTypeInfo.get("description"));
   if (zwaveTypeInfo.containsValue("min"))
      options.set("minimumValue", zwaveTypeInfo.get("min"));
   if (zwaveTypeInfo.containsValue("max"))
      options.set("maximumValue", zwaveTypeInfo.get("max"));
   if(zwaveTypeInfo.containsValue("precision"))
      options.set("precision", zwaveTypeInfo.get("precision"));

   return options;
}

shared::CDataContainer CConfigurationSchemaFactory::generateForInteger(shared::CDataContainer zwaveTypeInfo)
{
   /*
   "offsetHour" : {
   "type" : "int",
   "name" : "hour offset",
   "description" : "used to make an offset on the time in hour",
   "defaultValue" : "0",
   "maximumValue" : "23",
   "minimumValue" : "0"
   }

   */
   shared::CDataContainer options;
   options.set("type", "int");
   options.set("name", zwaveTypeInfo.get("name"));

   if (zwaveTypeInfo.containsValue("description"))
      options.set("description", zwaveTypeInfo.get("description"));
   if (zwaveTypeInfo.containsValue("min"))
      options.set("minimumValue", zwaveTypeInfo.get("min"));
   if (zwaveTypeInfo.containsValue("max"))
      options.set("maximumValue", zwaveTypeInfo.get("max"));

   return options;
}
shared::CDataContainer CConfigurationSchemaFactory::generateForBool(shared::CDataContainer zwaveTypeInfo)
{
   /*
   "enabled" : {
      "type" : "bool",
      "name" : "enabled",
      "description" : "Permit to enable the widget",
      "defaultValue" : "true"
   }

   */
   shared::CDataContainer options;
   options.set("type", "bool");
   options.set("name", zwaveTypeInfo.get("name"));

   if (zwaveTypeInfo.containsValue("description"))
      options.set("description", zwaveTypeInfo.get("description"));

   return options;
}
shared::CDataContainer CConfigurationSchemaFactory::generateForString(shared::CDataContainer zwaveTypeInfo)
{
   /*
   "ipAddress" : {
      "type" : "string",
      "name" : "IP Address",
      "description" : "Set the IP Address of the peripheral",
      "regex" : "([0-9]{1,3}\\.){3}[0-9]{1,3}",
      "regexErrorMessage" : "Must be an ipv4 adress like 192.168.0.12",
      "required" : "true"
   }
   */
   shared::CDataContainer options;
   options.set("type", "string");
   options.set("name", zwaveTypeInfo.get("name"));

   if (zwaveTypeInfo.containsValue("description"))
      options.set("description", zwaveTypeInfo.get("description"));

   return options;
}
shared::CDataContainer CConfigurationSchemaFactory::generateForEnum(shared::CDataContainer zwaveTypeInfo)
{
   /*
   "timeFormat" : {
      "name" : "time format",
      "description" : "Permit to change the time format of the clock",
      "type" : "enum",
      "values" : {
         "12H" : "12 hours",
         "24H" : "24 hours"
      },
      "defaultValue" : "12H"
   }
   */

   shared::CDataContainer options;
   options.set("type", "enum");
   options.set("name", zwaveTypeInfo.get("name"));

   if (zwaveTypeInfo.containsValue("description"))
      options.set("description", zwaveTypeInfo.get("description"));

   if (zwaveTypeInfo.containsChild("values"))
      options.set("values", zwaveTypeInfo.get<shared::CDataContainer>("values"));

   return options;
}