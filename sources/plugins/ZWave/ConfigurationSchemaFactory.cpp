#include "stdafx.h"
#include "ConfigurationSchemaFactory.h"
#include <shared/exception/NotSupported.hpp>
#include "OpenZWaveNodeKeywordBase.h"
#include "OpenZWaveNodeKeywordGeneric.h"

shared::CDataContainerSharedPtr CConfigurationSchemaFactory::generateForHistorizer(boost::shared_ptr<IOpenZWaveNodeKeyword> historizer)
{
   if (boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<double> >(historizer))
   {
      return CConfigurationSchemaFactory::generateForDouble(historizer->getTypeInformation()->serialize(), historizer->getCommandClass());
   }

   if (boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<Poco::Int8> >(historizer) ||
      boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<short> >(historizer) ||
      boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<Poco::Int32> >(historizer) ||
      boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<Poco::UInt8> >(historizer)
      )
   {
      return CConfigurationSchemaFactory::generateForInteger(historizer->getTypeInformation()->serialize(), historizer->getCommandClass());
   }

   if (boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<bool> >(historizer))
   {
      return CConfigurationSchemaFactory::generateForBool(historizer->getTypeInformation()->serialize(), historizer->getCommandClass());
   }
   
   if (boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<std::string> >(historizer))
   {
      return CConfigurationSchemaFactory::generateForString(historizer->getTypeInformation()->serialize(), historizer->getCommandClass());
   }
   
   if (boost::dynamic_pointer_cast< COpenZWaveNodeKeywordGeneric<COpenZWaveEnumHandler> >(historizer))
   {
      return CConfigurationSchemaFactory::generateForEnum(historizer->getTypeInformation()->serialize(), historizer->getCommandClass());
   }
   
   throw shared::exception::CNotSupported("This historizer");
}

std::string CConfigurationSchemaFactory::generateValidKeyName(const std::string & val)
{
   return boost::replace_all_copy(val, ".", "_");
}

shared::CDataContainerSharedPtr CConfigurationSchemaFactory::generateForDouble(shared::CDataContainerSharedPtr zwaveTypeInfo, ECommandClass commandclass)
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
   shared::CDataContainerSharedPtr options = new_CDataContainerSharedPtr();
   options->set("type", "decimal");
   options->set("name", zwaveTypeInfo->get<std::string>("name"));
   options->set("description", generateDescription(zwaveTypeInfo, commandclass));

   if (zwaveTypeInfo->containsValue("min"))
      options->set("minimumValue", zwaveTypeInfo->get<std::string>("min"));
   if (zwaveTypeInfo->containsValue("max"))
      options->set("maximumValue", zwaveTypeInfo->get<std::string>("max"));
   if(zwaveTypeInfo->containsValue("precision"))
      options->set("precision", zwaveTypeInfo->get<std::string>("precision"));

   return options;
}

shared::CDataContainerSharedPtr CConfigurationSchemaFactory::generateForInteger(shared::CDataContainerSharedPtr zwaveTypeInfo, ECommandClass commandclass)
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
   shared::CDataContainerSharedPtr options = new_CDataContainerSharedPtr();
   options->set("type", "int");
   options->set("name", zwaveTypeInfo->get<std::string>("name"));
   options->set("description", generateDescription(zwaveTypeInfo, commandclass));

   if (zwaveTypeInfo->containsValue("min"))
      options->set("minimumValue", zwaveTypeInfo->get<std::string>("min"));
   if (zwaveTypeInfo->containsValue("max"))
      options->set("maximumValue", zwaveTypeInfo->get<std::string>("max"));

   //disable verification-> workaround for #248
   options->set("disableCheck", true);
   

   return options;
}
shared::CDataContainerSharedPtr CConfigurationSchemaFactory::generateForBool(shared::CDataContainerSharedPtr zwaveTypeInfo, ECommandClass commandclass)
{
   /*
   "enabled" : {
      "type" : "bool",
      "name" : "enabled",
      "description" : "Permit to enable the widget",
      "defaultValue" : "true"
   }

   */
   shared::CDataContainerSharedPtr options = new_CDataContainerSharedPtr();
   options->set("type", "bool");
   options->set("name", zwaveTypeInfo->get<std::string>("name"));
   options->set("description", generateDescription(zwaveTypeInfo, commandclass));
   return options;
}
shared::CDataContainerSharedPtr CConfigurationSchemaFactory::generateForString(shared::CDataContainerSharedPtr zwaveTypeInfo, ECommandClass commandclass)
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
   shared::CDataContainerSharedPtr options = new_CDataContainerSharedPtr();
   options->set("type", "string");
   options->set("name", zwaveTypeInfo->get<std::string>("name"));
   options->set("description", generateDescription(zwaveTypeInfo, commandclass));
   return options;
}
shared::CDataContainerSharedPtr CConfigurationSchemaFactory::generateForEnum(shared::CDataContainerSharedPtr zwaveTypeInfo, ECommandClass commandclass)
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

   shared::CDataContainerSharedPtr options = new_CDataContainerSharedPtr();
   options->set("type", "enum");
   options->set("name", zwaveTypeInfo->get<std::string>("name"));
   options->set("description", generateDescription(zwaveTypeInfo, commandclass));

   if (zwaveTypeInfo->containsChild("values"))
      options->set("values", zwaveTypeInfo->get<shared::CDataContainer>("values"));

   return options;
}

std::string CConfigurationSchemaFactory::generateDescription(shared::CDataContainerSharedPtr zwaveTypeInfo, ECommandClass commandclass)
{
	std::string descr = (boost::format("0x%02X %s") % commandclass.toInteger() % commandclass.toString()).str();
	if (zwaveTypeInfo->containsValue("description")) {
		descr += " : " + zwaveTypeInfo->get<std::string>("description");
	}
	return descr;
}