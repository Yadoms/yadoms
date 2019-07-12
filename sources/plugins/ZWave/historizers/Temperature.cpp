#include "stdafx.h"
#include "Temperature.h"
#include <shared/plugin/yPluginApi/historization/Temperature.h>

namespace historizers
{
   CTemperature::CTemperature(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CTemperature>(name, accessMode, shared::plugin::yPluginApi::EMeasureType::kAbsolute, ti))
   {
   }

   CTemperature::~CTemperature()
   {
   }

   void CTemperature::setWithUnits(double value, const std::string& unit)
   {
      if (isUnitFahrenheit(unit))
         setFahrenheit(value);
      else
      {
         //default behavior
         COpenZWaveSingleHistorizableData<double>::setWithUnits(value, unit);
      }
   }

   double CTemperature::getWithUnits(const std::string& unit) const
   {
      if (isUnitFahrenheit(unit))
         return getFahrenheit();

      //default behavior
      return COpenZWaveSingleHistorizableData<double>::getWithUnits(unit);
   }

   bool CTemperature::isUnitFahrenheit(const std::string& unit) const
   {
      return unit == "F";
   }

   void CTemperature::setFahrenheit(double value)
   {
      set((value - 32.0) * 5.0 / 9.0);
   }

   double CTemperature::getFahrenheit() const
   {
      return (get() * 9.0 / 5.0) + 32.0;
   }
} //namespace historizers 


