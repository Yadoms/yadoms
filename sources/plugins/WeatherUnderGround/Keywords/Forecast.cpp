#include "stdafx.h"
#include "Forecast.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <boost/lexical_cast.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CForecast::CForecast( std::string PluginName, std::string KeyWordName, const EPeriod& Period )
   :m_PluginName ( PluginName ), m_forecast( new yApi::historization::CForecastHistorizer(KeyWordName, yApi::EKeywordAccessMode::kGetSet, Period ) )
{}

void CForecast::Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const
{
   if (!context->keywordExists( m_PluginName, m_forecast->getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CForecast::~CForecast()
{}

void CForecast::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, *m_forecast);
}

void CForecast::AddUnit(
            const std::string& UnitName,
            const std::string& UnitValue
   )
{
   m_forecast->AddUnit ( UnitName, UnitValue );
}

void CForecast::AddPeriod(const shared::CDataContainer & ValueContainer, 
                          const std::string& filterYear,
                          const std::string& filterMonth,
                          const std::string& filterDay,
	                       const std::string& filterWeatherCondition,
	                       const std::string& filterTempMax, 
					           const std::string& filterTempMin,
					           const std::string& filterMaxWind,
					           const std::string& filterAveWind,
					           const std::string& filterAveHumidity,
                          const std::string& RainDay
					           )
{
	m_forecast->AddPeriod(
                     ValueContainer.get<std::string>( filterYear ),
                     ValueContainer.get<std::string>( filterMonth ),
                     ValueContainer.get<std::string>( filterDay ),
		               ValueContainer.get<std::string>( filterWeatherCondition ),
						   ValueContainer.get<std::string>( filterTempMax ),
						   ValueContainer.get<std::string>( filterTempMin ),
						   boost::lexical_cast<std::string>(ValueContainer.get<double>( filterMaxWind ) / 3.6), // Transform from Km/h -> m/s
						   boost::lexical_cast<std::string>(ValueContainer.get<double>( filterAveWind ) / 3.6), // Transform from Km/h -> m/s
						   ValueContainer.get<std::string>( filterAveHumidity ),
                     ValueContainer.get<std::string>( RainDay )
		                 );

	YADOMS_LOG(debug) << "Forecast Update !";
}

void CForecast::ClearAllPeriods( void )
{
   m_forecast->ClearAllPeriods();
}

void CForecast::SetCityName ( const std::string CityName )
{
   m_forecast->SetCityName ( CityName );
}

boost::shared_ptr<yApi::historization::IHistorizable> CForecast::GetHistorizable() const
{
	return m_forecast;
}