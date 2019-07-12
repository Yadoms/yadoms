#include "stdafx.h"
#include "Counter.h"
#include <shared/plugin/yPluginApi/historization/Counter.h>


namespace historizers
{
   CCounter::CCounter(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CIntegerTypeInfo & ti)
      : COpenZWaveSingleHistorizableData<Poco::Int64>(boost::make_shared<shared::plugin::yPluginApi::historization::CCounter>(name, accessMode, shared::plugin::yPluginApi::EMeasureType::kCumulative, ti))
   {
   }

   CCounter::CCounter(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::EMeasureType measureType, CIntegerTypeInfo & ti)
      : COpenZWaveSingleHistorizableData<Poco::Int64>(boost::make_shared<shared::plugin::yPluginApi::historization::CCounter>(name, accessMode, measureType, ti))
   {
   }

   CCounter::~CCounter()
   {
   }
} //namespace historizers 


