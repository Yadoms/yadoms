#include "stdafx.h"
#include "Counter.h"
#include <shared/plugin/yPluginApi/historization/Counter.h>

namespace historizers {

   CCounter::CCounter(const std::string & name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
      : COpenZWaveSingleHistorizableData<Poco::Int64>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<Poco::Int64> >(new shared::plugin::yPluginApi::historization::CCounter(name, accessMode)))
   {

   }

   CCounter::CCounter(const std::string & name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType)
      : COpenZWaveSingleHistorizableData<Poco::Int64>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<Poco::Int64> >(new shared::plugin::yPluginApi::historization::CCounter(name, accessMode, measureType)))
   {

   }

   CCounter::~CCounter()
   {
   }

} //namespace historizers 
