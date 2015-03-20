#include "stdafx.h"
#include "Forecast3Days.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CForecast3Days::CForecast3Days(boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration, std::string PluginName, const std::string Prefix):
           m_Localisation              ( WUConfiguration.getLocalisation() ),
		     m_PluginName                ( PluginName ),
           //TODO : Le faire à partir d'un helper pour le Day. Aujourd'hui Day et Hour à insérer dedans
		     m_Forecast                  ( PluginName, "Forecast3Days","Day")
{	
	m_URL.clear();
	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/forecast/q/" << m_Localisation << ".json";

	//Initialization

   try
   {
	   if (WUConfiguration.IsForecast3DaysEnabled())
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
      }
   }
   	catch (...)
	{
		YADOMS_LOG(warning) << "Configuration or initialization error of Forecast 3 Days module"  << std::endl;
	}
}

void CForecast3Days::OnUpdate( const IWUConfiguration& WUConfiguration )
{
    m_Localisation = WUConfiguration.getLocalisation();
	
	m_URL.clear();

	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/forecast/q/" << m_Localisation << ".json";
}

void CForecast3Days::Request( boost::shared_ptr<yApi::IYPluginApi> context )
{
	try
	{
	   m_data = m_webServer.SendGetRequest( m_URL.str() );
      m_data.printToLog();
	}
	catch (shared::exception::CException)
	{
		YADOMS_LOG(warning) << "No Information from web Site !"  << std::endl;
	}
	catch (...)
	{}
}

void CForecast3Days::Parse( boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration )
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

			if (WUConfiguration.IsForecast3DaysEnabled())
			{
				std::vector< shared::CDataContainer > result = m_data.get< std::vector<shared::CDataContainer> >("forecast.simpleforecast.forecastday");
            std::vector< shared::CDataContainer >::iterator i;

            m_Forecast.ClearAllPeriods();

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
                                    "avehumidity",
                                    "qpf_allday.mm"
                                    );
				}

            KeywordList.push_back (m_Forecast.GetHistorizable());
			}

			context->historizeData(m_PluginName, KeywordList);
		}
	}
	catch (shared::exception::CException)
	{
		YADOMS_LOG(warning) << "Error during the parsing of the element !"  << std::endl;
	}
	catch (...)
	{
	}
}

CForecast3Days::~CForecast3Days()
{}