#include "stdafx.h"
#include "ForecastFormatter.h"
#include "NoPeriodException.hpp"
#include <shared/DataContainer.h>

namespace specificHistorizer
{
   CForecastFormatter::CForecastFormatter(const weatherunderground::helper::EPeriod& period)
      : m_periodString(period.toString())
   {
   }

   void CForecastFormatter::addUnit(const std::string& unitName,
                                    const std::string& unitValue)
   {
      try
      {
         m_units.get(unitName);
      }
      catch (shared::exception::CException&)
      { // If Exception, we create the unit
         m_units.set(unitName, unitValue);
      }
   }

   void CForecastFormatter::addPeriod(const std::string& year,
                                      const std::string& month,
                                      const std::string& day,
                                      const std::string& weatherCondition,
                                      const std::string& tempMax,
                                      const std::string& tempMin,
                                      const std::string& maxWind,
                                      const std::string& aveWind,
                                      const std::string& aveWindDegrees,
                                      const std::string& aveHumidity,
                                      const std::string& rainDay,
                                      const std::string& snowDay)
   {
      shared::CDataContainer temp;

      temp.set("Year", year);
      temp.set("Month", month);
      temp.set("Day", day);
      temp.set("WeatherCondition", weatherCondition);
      temp.set("TempMax", tempMax);
      temp.set("TempMin", tempMin);
      temp.set("MaxWind", maxWind);
      temp.set("AveWind", aveWind);
      temp.set("AveWindDegrees", aveWindDegrees);
      temp.set("AveHumidity", aveHumidity);
      temp.set("RainDay", rainDay);
      temp.set("SnowDay", snowDay);

      m_periods.push_back(temp);
   }

   void CForecastFormatter::clearAllPeriods()
   {
      m_periods.clear();
   }

   void CForecastFormatter::setCityName(const std::string& cityName)
   {
      m_localisation = cityName;
   }

   CForecastFormatter::~CForecastFormatter()
   {
   }

   std::string CForecastFormatter::formatValue() const
   {
      shared::CDataContainer temp;

      temp.set("Units", m_units);

      if (!m_periodString.empty())
         temp.set("PeriodUnit", m_periodString);
      else
         throw CNoPeriodException("No Period configured !");

      temp.set("city", m_localisation);
      temp.set("forecast", m_periods);

      return temp.serialize();
   }
}