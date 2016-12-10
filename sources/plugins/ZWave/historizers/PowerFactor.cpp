#include "stdafx.h"
#include "PowerFactor.h"
#include <shared/plugin/yPluginApi/historization/PowerFactor.h>

namespace historizers
{
   CPowerFactor::CPowerFactor(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CPowerFactor>(name, accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti))
   {
   }

   CPowerFactor::CPowerFactor(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CPowerFactor>(name, accessMode, measureType, ti))
   {
   }

   CPowerFactor::~CPowerFactor()
   {
   }
} //namespace historizers 


