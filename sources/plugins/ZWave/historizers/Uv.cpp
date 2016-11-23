#include "stdafx.h"
#include "Uv.h"
#include <shared/plugin/yPluginApi/historization/Uv.h>

namespace historizers
{
   CUv::CUv(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CUv>(name, accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti))
   {
   }

   CUv::CUv(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CUv>(name, accessMode, measureType, ti))
   {
   }

   CUv::~CUv()
   {
   }
} //namespace historizers 


