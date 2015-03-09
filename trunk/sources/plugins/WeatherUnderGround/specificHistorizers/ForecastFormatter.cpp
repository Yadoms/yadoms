#include "stdafx.h"
#include "ForecastFormatter.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>


namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

CForecastFormatter::CForecastFormatter()
{}

void CForecastFormatter::AddNewDay(
		        const std::string& WeatherCondition, 
	            const std::string& TempMax, 
				const std::string& TempMin,
				const std::string& MaxWind,
				const std::string& AveWind,
				const std::string& AveHumidity
				)
{
	CDataContainer Temp;

	Temp.set ("WeatherCondition", WeatherCondition);
	Temp.set ("TempMax", TempMax);
	Temp.set ("TempMin", TempMin);
	Temp.set ("MaxWind", MaxWind);
	Temp.set ("AveWind", AveWind);
	Temp.set ("AveHumidity", AveHumidity);

	Periods.push_back( Temp );
}

void CForecastFormatter::Finalize()
{
	m_ForecastFrame.set ("forecast", Periods);
}

CForecastFormatter::~CForecastFormatter()
{
}

std::string CForecastFormatter::formatValue() const
{
    return m_ForecastFrame.serialize();
}

} } } } // namespace shared::plugin::yPluginApi::historization

