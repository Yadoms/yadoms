#include "stdafx.h"
#include "WeatherConditions.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CWeatherConditions::CWeatherConditions(boost::shared_ptr<yApi::IYPluginApi> context, IWUConfiguration& WUConfiguration, const std::string & PluginName, const std::string & Prefix):
           m_Localisation        ( WUConfiguration.getLocalisation() ),
           m_CountryOrState      ( WUConfiguration.getCountryOrState() ),
           m_PluginName          ( PluginName),
           m_Temp                ( PluginName, Prefix + "temperature" ),
           m_Pressure            ( PluginName, Prefix + "pressure" ),
           m_Humidity            ( PluginName, Prefix + "Humidity" ),
           m_Visibility          ( PluginName, Prefix + "Visibility" ),
           m_UV                  ( PluginName, Prefix + "UV" ),
           m_DewPoint            ( PluginName, Prefix + "DewPoint"),
           m_Rain_1hr            ( PluginName, Prefix + "Rain_1hr"),
           m_WeatherConditionUrl ( PluginName, Prefix + "WeatherCondition" ),
           m_WindDirection       ( PluginName, Prefix + "WindDirection" ),
           m_WindAverageSpeed    ( PluginName, Prefix + "windAverageSpeed"),
           m_WindMaxSpeed        ( PluginName, Prefix + "windMaxSpeed"),
           m_FeelsLike           ( PluginName, Prefix + "FeelsLike" ),
           m_Windchill           ( PluginName, Prefix + "Windchill" ),
		   m_LiveConditions      ( PluginName, "LiveConditions")

{
   try
   {
	   m_CatchError = false;

	   m_URL.str("");
	   m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/conditions/q/" << m_CountryOrState << "/" << m_Localisation << ".json";

	   InitializeVariables ( context, WUConfiguration );
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(warning) << "Configuration or initialization error of Weather condition module :" << e.what() << std::endl;

	  // Informs Yadoms about the plugin actual state
      context->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError" );

	  m_CatchError = true;
   }
}

void CWeatherConditions::InitializeVariables ( boost::shared_ptr<yApi::IYPluginApi> context, 
	                                           IWUConfiguration& WUConfiguration
								                  )
{
	   if (WUConfiguration.IsConditionsIndividualKeywordsEnabled())
	   {
		  m_Temp.Initialize                ( context );
		  m_Pressure.Initialize            ( context );
		  m_Humidity.Initialize            ( context );
		  m_Visibility.Initialize          ( context );
		  m_UV.Initialize                  ( context );
		  m_DewPoint.Initialize            ( context );
		  m_Rain_1hr.Initialize            ( context );
		  m_WeatherConditionUrl.Initialize ( context );
		  m_WindDirection.Initialize       ( context );
		  m_WindAverageSpeed.Initialize    ( context );
		  m_WindMaxSpeed.Initialize        ( context );
		  m_FeelsLike.Initialize           ( context );
		  m_Windchill.Initialize           ( context );
		}

	if (WUConfiguration.IsLiveConditionsEnabled())
	{
		m_LiveConditions.Initialize ( context );

		m_LiveConditions.AddUnit (
							shared::plugin::yPluginApi::CStandardCapacities::Temperature.getName(),
							shared::plugin::yPluginApi::CStandardCapacities::Temperature.getUnit() 
							);
		m_LiveConditions.AddUnit (
							shared::plugin::yPluginApi::CStandardCapacities::Speed.getName(),
							shared::plugin::yPluginApi::CStandardCapacities::Speed.getUnit() 
							);
		m_LiveConditions.AddUnit (
							shared::plugin::yPluginApi::CStandardCapacities::Humidity.getName(),
							shared::plugin::yPluginApi::CStandardCapacities::Humidity.getUnit() 
							);
		m_LiveConditions.AddUnit (
							shared::plugin::yPluginApi::CStandardCapacities::Rain.getName(),
							shared::plugin::yPluginApi::CStandardCapacities::Rain.getUnit() 
							);
	}
}

void CWeatherConditions::OnUpdate( boost::shared_ptr<yApi::IYPluginApi> context, IWUConfiguration& WUConfiguration )
{
   try
   {
	   m_CatchError = false;

	   InitializeVariables ( context, WUConfiguration );

      //read the localisation
      m_Localisation = WUConfiguration.getLocalisation();

      //read the country or State code
      m_CountryOrState = WUConfiguration.getCountryOrState();

      m_URL.str("");
      m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/conditions/q/" << m_CountryOrState << "/" << m_Localisation << ".json";
   }
   catch (shared::exception::CException& e)
	{
		YADOMS_LOG(warning) << e.what()  << std::endl;
		context->setPluginState(yApi::historization::EPluginState::kCustom, "Initialization Error" );

		m_CatchError = true;
	}
}

