#include "stdafx.h"
#include "Weight.h"
#include <shared/plugin/yPluginApi/historization/Weight.h>

namespace historizers
{
   CWeight::CWeight(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CWeight>(name, accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti))
   {
   }

   CWeight::CWeight(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CWeight>(name, accessMode, measureType, ti))
   {
   }

   CWeight::~CWeight()
   {
   }

   // COpenZWaveSingleHistorizableData<Poco::Int64> override ------------------------
   void CWeight::setWithUnits(double value, const std::string& unit)
   {
      if (unit == "lb")
         set(value / 2.204622622);
      else
         set(value);
   }

   double CWeight::getWithUnits(const std::string& unit) const
   {
      if (unit == "lb")
         return get() * 2.204622622;
      return get();
   }

   // [END] - COpenZWaveSingleHistorizableData<double> override ------------------------
} //namespace historizers 


