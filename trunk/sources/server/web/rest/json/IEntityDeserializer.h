#pragma once

#include "Json.h"

namespace web { namespace rest { namespace json {


   template<class TEntity>
   class IEntityDeserializer
   {
   public:
      //------------------------------------------
      ///\brief      Deserialize an entity object from JSON data
      ///\param [in] object  json data
      ///\return     the entity
      //------------------------------------------
      virtual boost::shared_ptr<TEntity> deserialize(const CJson & object) = 0;

   public:
      virtual ~IEntityDeserializer() {}
   };


} //namespace json
} //namespace rest
} //namespace web 

