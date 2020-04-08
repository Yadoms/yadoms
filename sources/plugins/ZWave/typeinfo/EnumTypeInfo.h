#pragma once
#include <shared/plugin/yPluginApi/typeInfo/ITypeInfo.h>
#include <shared/DataContainer.h>
#include <value_classes/ValueID.h>

//-----------------------------------------------------
///\brief Interface for keyword type information
//-----------------------------------------------------
class CEnumTypeInfo : public shared::plugin::yPluginApi::typeInfo::ITypeInfo
{
public:
   //-----------------------------------------------------
   ///\brief   Constructor
   //-----------------------------------------------------
   explicit CEnumTypeInfo(OpenZWave::ValueID& vID);

   //-----------------------------------------------------
   ///\brief   Destructor
   //-----------------------------------------------------
   virtual ~CEnumTypeInfo();

   // shared::plugin::yPluginApi::typeInfo::ITypeInfo implementation 
   shared::CDataContainerSharedPtr serialize() const override;
   // END shared::plugin::yPluginApi::typeInfo::ITypeInfo implementation 

private:
   //-----------------------------------------------------
   ///\brief   Generate typeInfo content
   ///\param [in] vID   The openzwave ValueID
   ///\return  The content
   //-----------------------------------------------------
   void initialize(OpenZWave::ValueID& vID);

   //-----------------------------------------------------
   ///\brief   The typeInfo content
   //-----------------------------------------------------
   shared::CDataContainerSharedPtr m_data;
};

