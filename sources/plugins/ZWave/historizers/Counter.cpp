#include "stdafx.h"
#include "Counter.h"
#include <shared/plugin/yPluginApi/historization/Counter.h>

namespace historizers
{
   CCounter::CCounter(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
      : COpenZWaveSingleHistorizableData<Poco::Int64>(boost::make_shared<shared::plugin::yPluginApi::historization::CCounter>(name,
                                                                                                                              accessMode))
   {
   }

   CCounter::CCounter(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode,
                      shared::plugin::yPluginApi::historization::EMeasureType measureType)
      : COpenZWaveSingleHistorizableData<Poco::Int64>(boost::make_shared<shared::plugin::yPluginApi::historization::CCounter>(name,
                                                                                                                              accessMode,
                                                                                                                              measureType))
   {
   }

   CCounter::~CCounter()
   {
   }
} //namespace historizers 


