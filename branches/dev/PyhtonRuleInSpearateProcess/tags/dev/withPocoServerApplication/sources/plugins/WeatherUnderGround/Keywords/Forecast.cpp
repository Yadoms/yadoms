#include "stdafx.h"
#include "Forecast.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CForecast::CForecast( std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_forecast( new yApi::historization::CForecastHistorizer(KeyWordName, yApi::EKeywordAccessMode::kGetSet) )
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

void CForecast::AddPeriod(const shared::CDataContainer & ValueContainer, 
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
		               ValueContainer.get<std::string>( filterWeatherCondition ),
						   ValueContainer.get<std::string>( filterTempMax ),
						   ValueContainer.get<std::string>( filterTempMin ),
						   ValueContainer.get<std::string>( filterMaxWind ),
						   ValueContainer.get<std::string>( filterAveWind ),
						   ValueContainer.get<std::string>( filterAveHumidity ),
                     ValueContainer.get<std::string>( RainDay )
		                 );

	YADOMS_LOG(debug) << "Forecast Update !";
}

boost::shared_ptr<yApi::historization::IHistorizable> CForecast::GetHistorizable() const
{
	return m_forecast;
}
