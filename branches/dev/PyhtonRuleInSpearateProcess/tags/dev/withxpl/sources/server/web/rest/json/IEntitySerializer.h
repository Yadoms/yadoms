#pragma once

#include "Json.h"

namespace web { namespace rest { namespace json {


   template<class TEntity>
   class IEntitySerializer
   {
   public:
      //------------------------------------------
      ///\brief      Serialize an entity object to JSON data
      ///\param [in] object  the Entity object to Serialize
      ///\return     the object serialized as JSON data
      //------------------------------------------
      virtual CJson serialize(const TEntity & object) = 0;

   public:
      virtual ~IEntitySerializer() {}
   };


} //namespace json
} //namespace rest
} //namespace web 

