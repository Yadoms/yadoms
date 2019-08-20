#include "stdafx.h"
#include "RainRate.h"
#include <shared/plugin/yPluginApi/historization/RainRate.h>

namespace historizers
{
   CRainRate::CRainRate(const std::string& name, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CRainRate>(name, shared::plugin::yPluginApi::EKeywordAccessMode::kGet, shared::plugin::yPluginApi::EMeasureType::kCumulative, ti))
   {
   }

   CRainRate::CRainRate(const std::string& name, shared::plugin::yPluginApi::EMeasureType measureType, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CRainRate>(name, shared::plugin::yPluginApi::EKeywordAccessMode::kGet, measureType, ti))
   {
   }

   CRainRate::~CRainRate()
   {
   }

   // COpenZWaveSingleHistorizableData<Poco::Int64> override ------------------------
   void CRainRate::setWithUnits(double value, const std::string& unit)
   {
      if (unit == "in/h")
         set(value / 0.03937008);
      else
         set(value);
   }

   double CRainRate::getWithUnits(const std::string& unit) const
   {
      if (unit == "in/h")
         return get() * 0.03937008;
      return get();
   }

   // [END] - COpenZWaveSingleHistorizableData<double> override ------------------------
} //namespace historizers 


