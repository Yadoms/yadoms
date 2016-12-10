#include "stdafx.h"
#include "Voltage.h"
#include <shared/plugin/yPluginApi/historization/Voltage.h>

namespace historizers
{
   CVoltage::CVoltage(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CVoltage>(name, accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti))
   {
   }

   CVoltage::CVoltage(const std::string& name,shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CVoltage>(name, accessMode, measureType, ti))
   {
   }

   CVoltage::~CVoltage()
   {
   }

   // COpenZWaveSingleHistorizableData<Poco::Int64> override ------------------------
   void CVoltage::setWithUnits(double value,
                               const std::string& unit)
   {
      if (unit == "mA")
         set(value / 1000);
      else
         set(value);
   }

   double CVoltage::getWithUnits(const std::string& unit) const
   {
      if (unit == "mA")
         return get() * 1000;
      return get();
   }

   // [END] - COpenZWaveSingleHistorizableData<double> override ------------------------
} //namespace historizers 


