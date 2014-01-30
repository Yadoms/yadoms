#pragma once

#include "IEntitySerializer.h"
#include "IEntityDeserializer.h"
#include "database/entities/Acquisition.h"

class CAcquisitionEntitySerializer : public IEntitySerializer<CAcquisition>, public IEntityDeserializer<CAcquisition>
{
public:
   //------------------------------------------
   ///\brief      Constructor
   //------------------------------------------
   CAcquisitionEntitySerializer();

   //------------------------------------------
   ///\brief      Desstructor
   //------------------------------------------
   virtual ~CAcquisitionEntitySerializer();
   
   // IEntitySerializer implementation
   CJson serialize(const CAcquisition & object);
   // [END] IEntitySerializer implementation

   // IEntityDeserializer implementation
   CAcquisition deserialize(const CJson & object);
   // [END] IEntityDeserializer implementation
  
};
