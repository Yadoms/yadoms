#include "stdafx.h"
#include "Humidity.h"
#include <shared/plugin/yPluginApi/historization/Humidity.h>

namespace historizers
{
   CHumidity::CHumidity(const std::string& name,
                        shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CHumidity>(name, accessMode))
   {
   }

   CHumidity::CHumidity(const std::string& name,
                        shared::plugin::yPluginApi::EKeywordAccessMode accessMode,
                        shared::plugin::yPluginApi::historization::EMeasureType measureType)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CHumidity>(name, accessMode, measureType))
   {
   }

   CHumidity::~CHumidity()
   {
   }
} //namespace historizers 


