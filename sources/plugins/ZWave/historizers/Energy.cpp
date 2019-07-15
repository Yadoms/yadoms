#include "stdafx.h"
#include "Energy.h"
#include <shared/plugin/yPluginApi/historization/Energy.h>

namespace historizers
{
   CEnergy::CEnergy(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CEnergyDouble>(name, accessMode, shared::plugin::yPluginApi::EMeasureType::kCumulative, ti))
   {
   }

   CEnergy::CEnergy(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::EMeasureType measureType, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CEnergyDouble>(name, accessMode, measureType, ti))
   {
   }

   CEnergy::~CEnergy()
   {
   }
} //namespace historizers 


