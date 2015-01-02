#include "stdafx.h"
#include "WeatherConditions.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include "Keywords/Barometric.h"
#include "Keywords/Humidity.h"
#include "Keywords/Rain.h"
#include "Keywords/Temp.h"
#include "Keywords/UV.h"
#include "Keywords/Visibility.h"
#include "Keywords/WeatherIcon.h"
#include "Keywords/Wind.h"

CWeatherConditions::CWeatherConditions(boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration):
           m_Localisation ( WUConfiguration.getLocalisation() )
{	
	m_URL.clear();

	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/conditions/q/" << m_Localisation << ".json";
}

void CWeatherConditions::OnUpdate( const IWUConfiguration& WUConfiguration )
{
    m_Localisation = WUConfiguration.getLocalisation();
	
	m_URL.clear();

	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/conditions/q/" << m_Localisation << ".json";
}

void CWeatherConditions::Request( boost::shared_ptr<yApi::IYPluginApi> context )
{
	try
	{
	   m_data = m_webServer.SendGetRequest( m_URL.str() );

	   YADOMS_LOG(information) << "Observation location :" << m_data.get<std::string>("current_observation.observation_location.full");
	}
	catch (shared::exception::CException)
	{
		YADOMS_LOG(warning) << "No Information from web Site !"  << std::endl;
	}
	catch (...)
	{
	}
}

void CWeatherConditions::Parse( boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration, std::string PluginName )
{
	static const std::string Prefix = "conditions.";

	try
	{
		std::string error = m_data.getWithDefault<std::string>( "response.error.description","" );

		if (!error.empty())
		{
			YADOMS_LOG(error) << "ERROR : " << error  << std::endl;
		}
		else
		{
			if (WUConfiguration.IsStandardInformationEnabled())
			{
				CTemp m_Temp      ( context, PluginName, Prefix + "temperature" );

				m_Temp.SetValue           ( m_data, "current_observation.temp_c" );
				m_Temp.historizeData      ( context );

				CBarometric m_Pressure  ( context, PluginName, Prefix + "pressure" );

                m_Pressure.SetValue       ( m_data, "current_observation.pressure_mb" );
			    m_Pressure.historizeData  ( context );

				CHumidity m_Humidity     ( context, PluginName, Prefix + "Humidity" );

			    m_Humidity.SetValue       ( m_data, "current_observation.relative_humidity" );
			    m_Humidity.historizeData  ( context );

				CVisibility m_Visibility( context, PluginName, Prefix + "Visibility" );

			    m_Visibility.SetValue     ( m_data, "current_observation.visibility_km" );
			    m_Visibility.historizeData( context );

				CUV m_UV        ( context, PluginName, Prefix + "UV" );

			    m_UV.SetValue             ( m_data, "current_observation.UV" );
			    m_UV.historizeData        ( context );

				CTemp m_DewPoint  ( context, PluginName, Prefix + "DewPoint");

			    m_DewPoint.SetValue       ( m_data, "current_observation.dewpoint_c" );
			    m_DewPoint.historizeData  ( context );

				CRain m_Rain_1hr  ( context, PluginName , Prefix + "Rain_1hr");

			    m_Rain_1hr.SetValue       ( m_data, "current_observation.precip_today_metric" );
			    m_Rain_1hr.historizeData  ( context );

				CWeatherIcon m_WeatherConditionUrl ( context, PluginName, Prefix + "WeatherCondition" );

				m_WeatherConditionUrl.SetValue ( m_data, "current_observation.icon");
				m_WeatherConditionUrl.historizeData ( context );
			}

			if (WUConfiguration.IsWindEnabled())
			{
				CWind m_Wind             ( context, PluginName, Prefix );

			    m_Wind.SetValue           ( m_data, "current_observation.wind_degrees", "current_observation.wind_kph", "current_observation.wind_gust_kph");
			    m_Wind.historizeData      ( context );

				CTemp m_FeelsLike ( context, PluginName, Prefix + "FeelsLike" );

			    m_FeelsLike.SetValue      ( m_data, "current_observation.feelslike_c" );
			    m_FeelsLike.historizeData ( context );

				CTemp m_Windchill ( context, PluginName, Prefix + "Windchill" );

			    m_Windchill.SetValue      ( m_data, "current_observation.windchill_c" );
			    m_Windchill.historizeData ( context );
			}
		}
	}
	catch (...)
	{
	}
}

CWeatherConditions::~CWeatherConditions()
{}