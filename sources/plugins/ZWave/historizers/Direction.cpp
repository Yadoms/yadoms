#include "stdafx.h"
#include "Direction.h"
#include <shared/plugin/yPluginApi/historization/Direction.h>

namespace historizers {

   CDirection::CDirection(const std::string & name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
      : COpenZWaveSingleHistorizableData<int>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<int> >(new shared::plugin::yPluginApi::historization::CDirection(name, accessMode)))
   {

   }

   CDirection::CDirection(const std::string & name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType)
      : COpenZWaveSingleHistorizableData<int>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<int> >(new shared::plugin::yPluginApi::historization::CDirection(name, accessMode, measureType)))
   {

   }

   CDirection::~CDirection()
   {
   }

} //namespace historizers 