#include "stdafx.h"
#include "ForecastFormatter.h"
#include <shared/exception/InvalidParameter.hpp>
#include "NoPeriodException.hpp"
#include <shared/DataContainer.h>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

CForecastFormatter::CForecastFormatter( const weatherunderground::helper::EPeriod & Period )
   :m_PeriodString(Period.toString())
{
}

void CForecastFormatter::AddUnit(
            const std::string& UnitName,
            const std::string& UnitValue
   )
{
	try
	{
		m_Units.get ( UnitName );
	}
	catch ( shared::exception::CException& )
	{  // If Exception, we create the unit
		m_Units.set ( UnitName, UnitValue );
	}
}

void CForecastFormatter::AddPeriod(
            const std::string& Year,
            const std::string& Month,
            const std::string& Day,
            const std::string& WeatherCondition, 
            const std::string& TempMax, 
            const std::string& TempMin,
            const std::string& MaxWind,
            const std::string& AveWind,
			const std::string& AveWindDegrees,
            const std::string& AveHumidity,
            const std::string& RainDay,
			const std::string& SnowDay
				)
{
   CDataContainer Temp;

   Temp.set ("Year", Year);
   Temp.set ("Month", Month);
   Temp.set ("Day", Day);
   Temp.set ("WeatherCondition", WeatherCondition);
   Temp.set ("TempMax", TempMax);
   Temp.set ("TempMin", TempMin);
   Temp.set ("MaxWind", MaxWind);
   Temp.set ("AveWind", AveWind);
   Temp.set ("AveWindDegrees", AveWindDegrees);
   Temp.set ("AveHumidity", AveHumidity);
   Temp.set ("RainDay", RainDay);
   Temp.set ("SnowDay", SnowDay);

   m_Periods.push_back( Temp );
}

void CForecastFormatter::ClearAllPeriods( void )
{
   m_Periods.clear();
}

void CForecastFormatter::SetCityName ( const std::string & CityName )
{
   m_Localisation = CityName;
}

CForecastFormatter::~CForecastFormatter()
{}

std::string CForecastFormatter::formatValue() const
{
   CDataContainer Temp;

   Temp.set ("Units", m_Units);

   if (!m_PeriodString.empty())
      Temp.set ("PeriodUnit", m_PeriodString );
   else
	   throw CNoPeriodException ("No Period configured !");
   
   Temp.set ("city", m_Localisation);
   Temp.set ("forecast", m_Periods);

   return Temp.serialize();
}

} } } } // namespace shared::plugin::yPluginApi::historization

