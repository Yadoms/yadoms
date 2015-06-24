#include "stdafx.h"
#include "Astronomy.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CAstronomy::CAstronomy(boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration, std::string PluginName, const std::string Prefix):
           m_Localisation         ( WUConfiguration.getLocalisation() ),
           PercentIlluminatedMoon ( PluginName, Prefix + "PercentIllumitedMoon" ),
           AgeOfMoon              ( PluginName, Prefix + "AgeOfMoon" )
{	
	m_URL.clear();
	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/astronomy/q/" << m_Localisation << ".json";

   if (WUConfiguration.IsAstronomyEnabled())
   {
	   PercentIlluminatedMoon.Initialize ( context );
	   AgeOfMoon.Initialize              ( context );
   }
}

void CAstronomy::OnUpdate( const IWUConfiguration& WUConfiguration )
{
    m_Localisation = WUConfiguration.getLocalisation();
	
	m_URL.clear();

	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/astronomy/q/" << m_Localisation << ".json";
}

void CAstronomy::Request( boost::shared_ptr<yApi::IYPluginApi> context )
{
	try
	{
	   m_data = m_webServer.SendGetRequest( m_URL.str() );
	}
	catch (shared::exception::CException)
	{
		YADOMS_LOG(warning) << "No Information from web Site !"  << std::endl;
	}
	catch (...)
	{
	}
}

void CAstronomy::Parse( boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration )
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
			if (WUConfiguration.IsAstronomyEnabled())
			{
				PercentIlluminatedMoon.SetValue      ( m_data, "moon_phase.percentIlluminated" );
				PercentIlluminatedMoon.historizeData ( context );

				AgeOfMoon.SetValue                   ( m_data, "moon_phase.ageOfMoon" );
				AgeOfMoon.historizeData              ( context );
			}
		}
	}
	catch (...)
	{
	}
}

CAstronomy::~CAstronomy()
{}