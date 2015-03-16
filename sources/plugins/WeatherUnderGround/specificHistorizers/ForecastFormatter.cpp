#include "stdafx.h"
#include "ForecastFormatter.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>


namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

CForecastFormatter::CForecastFormatter()
{
   m_period = 0;
}

void CForecastFormatter::AddPeriod(
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

	Temp.set ("WeatherCondition", WeatherCondition);
	Temp.set ("TempMax", TempMax);
	Temp.set ("TempMin", TempMin);
	Temp.set ("MaxWind", MaxWind);
	Temp.set ("AveWind", AveWind);
	Temp.set ("AveHumidity", AveHumidity);
   Temp.set ("RainDay", RainDay);
   Temp.set ("Period", m_period);

	Periods.push_back( Temp );

   m_period++;
}

void CForecastFormatter::Finalize()
{
   m_ForecastFrame.set ("PeriodUnit", "Day" );
   m_ForecastFrame.set ("forecast", Periods);
}

CForecastFormatter::~CForecastFormatter()
{
}

std::string CForecastFormatter::formatValue() const
{
   m_ForecastFrame.printToLog();
    return m_ForecastFrame.serialize();
}

} } } } // namespace shared::plugin::yPluginApi::historization

