#include "stdafx.h"
#include "ForecastReceiver.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CForecastReceiver::CForecastReceiver(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string APIKey, std::string Localisation): 
    m_Temp_C   ( context, PluginName, "temperature_C", "C" ),
    m_Temp_F   ( context, PluginName, "temperature_F", "F" ),
	m_Pressure ( context, PluginName )
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

	//TODO : Rajouter context et Request ici
}

void CForecastReceiver::Request( boost::shared_ptr<yApi::IYPluginApi> context )
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

void CForecastReceiver::Parse( boost::shared_ptr<yApi::IYPluginApi> context )
{
	try
	{
	   m_Temp_C.GetValue        ( m_data.get<double>( "current_observation.temp_c" ) );
	   m_Temp_C.historizeData   ( context );
	   m_Temp_F.GetValue        ( m_data.get<double>( "current_observation.temp_f" ) );
	   m_Temp_F.historizeData   ( context );
	   m_Pressure.GetValue      ( m_data.get<double>( "current_observation.pressure_mb" ) );
	   m_Pressure.historizeData ( context );
	}
	catch (boost::thread_interrupted&)
	{
		YADOMS_LOG(information) << "Error During Parsing Data !"  << std::endl;
	}
}

CForecastReceiver::~CForecastReceiver()
{}