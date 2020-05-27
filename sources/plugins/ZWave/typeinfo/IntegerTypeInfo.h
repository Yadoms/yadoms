#pragma once
#include <shared/plugin/yPluginApi/typeInfo/IntTypeInfo.h>
#include <shared/DataContainer.h>
#include <value_classes/ValueID.h>

//-----------------------------------------------------
///\brief Interface for keyword type information
//-----------------------------------------------------
class CIntegerTypeInfo : public shared::plugin::yPluginApi::typeInfo::CIntTypeInfo
{
public:
   //-----------------------------------------------------
   ///\brief   Constructor
   //-----------------------------------------------------
   explicit CIntegerTypeInfo(OpenZWave::ValueID& vID);

   //-----------------------------------------------------
   ///\brief   Destructor
   //-----------------------------------------------------
   virtual ~CIntegerTypeInfo();
   // shared::plugin::yPluginApi::typeInfo::ITypeInfo implementation 
   boost::shared_ptr<shared::CDataContainer> serialize() const override;
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
   boost::shared_ptr<shared::CDataContainer> m_data;
};

