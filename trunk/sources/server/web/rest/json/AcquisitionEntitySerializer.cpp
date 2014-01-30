#include "stdafx.h"
#include "AcquisitionEntitySerializer.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "AcquisitionEntityJsonDefinition.h"

CAcquisitionEntitySerializer::CAcquisitionEntitySerializer()
{
}

CAcquisitionEntitySerializer::~CAcquisitionEntitySerializer()
{
}

// IEntitySerializer implementation
CJson CAcquisitionEntitySerializer::serialize(const CAcquisition & object)
{
   CJson result;
   result.put(CAcquisitionEntityJsonDefinition::getIdIdentifier(), object.getId());
   result.put(CAcquisitionEntityJsonDefinition::getSourceIdentifier(), object.getSource());
   result.put(CAcquisitionEntityJsonDefinition::getKeywordIdentifier(), object.getKeyword());
   result.put(CAcquisitionEntityJsonDefinition::getValueIdentifier(), object.getValue());
   result.put(CAcquisitionEntityJsonDefinition::getDateIdentifier(), boost::posix_time::to_simple_string(object.getDate()));
   return result;
}
// [END] IEntitySerializer implementation


// IEntityDeserializer implementation
CAcquisition CAcquisitionEntitySerializer::deserialize(const CJson & object)
{
   CAcquisition entity;

   if(object.find(CAcquisitionEntityJsonDefinition::getIdIdentifier()) != object.not_found())
      entity.setId(object.get<int>(CAcquisitionEntityJsonDefinition::getIdIdentifier()));

   if(object.find(CAcquisitionEntityJsonDefinition::getSourceIdentifier()) != object.not_found())
      entity.setSource(object.get<std::string>(CAcquisitionEntityJsonDefinition::getSourceIdentifier()));

   if(object.find(CAcquisitionEntityJsonDefinition::getKeywordIdentifier()) != object.not_found())
      entity.setKeyword(object.get<std::string>(CAcquisitionEntityJsonDefinition::getKeywordIdentifier()));

   if(object.find(CAcquisitionEntityJsonDefinition::getValueIdentifier()) != object.not_found())
      entity.setValue(object.get<std::string>(CAcquisitionEntityJsonDefinition::getValueIdentifier()));

   if(object.find(CAcquisitionEntityJsonDefinition::getDateIdentifier()) != object.not_found())
      entity.setDate(boost::posix_time::time_from_string(object.get<std::string>(CAcquisitionEntityJsonDefinition::getDateIdentifier())));

      return entity;
}

// [END] IEntityDeserializer implementation