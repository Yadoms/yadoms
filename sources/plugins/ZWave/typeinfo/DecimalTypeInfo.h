#pragma once
#include <shared/plugin/yPluginApi/historization/typeInfo/DoubleTypeInfo.h>
#include <shared/DataContainer.h>
#include <value_classes/ValueID.h>

//-----------------------------------------------------
///\brief Interface for keyword type information
//-----------------------------------------------------
class CDecimalTypeInfo : public shared::plugin::yPluginApi::historization::typeInfo::CDoubleTypeInfo
{
public:
   //-----------------------------------------------------
   ///\brief   Constructor
   //-----------------------------------------------------
   explicit CDecimalTypeInfo(OpenZWave::ValueID& vID);

   //-----------------------------------------------------
   ///\brief   Destructor
   //-----------------------------------------------------
   virtual ~CDecimalTypeInfo();

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

