#include "stdafx.h"
#include "ForecastFormatter.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>


namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

CForecastFormatter::CForecastFormatter( const EPeriod & Period )
{
   m_PeriodString = EPeriodEnum[Period];
}

void CForecastFormatter::AddUnit(
            const std::string& UnitName,
            const std::string& UnitValue
   )
{
   m_Units.set ( UnitName, UnitValue );
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
				const std::string& AveHumidity,
            const std::string& RainDay
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
	Temp.set ("AveHumidity", AveHumidity);
   Temp.set ("RainDay", RainDay);

	m_Periods.push_back( Temp );
}

void CForecastFormatter::ClearAllPeriods( void )
{
   m_Periods.clear();
}

void CForecastFormatter::SetCityName ( const std::string CityName )
{
   m_Localisation = CityName;
}

CForecastFormatter::~CForecastFormatter()
{}

std::string CForecastFormatter::formatValue() const
{
   CDataContainer Temp;

   Temp.set ("Units", m_Units);

   //TODO : Si c'est vide, il faut envoyer une erreur
   if (!m_PeriodString.empty())
      Temp.set ("PeriodUnit", m_PeriodString );
   
   Temp.set ("city", m_Localisation);
   Temp.set ("forecast", m_Periods);

   return Temp.serialize();
}

} } } } // namespace shared::plugin::yPluginApi::historization

