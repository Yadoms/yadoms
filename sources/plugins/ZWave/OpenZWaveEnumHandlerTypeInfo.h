#pragma once
#include <shared/plugin/yPluginApi/historization/typeInfo/ITypeInfo.h>
#include <shared/DataContainer.h>
#include <value_classes/ValueID.h>

//-----------------------------------------------------
///\brief Interface for keyword type information
//-----------------------------------------------------
class COpenZWaveEnumHandlerTypeInfo : public shared::plugin::yPluginApi::historization::typeInfo::ITypeInfo
{
public:
   //-----------------------------------------------------
   ///\brief   Constructor
   //-----------------------------------------------------
   COpenZWaveEnumHandlerTypeInfo(OpenZWave::ValueID & vID);

   //-----------------------------------------------------
   ///\brief   Destructor
   //-----------------------------------------------------
   virtual ~COpenZWaveEnumHandlerTypeInfo();

   // shared::plugin::yPluginApi::historization::typeInfo::ITypeInfo implementation 
   virtual shared::CDataContainer serialize() const;
   // END shared::plugin::yPluginApi::historization::typeInfo::ITypeInfo implementation 

private:
   //-----------------------------------------------------
   ///\brief   Generate typeInfo content
   ///\param [in] vID   The openzwave ValueID
   ///\return  The content
   //-----------------------------------------------------
   shared::CDataContainer getTypeInfo(OpenZWave::ValueID & vID);

   //-----------------------------------------------------
   ///\brief   The typeInfo content
   //-----------------------------------------------------
   shared::CDataContainer m_data;

};
