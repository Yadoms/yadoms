#pragma once

#include "IEntitySerializer.h"
#include "IEntityDeserializer.h"
#include "database/entities/Widget.h"

class CWidgetEntitySerializer : public IEntitySerializer<CWidget>, public IEntityDeserializer<CWidget>
{
public:
   //------------------------------------------
   ///\brief      Constructor
   //------------------------------------------
   CWidgetEntitySerializer();

   //------------------------------------------
   ///\brief      Desstructor
   //------------------------------------------
   virtual ~CWidgetEntitySerializer();
   
   // IEntitySerializer implementation
   CJson serialize(const CWidget & object);
   // [END] IEntitySerializer implementation

   // IEntityDeserializer implementation
   CWidget deserialize(const CJson & object);
   // [END] IEntityDeserializer implementation
  
};
