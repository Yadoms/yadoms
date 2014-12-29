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

CWeatherConditions::CWeatherConditions(boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration)
{
	m_APIKey = WUConfiguration.getAPIKey();
    m_Localisation = WUConfiguration.getLocalisation();
	
	m_URL.clear();

	m_URL << "http://api.wunderground.com/api/" << m_APIKey << "/conditions/q/" << m_Localisation << ".json";
}

void CWeatherConditions::OnUpdate( const IWUConfiguration& WUConfiguration )
{
	m_APIKey = WUConfiguration.getAPIKey();
    m_Localisation = WUConfiguration.getLocalisation();
	
	m_URL.clear();

	m_URL << "http://api.wunderground.com/api/" << m_APIKey << "/conditions/q/" << m_Localisation << ".json";
}

void CWeatherConditions::Request( boost::shared_ptr<yApi::IYPluginApi> context )
{
	try
	{
	   m_data = m_webServer.SendGetRequest( m_URL.str() );
	}
	catch (boost::thread_interrupted&)
	{
		YADOMS_LOG(information) << "No Information from web Site !"  << std::endl;
	}
}

void CWeatherConditions::Parse( boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration, std::string PluginName )
{
	std::string Prefix = "conditions.";

	try
	{
		std::string error;
		error = m_data.getWithDefault<std::string>( "response.error.description","" );

		if (error.size()> 0)
		{
			YADOMS_LOG(error) << "ERROR : " << error  << std::endl;
		}
		else
		{
			if (WUConfiguration.IsTemperatureEnabled())
			{
				CTemp m_Temp      ( context, PluginName, Prefix + "temperature" );

				m_Temp.SetValue           ( m_data, "current_observation.temp_c" );
				m_Temp.historizeData      ( context );
			}

			if (WUConfiguration.IsPressureEnabled())
			{
				CBarometric m_Pressure  ( context, PluginName, Prefix + "pressure" );

			   m_Pressure.SetValue       ( m_data, "current_observation.pressure_mb" );
			   m_Pressure.historizeData  ( context );
			}

			//TODO : Séparer les conditions ici, et les mettres dans un autre champs que les autres
			if (WUConfiguration.IsWindEnabled())
			{
				CWind m_Wind             ( context, PluginName, Prefix );

			   m_Wind.SetValue           ( m_data, "current_observation.wind_degrees", "current_observation.wind_kph", "current_observation.wind_gust_kph");
			   m_Wind.historizeData      ( context );
			}
			if (WUConfiguration.IsHumidityEnabled())
			{
				CHumidity m_Humidity     ( context, PluginName, Prefix + "Humidity" );

			   m_Humidity.SetValue       ( m_data, "current_observation.relative_humidity" );
			   m_Humidity.historizeData  ( context );
			}

			if (WUConfiguration.IsVisibilityEnabled())
			{
				CVisibility m_Visibility( context, PluginName, Prefix + "Visibility" );

			   m_Visibility.SetValue     ( m_data, "current_observation.visibility_km" );
			   m_Visibility.historizeData( context );
			}

			if (WUConfiguration.IsUVEnabled())
			{
				CUV m_UV        ( context, PluginName, Prefix + "UV" );

			   m_UV.SetValue             ( m_data, "current_observation.UV" );
			   m_UV.historizeData        ( context );
			}

			if (WUConfiguration.IsDewPointEnabled())
			{
				CTemp m_DewPoint  ( context, PluginName, Prefix + "DewPoint");

			   m_DewPoint.SetValue       ( m_data, "current_observation.dewpoint_c" );
			   m_DewPoint.historizeData  ( context );
			}

			if (WUConfiguration.IsRain1hrEnabled())
			{
				CRain m_Rain_1hr  ( context, PluginName , Prefix + "Rain_1hr");

			   m_Rain_1hr.SetValue       ( m_data, "current_observation.precip_today_metric" );
			   m_Rain_1hr.historizeData  ( context );
			}

			if (WUConfiguration.IsRainDayEnabled())
			{
				CRain m_Rain_today( context, PluginName , Prefix + "Rain_today");

			   m_Rain_today.SetValue     ( m_data, "current_observation.precip_1hr_metric" );
			   m_Rain_today.historizeData( context );
			}

			if (WUConfiguration.IsFeelsLikeEnabled())
			{
				CTemp m_FeelsLike ( context, PluginName, Prefix + "FeelsLike" );

			   m_FeelsLike.SetValue      ( m_data, "current_observation.feelslike_c" );
			   m_FeelsLike.historizeData ( context );
			}

			if (WUConfiguration.IsWinChillEnabled())
			{
				CTemp m_Windchill ( context, PluginName, Prefix + "Windchill" );

			   m_Windchill.SetValue      ( m_data, "current_observation.windchill_c" );
			   m_Windchill.historizeData ( context );
			}
			if (WUConfiguration.IsWeatherConditionIconEnabled())
			{
				CWeatherIcon m_WeatherConditionUrl ( context, PluginName, Prefix + "WeatherCondition" );

				m_WeatherConditionUrl.SetValue ( m_data, "current_observation.icon");
				m_WeatherConditionUrl.historizeData ( context );
			}
		}
	}
	catch (boost::thread_interrupted&)
	{
	}
}

CWeatherConditions::~CWeatherConditions()
{}