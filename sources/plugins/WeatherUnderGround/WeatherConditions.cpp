#include "stdafx.h"
#include "WeatherConditions.h"
#include "ErrorAnswerHandler.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CWeatherConditions::CWeatherConditions(boost::shared_ptr<yApi::IYPluginApi> context, IWUConfiguration& WUConfiguration, const std::string & PluginName, const std::string & Prefix):
           m_Localisation        ( WUConfiguration.getLocalisation() ),
           m_CountryOrState      ( WUConfiguration.getCountryOrState() ),
           m_PluginName          ( PluginName),
           m_Temp                ( PluginName, Prefix + "temperature" ),
		   m_pressure            ( new yApi::historization::CPressure( Prefix + "pressure" )),
		   m_humidity            ( new yApi::historization::CHumidity ( Prefix + "Humidity" )),
		   m_visibility          ( new yApi::historization::CDistance ( Prefix + "Visibility" )),
		   m_uv                  ( new yApi::historization::CDirection( Prefix + "UV" )),
           m_DewPoint            ( PluginName, Prefix + "DewPoint"),
		   m_Rain_1hr            ( PluginName, Prefix + "Rain_1hr"),
           m_WeatherConditionUrl ( PluginName, Prefix + "WeatherCondition" ),
		   m_WindDirection       ( new yApi::historization::CDirection( Prefix + "WindDirection" )),
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
		shared::CDataContainer details;
		details.set("provider", "weather-underground");
		details.set("shortProvider", "wu");

	   if (WUConfiguration.IsConditionsIndividualKeywordsEnabled())
	   {
		  if (!context->keywordExists( m_PluginName, m_pressure->getKeyword()))      context->declareKeyword(m_PluginName, *m_pressure, details);
		  if (!context->keywordExists( m_PluginName, m_humidity->getKeyword()))      context->declareKeyword(m_PluginName, *m_humidity, details);
		  if (!context->keywordExists( m_PluginName, m_visibility->getKeyword()))    context->declareKeyword(m_PluginName, *m_visibility, details);
		  if (!context->keywordExists( m_PluginName, m_uv->getKeyword()))            context->declareKeyword(m_PluginName, *m_uv, details);
		  if (!context->keywordExists( m_PluginName, m_WindDirection->getKeyword())) context->declareKeyword(m_PluginName, *m_WindDirection, details);

		  m_Temp.Initialize                ( context, details );
		  m_DewPoint.Initialize            ( context, details );
		  m_Rain_1hr.Initialize            ( context, details );
		  m_WeatherConditionUrl.Initialize ( context, details );
		  m_WindAverageSpeed.Initialize    ( context, details );
		  m_WindMaxSpeed.Initialize        ( context, details );
		  m_FeelsLike.Initialize           ( context, details );
		  m_Windchill.Initialize           ( context, details );
		}

	if (WUConfiguration.IsLiveConditionsEnabled())
	{
		m_LiveConditions.Initialize ( context, details );

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
	if (!m_CatchError)
	{
		try
		{
		   m_data = m_webServer.SendGetRequest( m_URL.str() );
		}
		catch (shared::exception::CException& e)
		{
			YADOMS_LOG(warning) << "Weather Conditions :" << e.what()  << std::endl;
			context->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection" );
			m_CatchError = true;
		}
	}

	if (!m_CatchError)
	{
		try
		{
			m_data.printToLog ();

		    ErrorAnswerHandler Response( context, m_data );

			m_CatchError = Response.ContainError();
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
				//
				//Temperature
				//
				m_Temp.SetValue          ( m_data, "current_observation.temp_c" );
				KeywordList.push_back    ( m_Temp.GetHistorizable() );

				//
				//Pressure
				//
			    m_pressure->set(m_data.get<double>( "current_observation.pressure_mb" ));
			    YADOMS_LOG(debug) << m_pressure->getKeyword() << "=" << m_pressure->get() << "mbar";
			    KeywordList.push_back     ( m_pressure );

				//
				//Humidity
				//

			    std::string str_humidity = m_data.get<std::string>( "current_observation.relative_humidity" );
			    str_humidity.erase( str_humidity.end()-1 );
			    double d_humidity = (double)atof(str_humidity.c_str());
			    m_humidity->set( d_humidity );
			    YADOMS_LOG(debug) << m_humidity->getKeyword() << "=" << m_humidity->get() << "%";
			    KeywordList.push_back     ( m_humidity );

				//
				//Visibility
				//

				if (m_data.get<std::string>( "current_observation.visibility_km" ) == "N/A")
					YADOMS_LOG(information) << m_visibility->getKeyword() << " : N/A => Value not registered";
				else
				{
					// x 1000 -> The visibility from the web site is in kilometer
					m_visibility->set(m_data.get<double>( "current_observation.visibility_km" ) * 1000 );
					YADOMS_LOG(debug) << m_visibility->getKeyword() << "=" << m_visibility->get() << "m";
				}
			    KeywordList.push_back     ( m_visibility );

				//
				//UV
				//

	            m_uv->set((int) m_data.get<double>( "current_observation.UV" ));
	            YADOMS_LOG(debug) << m_uv->getKeyword() << "=" << m_uv->get();
			    KeywordList.push_back     ( m_uv );

				//
				//DewPoint
				//

			    m_DewPoint.SetValue       ( m_data, "current_observation.dewpoint_c" );
			    KeywordList.push_back     ( m_DewPoint.GetHistorizable() );

				//
				//Rain
				//

                m_Rain_1hr.SetValue                ( m_data, "current_observation.precip_today_metric" );
			    KeywordList.push_back              ( m_Rain_1hr.GetHistorizable() );

				//
				//Visual condition
				//

				m_WeatherConditionUrl.SetValue     ( m_data, "current_observation.icon");
				KeywordList.push_back              ( m_WeatherConditionUrl.GetHistorizable() );

				//
				//Wind (degrees)
				//

				m_WindDirection->set((int) m_data.get<double>( "current_observation.wind_degrees" ));
				YADOMS_LOG(debug) << m_WindDirection->getKeyword() << "=" << m_WindDirection->get() << " degrees";
			    KeywordList.push_back              ( m_WindDirection );

				//
				//Wind (speed)
				//

			    m_WindAverageSpeed.SetValue        ( m_data, "current_observation.wind_kph");
			    KeywordList.push_back              ( m_WindAverageSpeed.GetHistorizable() );

				//
				//Wind (Max speed)
				//

			    m_WindMaxSpeed.SetValue            ( m_data, "current_observation.wind_gust_kph");
			    KeywordList.push_back              ( m_WindMaxSpeed.GetHistorizable() );

				//
				//Feelslike
				//

			    m_FeelsLike.SetValue               ( m_data, "current_observation.feelslike_c" );
			    KeywordList.push_back              ( m_FeelsLike.GetHistorizable() );

				//
				//Windchill
				//

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