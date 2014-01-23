#pragma once

#include "IEntitySerializer.h"
#include "IEntityDeserializer.h"
#include "database/entities/Page.h"

class CPageEntitySerializer : public IEntitySerializer<CPage>, public IEntityDeserializer<CPage>
{
public:
   //------------------------------------------
   ///\brief      Constructor
   //------------------------------------------
   CPageEntitySerializer();

   //------------------------------------------
   ///\brief      Desstructor
   //------------------------------------------
   virtual ~CPageEntitySerializer();
   
   // IEntitySerializer implementation
   CJson serialize(const CPage & object);
   // [END] IEntitySerializer implementation

   // IEntityDeserializer implementation
   CPage deserialize(const CJson & object);
   // [END] IEntityDeserializer implementation
  
};
