#include "stdafx.h"

#include "JsonSerializers.h"
#include "JsonSerializersHelpers.hpp"
#include <shared/plugin/information/IInformation.h>
#include <shared/serialization/PTreeToJsonSerializer.h>
#include <shared/exception/InvalidParameter.hpp>
#include "database/entities/Entities.h"

namespace web { namespace rest { namespace json {
   
   DECLARE_JSON_ENTITY_IMPLEMENTATION(Plugin, "Plugin",
      ((Id)("id")JSON_INT)
      ((Name)("name")JSON_STRING)
      ((PluginName)("pluginName")JSON_STRING)
      ((Configuration)("configuration")JSON_STRING)
      ((Enabled)("enabled")JSON_BOOL)
   )


   DECLARE_JSON_ENTITY_IMPLEMENTATION(Configuration,  "Configuration",
      ((Section)("section")JSON_STRING)
      ((Name)("name")JSON_STRING)
      ((Value)("value")JSON_STRING)
      ((DefaultValue)("default_value")JSON_STRING)
      ((Description)("description")JSON_STRING)
      ((SecurityAccess)("securityAccess")JSON_ENUM(database::entities::ESecurityAccess))
      ((LastModificationDate)("last_modification_date")JSON_DATE)
   )


   DECLARE_JSON_ENTITY_IMPLEMENTATION(Acquisition,  "Acquisition",
      ((Id)("id")JSON_INT)
      ((Source)("source")JSON_STRING)
      ((Keyword)("keyword")JSON_STRING)
      ((Value)("value")JSON_STRING)
      ((Date)("date")JSON_DATE)
   )


   DECLARE_JSON_ENTITY_IMPLEMENTATION(Device,  "Device",
      ((Id)("id")JSON_INT)
      ((DataSource)("data_source")JSON_STRING)
      ((Name)("name")JSON_STRING)
      ((Configuration)("configuration")JSON_STRING)
   )


   DECLARE_JSON_ENTITY_IMPLEMENTATION(Keyword,  "Keyword",
      ((Name)("name")JSON_STRING)
   )

   DECLARE_JSON_ENTITY_IMPLEMENTATION(Page,  "Page",
      ((Id)("id")JSON_INT)
      ((Name)("name")JSON_STRING)
      ((PageOrder)("pageOrder")JSON_INT)
   )

