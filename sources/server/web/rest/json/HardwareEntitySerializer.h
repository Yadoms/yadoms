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
   
private:
      //------------------------------------------
   ///\brief      string identifier fr entity member Id 
   //------------------------------------------
   static std::string m_hardwareIdIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member Name 
   //------------------------------------------
   static std::string m_hardwareNameIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member Name 
   //------------------------------------------
   static std::string m_hardwarePluginNameIdentifier;   
   //------------------------------------------
   ///\brief      string identifier fr entity member Configuration 
   //------------------------------------------
   static std::string m_hardwareConfigurationIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member Enabled 
   //------------------------------------------
   static std::string m_hardwareEnabledIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member Deleted 
   //------------------------------------------
   static std::string m_hardwareDeletedIdentifier;
};
