#include "stdafx.h"
#include "Pressure.h"
#include <shared/plugin/yPluginApi/historization/Pressure.h>

namespace historizers
{
   CPressure::CPressure(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CPressure>(name, accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti))
   {
   }

   CPressure::CPressure(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CPressure>(name, accessMode, measureType, ti))
   {
   }

   CPressure::~CPressure()
   {
   }

   // COpenZWaveSingleHistorizableData<Poco::Int64> override ------------------------
   void CPressure::setWithUnits(double value, const std::string& unit)
   {
      if (unit == "inHg")
         set(value / 0.029613397);
      else if (unit == "kPa")
         set(value * 10.0);
      else
         set(value);
   }

   double CPressure::getWithUnits(const std::string& unit) const
   {
      if (unit == "inHg")
         return get() * 0.029613397;
      if (unit == "kPa")
         return get() / 10.0;
      return get();
   }

   // [END] - COpenZWaveSingleHistorizableData<double> override ------------------------
} //namespace historizers 


