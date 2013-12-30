#pragma once

#include "IEntitySerializer.h"
#include "IEntityDeserializer.h"
#include "database/entities/Hardware.h"

class CHardwareEntitySerializer : public IEntitySerializer<CHardware>, public IEntityDeserializer<CHardware>
{
public:
   //------------------------------------------
   ///\brief      Constructor
   //------------------------------------------
   CHardwareEntitySerializer();

   //------------------------------------------
   ///\brief      Desstructor
   //------------------------------------------
   virtual ~CHardwareEntitySerializer();
   
   // IEntitySerializer implementation
   CJson serialize(const CHardware & object);
   // [END] IEntitySerializer implementation

   // IEntityDeserializer implementation
   CHardware deserialize(const CJson & object);
   // [END] IEntityDeserializer implementation
   
   
};
