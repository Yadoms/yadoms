#include "stdafx.h"
#include "PageEntitySerializer.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "PageEntityJsonDefinition.h"


CPageEntitySerializer::CPageEntitySerializer()
{
}

CPageEntitySerializer::~CPageEntitySerializer()
{
}

// IEntitySerializer implementation
CJson CPageEntitySerializer::serialize(const CPage & object)
{
   CJson result;
   result.put(CPageEntityJsonDefinition::getIdIdentifier(), object.getId());
   result.put(CPageEntityJsonDefinition::getNameIdentifier(), object.getName());
   return result;
}
// [END] IEntitySerializer implementation


// IEntityDeserializer implementation
CPage CPageEntitySerializer::deserialize(const CJson & object)
{
   CPage entity;

   if(object.find(CPageEntityJsonDefinition::getIdIdentifier()) != object.not_found())
      entity.setId(object.get<int>(CPageEntityJsonDefinition::getIdIdentifier()));

   if(object.find(CPageEntityJsonDefinition::getNameIdentifier()) != object.not_found())
      entity.setName(object.get<std::string>(CPageEntityJsonDefinition::getNameIdentifier()));

   return entity;
}

// [END] IEntityDeserializer implementation