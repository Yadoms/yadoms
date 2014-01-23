#include "stdafx.h"
#include "HardwareEntitySerializer.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "HardwareEntityJsonDefinition.h"




CHardwareEntitySerializer::CHardwareEntitySerializer()
{
}

CHardwareEntitySerializer::~CHardwareEntitySerializer()
{
}

// IEntitySerializer implementation
CJson CHardwareEntitySerializer::serialize(const CHardware & object)
{
   CJson result;
   result.put(CHardwareEntityJsonDefinition::getIdIdentifier(), object.getId());
   result.put(CHardwareEntityJsonDefinition::getEnabledIdentifier(), object.getEnabled());
   result.put(CHardwareEntityJsonDefinition::getDeletedIdentifier(), object.getDeleted());
   result.put(CHardwareEntityJsonDefinition::getConfigurationIdentifier(), object.getConfiguration());
   result.put(CHardwareEntityJsonDefinition::getNameIdentifier(), object.getName());
   result.put(CHardwareEntityJsonDefinition::getPluginNameIdentifier(), object.getPluginName());
   return result;
}
// [END] IEntitySerializer implementation


// IEntityDeserializer implementation
CHardware CHardwareEntitySerializer::deserialize(const CJson & object)
{
   CHardware entity;

   if(object.find(CHardwareEntityJsonDefinition::getIdIdentifier()) != object.not_found())
      entity.setId(object.get<int>(CHardwareEntityJsonDefinition::getIdIdentifier()));

   if(object.find(CHardwareEntityJsonDefinition::getEnabledIdentifier()) != object.not_found())
      entity.setEnabled(object.get<bool>(CHardwareEntityJsonDefinition::getEnabledIdentifier()));

   if(object.find(CHardwareEntityJsonDefinition::getDeletedIdentifier()) != object.not_found())
      entity.setDeleted(object.get<bool>(CHardwareEntityJsonDefinition::getDeletedIdentifier()));

   if(object.find(CHardwareEntityJsonDefinition::getConfigurationIdentifier()) != object.not_found())
      entity.setConfiguration(object.get<std::string>(CHardwareEntityJsonDefinition::getConfigurationIdentifier()));

   if(object.find(CHardwareEntityJsonDefinition::getNameIdentifier()) != object.not_found())
      entity.setName(object.get<std::string>(CHardwareEntityJsonDefinition::getNameIdentifier()));

   if(object.find(CHardwareEntityJsonDefinition::getPluginNameIdentifier()) != object.not_found())
      entity.setPluginName(object.get<std::string>(CHardwareEntityJsonDefinition::getPluginNameIdentifier()));

   return entity;
}

// [END] IEntityDeserializer implementation