   DECLARE_JSON_ENTITY_IMPLEMENTATION(Widget,  "Widget",
      ((Id)("id")JSON_INT)
      ((IdPage)("idPage")JSON_INT)
      ((Name)("name")JSON_STRING)
      ((SizeX)("sizeX")JSON_INT)
      ((SizeY)("sizeY")JSON_INT)
      ((PositionX)("positionX")JSON_INT)
      ((PositionY)("positionY")JSON_INT)
      ((Configuration)("configuration")JSON_PTREE)
   )
   
   

//std::string CWidgetEntityJsonDefinition::m_IdIdentifier = "id" ;
//std::string CWidgetEntityJsonDefinition::m_IdPageIdentifier = "idPage" ;
//std::string CWidgetEntityJsonDefinition::m_NameIdentifier = "name" ;
//std::string CWidgetEntityJsonDefinition::m_SizeXIdentifier = "sizeX" ;
//std::string CWidgetEntityJsonDefinition::m_SizeYIdentifier = "sizeY" ;
//std::string CWidgetEntityJsonDefinition::m_PositionXIdentifier = "positionX" ;
//std::string CWidgetEntityJsonDefinition::m_PositionYIdentifier = "positionY" ;
//std::string CWidgetEntityJsonDefinition::m_ConfigurationIdentifier = "configuration" ;
//
//   CWidgetEntitySerializer::CWidgetEntitySerializer() {
//   }
//   CWidgetEntitySerializer::~CWidgetEntitySerializer() {
//   }
//   CJson CWidgetEntitySerializer::serialize(const database::entities::CWidget & object) {
//	   CJson result;
//	   result.put(CWidgetEntityJsonDefinition::getIdIdentifier(), object.getId());
//	   result.put(CWidgetEntityJsonDefinition::getIdPageIdentifier(), object.getIdPage());
//	   result.put(CWidgetEntityJsonDefinition::getNameIdentifier(), object.getName());
//	   result.put(CWidgetEntityJsonDefinition::getSizeXIdentifier(), object.getSizeX());
//	   result.put(CWidgetEntityJsonDefinition::getSizeYIdentifier(), object.getSizeY());
//	   result.put(CWidgetEntityJsonDefinition::getPositionXIdentifier(), object.getPositionX());
//	   result.put(CWidgetEntityJsonDefinition::getPositionYIdentifier(), object.getPositionY());
//	   //result.put(CWidgetEntityJsonDefinition::getConfigurationIdentifier(), object.getConfiguration());
//
//      
//      result.add_child(CWidgetEntityJsonDefinition::getConfigurationIdentifier(), CJsonSerializer::deserialize(object.getConfiguration()));
//	   return result;
//   }
//   boost::shared_ptr< database::entities::CWidget > CWidgetEntitySerializer::deserialize(const CJson & object) {
//	   boost::shared_ptr< database::entities::CWidget > entity(new database::entities::CWidget() );
//	   if(object.find(CWidgetEntityJsonDefinition::getIdIdentifier()) != object.not_found()) entity->setId(object.get<int >(CWidgetEntityJsonDefinition::getIdIdentifier()));
//	   if(object.find(CWidgetEntityJsonDefinition::getIdPageIdentifier()) != object.not_found()) entity->setIdPage(object.get<int >(CWidgetEntityJsonDefinition::getIdPageIdentifier()));
//	   if(object.find(CWidgetEntityJsonDefinition::getNameIdentifier()) != object.not_found()) entity->setName(object.get<std::string >(CWidgetEntityJsonDefinition::getNameIdentifier()));
//	   if(object.find(CWidgetEntityJsonDefinition::getSizeXIdentifier()) != object.not_found()) entity->setSizeX(object.get<int >(CWidgetEntityJsonDefinition::getSizeXIdentifier()));
//	   if(object.find(CWidgetEntityJsonDefinition::getSizeYIdentifier()) != object.not_found()) entity->setSizeY(object.get<int >(CWidgetEntityJsonDefinition::getSizeYIdentifier()));
//	   if(object.find(CWidgetEntityJsonDefinition::getPositionXIdentifier()) != object.not_found()) entity->setPositionX(object.get<int >(CWidgetEntityJsonDefinition::getPositionXIdentifier()));
//	   if(object.find(CWidgetEntityJsonDefinition::getPositionYIdentifier()) != object.not_found()) entity->setPositionY(object.get<int >(CWidgetEntityJsonDefinition::getPositionYIdentifier()));
//	   if(object.find(CWidgetEntityJsonDefinition::getConfigurationIdentifier()) != object.not_found()) 
//      {
//            entity->setConfiguration(CJsonSerializer::serialize(object.get_child(CWidgetEntityJsonDefinition::getConfigurationIdentifier())));
//      }
//	   return entity;
//   }

   DECLARE_JSON_ENTITY_IMPLEMENTATION(PluginEventLogger,  "PluginEventLogger",
      ((Id)("id")JSON_INT)
      ((EventDate)("eventDate")JSON_DATE)
      ((PluginName)("pluginName")JSON_STRING)
      ((PluginVersion)("pluginVersion")JSON_STRING)
      ((PluginRelease)("pluginRelease")JSON_ENUM(shared::plugin::information::EReleaseType))
      ((EventType)("eventType")JSON_ENUM(database::entities::EEventType))
      ((Message)("message")JSON_STRING)
   )

   DECLARE_JSON_ENTITY_IMPLEMENTATION(EventLogger,  "EventLogger",
      ((Id)("id")JSON_INT)
      ((Date)("date")JSON_DATE)
      ((Code)("code")JSON_STRING)
      ((OptionalData)("optionalData")JSON_STRING)
   )

} //namespace json
} //namespace rest
} //namespace web 

