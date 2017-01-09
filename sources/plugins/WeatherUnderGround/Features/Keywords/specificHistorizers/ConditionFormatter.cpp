#include "stdafx.h"
#include "ConditionFormatter.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>

namespace specificHistorizer
{
   CConditionFormatter::CConditionFormatter()
   {
   }

   void CConditionFormatter::addUnit(const std::string& unitName,
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

   void CConditionFormatter::setPeriod(const std::string& time,
                                       const std::string& weatherCondition,
                                       const std::string& temp,
                                       const std::string& pressure,
                                       const std::string& visibility,
                                       const std::string& uv,
                                       const std::string& dewPoint,
                                       const std::string& maxWind,
                                       const std::string& aveWind,
                                       const std::string& aveWindDegrees,
                                       const std::string& humidity,
                                       const std::string& rainDay,
                                       const std::string& feelslike,
                                       const std::string& windChill)
   {
      m_conditionFrame.set("Time", time);
      m_conditionFrame.set("WeatherCondition", weatherCondition);
      m_conditionFrame.set("Temp", temp);
      m_conditionFrame.set("Pressure", pressure);
      m_conditionFrame.set("Visibility", visibility);
      m_conditionFrame.set("UV", uv);
      m_conditionFrame.set("DewPoint", dewPoint);
      m_conditionFrame.set("MaxWind", maxWind);
      m_conditionFrame.set("AveWind", aveWind);
      m_conditionFrame.set("AveWindDegrees", aveWindDegrees);
      m_conditionFrame.set("Humidity", humidity);
      m_conditionFrame.set("RainDay", rainDay);
      m_conditionFrame.set("FeelsLike", feelslike);
      m_conditionFrame.set("WindChill", windChill);
   }

   void CConditionFormatter::setCityName(const std::string& cityName)
   {
      m_localisation = cityName;
   }

   CConditionFormatter::~CConditionFormatter()
   {
   }

   std::string CConditionFormatter::formatValue() const
   {
      shared::CDataContainer temp;

      temp.set("Units", m_units);
      temp.set("city", m_localisation);
      temp.set("Conditions", m_conditionFrame);

      return temp.serialize();
   }
}