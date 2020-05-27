#include "stdafx.h"
#include "Illumination.h"
#include <shared/plugin/yPluginApi/historization/Illumination.h>

namespace historizers
{
   CIllumination::CIllumination(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CIllumination>(name, accessMode, shared::plugin::yPluginApi::EMeasureType::kAbsolute, ti))
   {
   }

   CIllumination::CIllumination(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::EMeasureType measureType, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CIllumination>(name, accessMode, measureType, ti))
   {
   }

   CIllumination::~CIllumination()
   {
   }
} //namespace historizers 


