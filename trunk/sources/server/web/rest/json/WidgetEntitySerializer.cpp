#include "stdafx.h"
#include "WidgetEntitySerializer.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "WidgetEntityJsonDefinition.h"


CWidgetEntitySerializer::CWidgetEntitySerializer()
{
}

CWidgetEntitySerializer::~CWidgetEntitySerializer()
{
}

// IEntitySerializer implementation
CJson CWidgetEntitySerializer::serialize(const CWidget & object)
{
   CJson result;
   result.put(CWidgetEntityJsonDefinition::getIdIdentifier(), object.getId());
   result.put(CWidgetEntityJsonDefinition::getIdPageIdentifier(), object.getIdPage());
   result.put(CWidgetEntityJsonDefinition::getNameIdentifier(), object.getName());
   result.put(CWidgetEntityJsonDefinition::getSizeXIdentifier(), object.getSizeX());
   result.put(CWidgetEntityJsonDefinition::getSizeYIdentifier(), object.getSizeY());
   result.put(CWidgetEntityJsonDefinition::getConfigurationIdentifier(), object.getConfiguration());
   return result;
}
// [END] IEntitySerializer implementation


// IEntityDeserializer implementation
CWidget CWidgetEntitySerializer::deserialize(const CJson & object)
{
   CWidget entity;

   if(object.find(CWidgetEntityJsonDefinition::getIdIdentifier()) != object.not_found())
      entity.setId(object.get<int>(CWidgetEntityJsonDefinition::getIdIdentifier()));

   if(object.find(CWidgetEntityJsonDefinition::getIdPageIdentifier()) != object.not_found())
      entity.setIdPage(object.get<int>(CWidgetEntityJsonDefinition::getIdPageIdentifier()));

   if(object.find(CWidgetEntityJsonDefinition::getNameIdentifier()) != object.not_found())
      entity.setName(object.get<std::string>(CWidgetEntityJsonDefinition::getNameIdentifier()));

   if(object.find(CWidgetEntityJsonDefinition::getSizeXIdentifier()) != object.not_found())
      entity.setSizeX(object.get<int>(CWidgetEntityJsonDefinition::getSizeXIdentifier()));

   if(object.find(CWidgetEntityJsonDefinition::getSizeYIdentifier()) != object.not_found())
      entity.setSizeY(object.get<int>(CWidgetEntityJsonDefinition::getSizeYIdentifier()));
      
   if(object.find(CWidgetEntityJsonDefinition::getConfigurationIdentifier()) != object.not_found())
      entity.setConfiguration(object.get<std::string>(CWidgetEntityJsonDefinition::getConfigurationIdentifier()));

   return entity;
}

// [END] IEntityDeserializer implementation