bool CWeatherConditions::Request( boost::shared_ptr<yApi::IYPluginApi> context )
{
	try
	{
	   m_CatchError = false;

	   m_data = m_webServer.SendGetRequest( m_URL.str() );

	   m_data.printToLog ();
	}
	catch (shared::exception::CException& e)
	{
		YADOMS_LOG(warning) << "Weather Conditions :" << e.what()  << std::endl;
		context->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection" );
		m_CatchError = true;
	}

	if (!m_CatchError)
	{
		try
		{
			std::string error = m_data.getWithDefault<std::string>("response.error.description","");

			// Si on passe alors c'est qu'il y a une erreur, sinon on sort.
			if (!error.empty())
			{
				m_CatchError = true;

				if (error.compare ("No cities match your search query") == 0)
				{
				   context->setPluginState(yApi::historization::EPluginState::kCustom, "CityNotFound" );
				}
			}
		}
		catch(...)
		{
		}
	}

	if (!m_CatchError)
	{
		try
		{
		  m_CityConditions = m_data.get<std::string>("current_observation.observation_location.city");

		  m_LiveConditions.SetCityName ( m_CityConditions );

		  YADOMS_LOG(information) << "Observation location :" << m_data.get<std::string>("current_observation.observation_location.full");
		}
		catch (shared::exception::CException& e)
		{
			YADOMS_LOG(warning) << "Weather Conditions :" << e.what()  << std::endl;
			m_CatchError = true;
		}
	}
	return m_CatchError;
}

std::string CWeatherConditions::GetCityName ()
{
   return m_CityConditions;
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

			if (WUConfiguration.IsConditionsIndividualKeywordsEnabled())
			{
				m_Temp.SetValue          ( m_data, "current_observation.temp_c" );
				KeywordList.push_back    ( m_Temp.GetHistorizable() );

               m_Pressure.SetValue       ( m_data, "current_observation.pressure_mb" );
			   KeywordList.push_back     ( m_Pressure.GetHistorizable() );

			   m_Humidity.SetValue       ( m_data, "current_observation.relative_humidity" );
			   KeywordList.push_back     ( m_Humidity.GetHistorizable() );

			   m_Visibility.SetValue     ( m_data, "current_observation.visibility_km" );
			   KeywordList.push_back     ( m_Visibility.GetHistorizable() );

			   m_UV.SetValue             ( m_data, "current_observation.UV" );
			   KeywordList.push_back     ( m_UV.GetHistorizable() );

			   m_DewPoint.SetValue       ( m_data, "current_observation.dewpoint_c" );
			   KeywordList.push_back     ( m_DewPoint.GetHistorizable() );

			   m_Rain_1hr.SetValue       ( m_data, "current_observation.precip_today_metric" );
			   KeywordList.push_back     ( m_Rain_1hr.GetHistorizable() );

				m_WeatherConditionUrl.SetValue ( m_data, "current_observation.icon");
				KeywordList.push_back          ( m_WeatherConditionUrl.GetHistorizable() );

			    m_WindDirection.SetValue           ( m_data, "current_observation.wind_degrees");
			    KeywordList.push_back              ( m_WindDirection.GetHistorizable() );

			    m_WindAverageSpeed.SetValue        ( m_data, "current_observation.wind_kph");
			    KeywordList.push_back              ( m_WindAverageSpeed.GetHistorizable() );

			    m_WindMaxSpeed.SetValue            ( m_data, "current_observation.wind_gust_kph");
			    KeywordList.push_back              ( m_WindMaxSpeed.GetHistorizable() );

			    m_FeelsLike.SetValue               ( m_data, "current_observation.feelslike_c" );
			    KeywordList.push_back              ( m_FeelsLike.GetHistorizable() );

			    m_Windchill.SetValue               ( m_data, "current_observation.windchill_c" );
			    KeywordList.push_back              ( m_Windchill.GetHistorizable() );
			}

			if (WUConfiguration.IsLiveConditionsEnabled())
			{
				m_LiveConditions.SetPeriod (m_data,
												"current_observation.local_time_rfc822", 
												"current_observation.icon",
												"current_observation.temp_c",
												"current_observation.pressure_mb",
												"current_observation.visibility_km",
												"current_observation.UV",
												"current_observation.dewpoint_c",
												"current_observation.wind_gust_kph",
												"current_observation.wind_kph",
												"current_observation.wind_degrees",
												"current_observation.relative_humidity",
												"current_observation.precip_today_metric",
												"current_observation.feelslike_c",
												"current_observation.windchill_c"
												);
				KeywordList.push_back          ( m_LiveConditions.GetHistorizable() );
			}

			context->historizeData(m_PluginName, KeywordList);
		}
	}
	catch (shared::exception::CException& e)
	{
      YADOMS_LOG(warning) << e.what() << std::endl;
	}
}

bool CWeatherConditions::IsModuleInFault ( void )
{
	return m_CatchError;
}

CWeatherConditions::~CWeatherConditions()
{}