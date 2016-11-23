#include "stdafx.h"
#include "Humidity.h"
#include <shared/plugin/yPluginApi/historization/Humidity.h>

namespace historizers
{
   CHumidity::CHumidity(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CHumidity>(name, accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti))
   {
   }

   CHumidity::CHumidity(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CHumidity>(name, accessMode, measureType, ti))
   {
   }

   CHumidity::~CHumidity()
   {
   }
} //namespace historizers 


