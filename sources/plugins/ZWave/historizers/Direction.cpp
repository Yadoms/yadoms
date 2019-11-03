#include "stdafx.h"
#include "Direction.h"
#include <shared/plugin/yPluginApi/historization/Direction.h>

namespace historizers
{
   CDirection::CDirection(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CIntegerTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<int>(boost::make_shared<shared::plugin::yPluginApi::historization::CDirection>(name, accessMode, shared::plugin::yPluginApi::EMeasureType::kAbsolute, ti))
   {
   }

   CDirection::CDirection(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::EMeasureType measureType, CIntegerTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<int>(boost::make_shared<shared::plugin::yPluginApi::historization::CDirection>(name, accessMode, measureType, ti))
   {
   }

   CDirection::~CDirection()
   {
   }
} //namespace historizers 


