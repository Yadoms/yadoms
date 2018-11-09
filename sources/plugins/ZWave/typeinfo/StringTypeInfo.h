#pragma once
#include <shared/plugin/yPluginApi/historization/typeInfo/StringTypeInfo.h>
#include <shared/DataContainer.h>
#include <value_classes/ValueID.h>

//-----------------------------------------------------
///\brief Interface for keyword type information
//-----------------------------------------------------
class CStringTypeInfo : public shared::plugin::yPluginApi::historization::typeInfo::CStringTypeInfo
{
public:
   //-----------------------------------------------------
   ///\brief   Constructor
   //-----------------------------------------------------
   explicit CStringTypeInfo(OpenZWave::ValueID& vID);

   //-----------------------------------------------------
   ///\brief   Destructor
   //-----------------------------------------------------
   virtual ~CStringTypeInfo();

   // shared::plugin::yPluginApi::historization::typeInfo::ITypeInfo implementation 
   shared::CDataContainer serialize() const override;
   // END shared::plugin::yPluginApi::historization::typeInfo::ITypeInfo implementation 

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
   shared::CDataContainer m_data;
};

