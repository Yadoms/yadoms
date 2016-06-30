#include "stdafx.h"
#include "Uv.h"
#include <shared/plugin/yPluginApi/historization/Uv.h>

namespace historizers
{
   CUv::CUv(const std::string& name,
            shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CUv>(name,
                                                                                                                    accessMode))
   {
   }

   CUv::CUv(const std::string& name,
            shared::plugin::yPluginApi::EKeywordAccessMode accessMode,
            shared::plugin::yPluginApi::historization::EMeasureType measureType)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CUv>(name,
                                                                                                                    accessMode,
                                                                                                                    measureType))
   {
   }

   CUv::~CUv()
   {
   }
} //namespace historizers 


