#include "stdafx.h"
#include "Temperature.h"
#include "../StandardCapacities.h"
#include <Poco/RegularExpression.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

   CTemperature::CTemperature(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType, typeInfo::CDoubleTypeInfo & additionalInfo)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Temperature, accessMode, 0.0, measureType, additionalInfo)
   {
   }

   CTemperature::~CTemperature()
   {
   }

   void CTemperature::setWithUnits(double value, const std::string & unit)
   {
      if (isUnitFahrenheit(unit))
         setFahrenheit(value);
      else
      {
         //default behavior
         CSingleHistorizableData<double>::setWithUnits(value, unit);
      }
   }

   double CTemperature::getWithUnits(const std::string & unit) const
   {
      if(isUnitFahrenheit(unit))
         return getFahrenheit();

      //default behavior
      return CSingleHistorizableData<double>::getWithUnits(unit);
   }

   bool CTemperature::isUnitFahrenheit(const std::string & unit) const
   {
      std::string lowUnit = boost::to_lower_copy(unit);
      return Poco::RegularExpression("°?f").match(lowUnit);
   }

   void CTemperature::setFahrenheit(double value)
   {
      set((value - 32.0)*5.0 / 9.0);
   }

   double CTemperature::getFahrenheit() const
   {
      return (get()*9.0 / 5.0) + 32.0;
   }

} } } } // namespace shared::plugin::yPluginApi::historization

