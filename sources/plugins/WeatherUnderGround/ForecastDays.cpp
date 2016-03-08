#include "stdafx.h"
#include "ForecastDays.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CForecastDays::CForecastDays(boost::shared_ptr<yApi::IYPluginApi> context, 
                               IWUConfiguration& WUConfiguration, 
                               std::string PluginName, 
                               const std::string & Prefix
                               ):
           m_Localisation              ( WUConfiguration.getLocalisation() ),
           m_CountryOrState            ( WUConfiguration.getCountryOrState() ),
           m_Prefix                    ( Prefix ),
           m_PluginName                ( PluginName ),
           m_Forecast                  ( PluginName, "Forecast", weatherunderground::helper::EPeriod::kDay)
{
	m_URL.str("");
	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/" << m_Prefix << "/q/" << m_CountryOrState << "/" << m_Localisation << ".json";

	InitializeForecastDays ( context, WUConfiguration );
}

void CForecastDays::InitializeForecastDays ( boost::shared_ptr<yApi::IYPluginApi> context, 
	                                         IWUConfiguration& WUConfiguration
	                                       )
{
	//Initialization
   try
   {
	   if (WUConfiguration.IsForecast10DaysEnabled())
	   {
         m_Forecast.Initialize ( context );

		m_Forecast.AddUnit (
							shared::plugin::yPluginApi::CStandardCapacities::Temperature.getName(),
							shared::plugin::yPluginApi::CStandardCapacities::Temperature.getUnit() 
							);
		m_Forecast.AddUnit (
							shared::plugin::yPluginApi::CStandardCapacities::Speed.getName(),
							shared::plugin::yPluginApi::CStandardCapacities::Speed.getUnit() 
							);
		m_Forecast.AddUnit (
							shared::plugin::yPluginApi::CStandardCapacities::Humidity.getName(),
							shared::plugin::yPluginApi::CStandardCapacities::Humidity.getUnit() 
							);
		m_Forecast.AddUnit (
							shared::plugin::yPluginApi::CStandardCapacities::Rain.getName(),
							shared::plugin::yPluginApi::CStandardCapacities::Rain.getUnit() 
							);

		 if (WUConfiguration.IsRainIndividualKeywordsEnabled() )
		 {
			 for (int counter = 0; counter < 3; ++counter)
			 {
				 std::stringstream TempString; 
				 TempString << m_Prefix << "Rain_Day_" << counter;
				 m_Forecast_Rain[counter].reset (new CRain( m_PluginName, TempString.str() ));
				 m_Forecast_Rain[counter]->Initialize ( context );
			 }
		 }
      }
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(warning) << "Configuration or initialization error of Forecast 3 Days module :" << e.what()  << std::endl;
   }
}

void CForecastDays::OnUpdate(   boost::shared_ptr<yApi::IYPluginApi> context,
	                            IWUConfiguration& WUConfiguration )
{
   //read the localisation
   m_Localisation = WUConfiguration.getLocalisation();
	
   //read the country or State code
   m_CountryOrState = WUConfiguration.getCountryOrState();

	m_URL.str("");

	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/" << m_Prefix << "/q/" << m_CountryOrState << "/" << m_Localisation << ".json";

	InitializeForecastDays ( context, WUConfiguration );
}

void CForecastDays::Request( boost::shared_ptr<yApi::IYPluginApi> context )
{
	try
	{
	   m_data = m_webServer.SendGetRequest( m_URL.str() );
	}
	catch (shared::exception::CException& e)
	{
		YADOMS_LOG(warning) << "Forecast 10 days :"  << e.what() << std::endl;
	}
}

void CForecastDays::Parse( boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration )
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

			if (WUConfiguration.IsForecast10DaysEnabled())
			{
               std::vector< shared::CDataContainer > result = m_data.get< std::vector<shared::CDataContainer> >("forecast.simpleforecast.forecastday");
               std::vector< shared::CDataContainer >::iterator i;

               m_Forecast.ClearAllPeriods();

			   unsigned char counter = 0;

               for(i=result.begin(); i!=result.end(); ++i)
               {
					m_Forecast.AddPeriod(*i,
                                    "date.year",
                                    "date.month",
                                    "date.day",
                                    "icon", 
                                    "high.celsius", 
                                    "low.celsius", 
                                    "maxwind.kph", 
                                    "avewind.kph",
									"avewind.degrees",
                                    "avehumidity",
                                    "qpf_allday.mm",
									"snow_allday.cm"
                                    );

					if (WUConfiguration.IsRainIndividualKeywordsEnabled())
					{
						if ( counter < NB_RAIN_FORECAST_DAY )
						{
				  		   m_Forecast_Rain[counter]->SetValue ( *i,"qpf_allday.mm" );
						   KeywordList.push_back (m_Forecast_Rain[counter]->GetHistorizable());
						}
					    ++counter;
					}
               }
               KeywordList.push_back (m_Forecast.GetHistorizable());
			}
			context->historizeData(m_PluginName, KeywordList);

			YADOMS_LOG(debug) << "Forecast Updated !";
		}
	}
	catch (shared::exception::CException& e)
	{
      YADOMS_LOG(warning) << "Error during the parsing of the element ! : " << e.what() << std::endl;
	}
}

void CForecastDays::SetCityName ( const std::string & CityName )
{
   m_Forecast.SetCityName ( CityName );
}

CForecastDays::~CForecastDays()
{}
