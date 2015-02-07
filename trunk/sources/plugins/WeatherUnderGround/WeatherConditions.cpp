#include "stdafx.h"
#include "WeatherConditions.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

//JMB//
#include <boost/timer/timer.hpp>

CWeatherConditions::CWeatherConditions(boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration, const std::string & PluginName, const std::string & Prefix):
           m_Localisation        ( WUConfiguration.getLocalisation() ),
		   m_PluginName          ( PluginName),
           m_Temp                ( PluginName, Prefix + "temperature" ),
           m_Pressure            ( PluginName, Prefix + "pressure" ),
           m_Humidity            ( PluginName, Prefix + "Humidity" ),
           m_Visibility          ( PluginName, Prefix + "Visibility" ),
           m_UV                  ( PluginName, Prefix + "UV" ),
           m_DewPoint            ( PluginName, Prefix + "DewPoint"),
           m_Rain_1hr            ( PluginName , Prefix + "Rain_1hr"),
           m_WeatherConditionUrl ( PluginName, Prefix + "WeatherCondition" ),
           m_WindDirection       ( PluginName, Prefix ),
		   m_WindAverageSpeed    ( PluginName, Prefix ),
		   m_WindMaxSpeed        ( PluginName, Prefix ),
           m_FeelsLike           ( PluginName, Prefix + "FeelsLike" ),
           m_Windchill           ( PluginName, Prefix + "Windchill" )

{	
	m_URL.clear();
	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/conditions/q/" << m_Localisation << ".json";

   if (WUConfiguration.IsStandardInformationEnabled())
   {
      m_Temp.Initialize                ( context );
      m_Pressure.Initialize            ( context );
      m_Humidity.Initialize            ( context );
      m_Visibility.Initialize          ( context );
      m_UV.Initialize                  ( context );
      m_DewPoint.Initialize            ( context );
      m_Rain_1hr.Initialize            ( context );
      m_WeatherConditionUrl.Initialize ( context );
   }

   if (WUConfiguration.IsWindEnabled())
   {
	   m_WindDirection.Initialize      ( context );
	   m_WindAverageSpeed.Initialize   ( context );
	   m_WindMaxSpeed.Initialize       ( context );
	   m_FeelsLike.Initialize          ( context );
	   m_Windchill.Initialize          ( context );
   }
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

void CWeatherConditions::Parse( boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration )
{
	try
	{
		std::string error = m_data.getWithDefault<std::string>( "response.error.description","" );

		if (!error.empty())
		{
			YADOMS_LOG(error) << "ERROR : " << error  << std::endl;
		}
		else
		{
			std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList;

			if (WUConfiguration.IsStandardInformationEnabled())
			{
				m_Temp.SetValue           ( m_data, "current_observation.temp_c" );
				KeywordList.push_back (m_Temp.GetHistorizable());

               m_Pressure.SetValue       ( m_data, "current_observation.pressure_mb" );
			   KeywordList.push_back (m_Pressure.GetHistorizable());

			   m_Humidity.SetValue       ( m_data, "current_observation.relative_humidity" );
			   KeywordList.push_back (m_Humidity.GetHistorizable());

			   m_Visibility.SetValue     ( m_data, "current_observation.visibility_km" );
			   KeywordList.push_back (m_Visibility.GetHistorizable());

			   m_UV.SetValue             ( m_data, "current_observation.UV" );
			   KeywordList.push_back (m_UV.GetHistorizable());

			   m_DewPoint.SetValue       ( m_data, "current_observation.dewpoint_c" );
			   KeywordList.push_back (m_DewPoint.GetHistorizable());

			   m_Rain_1hr.SetValue       ( m_data, "current_observation.precip_today_metric" );
			   KeywordList.push_back (m_Rain_1hr.GetHistorizable());

				m_WeatherConditionUrl.SetValue ( m_data, "current_observation.icon");
				KeywordList.push_back (m_WeatherConditionUrl.GetHistorizable());
			}

			if (WUConfiguration.IsWindEnabled())
			{
			    m_WindDirection.SetValue           ( m_data, "current_observation.wind_degrees");
			    KeywordList.push_back (m_WindDirection.GetHistorizable());

			    m_WindAverageSpeed.SetValue           ( m_data, "current_observation.wind_kph");
			    KeywordList.push_back (m_WindAverageSpeed.GetHistorizable());

			    m_WindMaxSpeed.SetValue           ( m_data, "current_observation.wind_gust_kph");
			    KeywordList.push_back (m_WindMaxSpeed.GetHistorizable());

			    m_FeelsLike.SetValue      ( m_data, "current_observation.feelslike_c" );
			    KeywordList.push_back (m_FeelsLike.GetHistorizable());

			    m_Windchill.SetValue      ( m_data, "current_observation.windchill_c" );
			    KeywordList.push_back (m_Windchill.GetHistorizable());
			}
		   
			context->historizeData(m_PluginName, KeywordList);
		}
	}
	catch (...)
	{
	}
}

CWeatherConditions::~CWeatherConditions()
{}