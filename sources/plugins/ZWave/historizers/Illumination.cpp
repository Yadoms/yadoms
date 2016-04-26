#include "stdafx.h"
#include "Illumination.h"
#include <shared/plugin/yPluginApi/historization/Illumination.h>

namespace historizers {

   CIllumination::CIllumination(const std::string & name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
      : COpenZWaveSingleHistorizableData<double>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<double> >(new shared::plugin::yPluginApi::historization::CIllumination(name, accessMode)))
   {

   }

   CIllumination::CIllumination(const std::string & name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType)
      : COpenZWaveSingleHistorizableData<double>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<double> >(new shared::plugin::yPluginApi::historization::CIllumination(name, accessMode, measureType)))
   {

   }

   CIllumination::~CIllumination()
   {
   }

} //namespace historizers 
