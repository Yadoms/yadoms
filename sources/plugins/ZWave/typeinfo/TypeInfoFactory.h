#pragma once
#include <shared/plugin/yPluginApi/historization/typeInfo/ITypeInfo.h>
#include <shared/DataContainer.h>
#include <value_classes/ValueID.h>

//-----------------------------------------------------
///\brief Factory for generating type information
//-----------------------------------------------------
class CTypeInfoFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Create typeInfo from valueId
   /// \param [in] valueId  The keyword OpenZWave::ValueId source
   /// \return    The associated typeInfo
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::plugin::yPluginApi::historization::typeInfo::ITypeInfo> create(OpenZWave::ValueID& valueId);
};

