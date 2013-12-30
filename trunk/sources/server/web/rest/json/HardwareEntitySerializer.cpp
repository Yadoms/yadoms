#include "stdafx.h"
#include "HardwareEntitySerializer.h"
#include "tools/Exceptions/NotImplementedException.hpp"

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
   result.put("id", object.getId());
   result.put("enabled", object.getEnabled());
   result.put("deleted", object.getDeleted());
   result.put("configuration", object.getConfiguration());
   result.put("name", object.getName());
   result.put("pluginName", object.getPluginName());
   return result;
}
// [END] IEntitySerializer implementation


// IEntityDeserializer implementation
CHardware CHardwareEntitySerializer::deserialize(const CJson & object)
{
   CHardware entity;

   if(object.find("id") != object.not_found())
      entity.setId(object.get<int>("id"));

   if(object.find("enabled") != object.not_found())
      entity.setEnabled(object.get<bool>("enabled"));

   if(object.find("deleted") != object.not_found())
      entity.setDeleted(object.get<bool>("deleted"));

   if(object.find("configuration") != object.not_found())
      entity.setConfiguration(object.get<std::string>("configuration"));

   if(object.find("name") != object.not_found())
      entity.setName(object.get<std::string>("name"));

   if(object.find("pluginName") != object.not_found())
      entity.setPluginName(object.get<std::string>("pluginName"));

   return entity;
}

// [END] IEntityDeserializer implementation