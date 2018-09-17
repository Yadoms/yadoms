#include "stdafx.h"
#include "UserCode.h"
#include <shared/plugin/yPluginApi/historization/UserCode.h>
namespace historizers
{
   CUserCode::CUserCode(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CStringTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<std::string>(boost::make_shared<shared::plugin::yPluginApi::historization::CUserCode >(name, accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti))
   {
   }

   CUserCode::~CUserCode()
   {
   }


} //namespace historizers 


