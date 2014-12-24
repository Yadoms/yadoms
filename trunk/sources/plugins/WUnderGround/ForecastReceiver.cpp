#include "stdafx.h"
#include "ForecastReceiver.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CForecastReceiver::CForecastReceiver(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string APIKey, std::string Localisation): 
    m_Temp   ( context, PluginName, "temperature", "C" ),
	m_Windchill ( context, PluginName, "Windchill", "C" ),
	m_DewPoint ( context, PluginName, "DewPoint", "C" ),
	m_Pressure ( context, PluginName ),
	m_UV       ( context, PluginName ),
	m_Visibility( context, PluginName),
	m_Rain_1hr  ( context, PluginName, "Rain_1hr"),
	m_Rain_today( context, PluginName, "Rain_today"),
	m_Humidity  ( context, PluginName)
{
	m_APIKey = APIKey;
	m_Localisation = Localisation;
	m_PluginName = PluginName;
	
	m_URL << "http://api.wunderground.com/api/" << m_APIKey << "/conditions/q/" << m_Localisation << ".json";
}

void CForecastReceiver::OnUpdate( std::string PluginName, std::string APIKey, std::string Localisation )
{
	m_APIKey = APIKey;
	m_Localisation = Localisation;
	m_PluginName = PluginName;
	
	m_URL << "http://api.wunderground.com/api/" << m_APIKey << "/conditions/q/" << m_Localisation << ".json";
}

void CForecastReceiver::Request( boost::shared_ptr<yApi::IYPluginApi> context )
{
	try
	{
	   m_data = m_webServer.SendGetRequest( m_URL.str() );

	   //TODO : A virer, pour le moment, cela permet de voir l'ensemble des éléments pour les prochains..
	   m_data.printToLog();
	}
	catch (boost::thread_interrupted&)
	{
		YADOMS_LOG(information) << "No Information from web Site !"  << std::endl;
	}
}

void CForecastReceiver::Parse( boost::shared_ptr<yApi::IYPluginApi> context )
{
	try
	{
	   m_Temp.GetValue           ( m_data.get<double>( "current_observation.temp_c" ) );
	   m_Temp.historizeData      ( context );
	   m_Pressure.GetValue       ( m_data.get<double>( "current_observation.pressure_mb" ) );
	   m_Pressure.historizeData  ( context );
	   m_Windchill.GetValue      ( m_data.get<double>( "current_observation.windchill_c" ) );
	   m_Windchill.historizeData ( context );
	   m_DewPoint.GetValue       ( m_data.get<double>( "current_observation.dewpoint_c" ) );
	   m_DewPoint.historizeData  ( context );
	   m_UV.GetValue             ( m_data.get<double>( "current_observation.UV" ) );
	   m_UV.historizeData        ( context );
	   m_Visibility.GetValue     ( m_data.get<double>( "current_observation.visibility_km" ) * 1000 );
	   m_Visibility.historizeData( context );
	   m_Rain_1hr.GetValue       ( m_data.get<double>( "current_observation.precip_today_metric" ) );
	   m_Rain_1hr.historizeData  ( context );
	   m_Rain_today.GetValue     ( m_data.get<double>( "current_observation.precip_1hr_metric" ) );
	   m_Rain_today.historizeData( context );

	   // ----------------Historize Humidity --------------------------------------------------------------
	   std::string str_humidity = m_data.get<std::string>( "current_observation.relative_humidity" );
	   str_humidity.pop_back();
	   double d_humidity = (double)atof(str_humidity.c_str());

	   m_Humidity.GetValue      ( d_humidity );
	   m_Humidity.historizeData ( context );
	   //--------------------------------------------------------------------------------------------------
	}
	catch (boost::thread_interrupted&)
	{
		YADOMS_LOG(information) << "Error During Parsing Data !"  << std::endl;
	}
}

CForecastReceiver::~CForecastReceiver()
{}