#pragma once
#include <shared/plugin/yPluginApi/typeInfo/BoolTypeInfo.h>
#include <shared/DataContainer.h>
#include <value_classes/ValueID.h>

//-----------------------------------------------------
///\brief Interface for keyword type information
//-----------------------------------------------------
class CBoolTypeInfo : public shared::plugin::yPluginApi::typeInfo::CBoolTypeInfo
{
public:
   //-----------------------------------------------------
   ///\brief   Constructor
   //-----------------------------------------------------
   explicit CBoolTypeInfo(OpenZWave::ValueID& vID);

   //-----------------------------------------------------
   ///\brief   Destructor
   //-----------------------------------------------------
   virtual ~CBoolTypeInfo();

   // shared::plugin::yPluginApi::typeInfo::ITypeInfo implementation 
   shared::CDataContainer serialize() const override;
   // END shared::plugin::yPluginApi::typeInfo::ITypeInfo implementation 

private:
   //-----------------------------------------------------
   ///\brief   Generate typeInfo content
   ///\param [in] vID   The openzwave ValueID
   //-----------------------------------------------------
   void initialize(OpenZWave::ValueID& vID);

   //-----------------------------------------------------
   ///\brief   The typeInfo content
   //-----------------------------------------------------
   shared::CDataContainer m_data;
};

