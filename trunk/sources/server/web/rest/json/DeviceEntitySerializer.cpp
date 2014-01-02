#include "stdafx.h"
#include "DeviceEntitySerializer.h"
#include "tools/Exceptions/NotImplementedException.hpp"
#include "DeviceEntityJsonDefinition.h"


CDeviceEntitySerializer::CDeviceEntitySerializer()
{
}

CDeviceEntitySerializer::~CDeviceEntitySerializer()
{
}

// IEntitySerializer implementation
CJson CDeviceEntitySerializer::serialize(const CDevice & object)
{
   CJson result;
   result.put(CDeviceEntityJsonDefinition::getIdIdentifier(), object.getId());
   result.put(CDeviceEntityJsonDefinition::getDataSourceIdentifier(), object.getDataSource());
   result.put(CDeviceEntityJsonDefinition::getNameIdentifier(), object.getName());
   result.put(CDeviceEntityJsonDefinition::getConfigurationIdentifier(), object.getConfiguration());
   return result;
}
// [END] IEntitySerializer implementation


// IEntityDeserializer implementation
CDevice CDeviceEntitySerializer::deserialize(const CJson & object)
{
   CDevice entity;

   if(object.find(CDeviceEntityJsonDefinition::getIdIdentifier()) != object.not_found())
      entity.setId(object.get<int>(CDeviceEntityJsonDefinition::getIdIdentifier()));

   if(object.find(CDeviceEntityJsonDefinition::getDataSourceIdentifier()) != object.not_found())
      entity.setDataSource(object.get<std::string>(CDeviceEntityJsonDefinition::getDataSourceIdentifier()));

   if(object.find(CDeviceEntityJsonDefinition::getNameIdentifier()) != object.not_found())
      entity.setName(object.get<std::string>(CDeviceEntityJsonDefinition::getNameIdentifier()));

   if(object.find(CDeviceEntityJsonDefinition::getConfigurationIdentifier()) != object.not_found())
      entity.setConfiguration(object.get<std::string>(CDeviceEntityJsonDefinition::getConfigurationIdentifier()));

   return entity;
}

// [END] IEntityDeserializer implementation