#pragma once

#include "json.h"

template<class TEntity>
class IEntityDeserializer
{
public:
   //------------------------------------------
   ///\brief      Deserialize an entity object from JSON data
   ///\param [in] object  json data
   ///\return     the entity
   //------------------------------------------
   virtual TEntity deserialize(const CJson & object) = 0;

public:
   virtual ~IEntityDeserializer() {}
};