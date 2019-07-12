#include "stdafx.h"
#include "BatteryLevel.h"
#include <shared/plugin/yPluginApi/historization/BatteryLevel.h>

namespace historizers
{
   CBatteryLevel::CBatteryLevel(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CIntegerTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<int>(boost::make_shared<shared::plugin::yPluginApi::historization::CBatteryLevel>(name, accessMode, shared::plugin::yPluginApi::EMeasureType::kAbsolute, ti))
   {
   }

   CBatteryLevel::CBatteryLevel(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::EMeasureType measureType, CIntegerTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<int>(boost::make_shared<shared::plugin::yPluginApi::historization::CBatteryLevel>(name, accessMode, measureType, ti))
   {
   }

   CBatteryLevel::~CBatteryLevel()
   {
   }
} //namespace historizers 


