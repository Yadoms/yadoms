#include "stdafx.h"
#include "Current.h"
#include <shared/plugin/yPluginApi/historization/Current.h>

namespace historizers
{
   CCurrent::CCurrent(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CCurrent>(name, accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti))
   {
   }

   CCurrent::CCurrent(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CCurrent>(name, accessMode, measureType, ti))
   {
   }

   CCurrent::~CCurrent()
   {
   }

   // COpenZWaveSingleHistorizableData<Poco::Int64> override ------------------------
   void CCurrent::setWithUnits(double value, const std::string& unit)
   {
      if (unit == "mV")
         set(value / 1000);
      else
         set(value);
   }

   double CCurrent::getWithUnits(const std::string& unit) const
   {
      if (unit == "mV")
         return get() * 1000;
      return get();
   }

   // [END] - COpenZWaveSingleHistorizableData<double> override ------------------------
} //namespace historizers 


