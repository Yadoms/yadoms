#pragma once

#include "IEntitySerializer.h"
#include "IEntityDeserializer.h"
#include "database/entities/Device.h"

class CDeviceEntitySerializer : public IEntitySerializer<CDevice>, public IEntityDeserializer<CDevice>
{
public:
   //------------------------------------------
   ///\brief      Constructor
   //------------------------------------------
   CDeviceEntitySerializer();

   //------------------------------------------
   ///\brief      Desstructor
   //------------------------------------------
   virtual ~CDeviceEntitySerializer();
   
   // IEntitySerializer implementation
   CJson serialize(const CDevice & object);
   // [END] IEntitySerializer implementation

   // IEntityDeserializer implementation
   CDevice deserialize(const CJson & object);
   // [END] IEntityDeserializer implementation
  
};
