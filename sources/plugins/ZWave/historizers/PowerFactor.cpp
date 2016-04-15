#include "stdafx.h"
#include "PowerFactor.h"
#include <shared/plugin/yPluginApi/historization/PowerFactor.h>

namespace historizers {

   CPowerFactor::CPowerFactor(const std::string & name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
      : COpenZWaveSingleHistorizableData<double>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<double> >(new shared::plugin::yPluginApi::historization::CPowerFactor(name, accessMode)))
   {

   }

   CPowerFactor::CPowerFactor(const std::string & name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType)
      : COpenZWaveSingleHistorizableData<double>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<double> >(new shared::plugin::yPluginApi::historization::CPowerFactor(name, accessMode, measureType)))
   {

   }

   CPowerFactor::~CPowerFactor()
   {
   }

} //namespace historizers 