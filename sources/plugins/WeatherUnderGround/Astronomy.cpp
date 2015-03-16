#include "stdafx.h"
#include "Astronomy.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CAstronomy::CAstronomy(boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration, const std::string & PluginName, const std::string & Prefix):
           m_Localisation         ( WUConfiguration.getLocalisation() ),
		   m_PluginName           ( PluginName ),
           PercentIlluminatedMoon ( PluginName, Prefix + "PercentIllumitedMoon" ),
           AgeOfMoon              ( PluginName, Prefix + "AgeOfMoon" )
{	
	m_URL.clear();
	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/astronomy/q/" << m_Localisation << ".json";

   try 
   {
	   if (WUConfiguration.IsAstronomyEnabled())
	   {
		   PercentIlluminatedMoon.Initialize ( context );
		   AgeOfMoon.Initialize              ( context );
	   }
   }
   catch (...) //TODO : To change !!
   {
	   YADOMS_LOG(warning) << "Configuration or initialization error of Astronomy module"  << std::endl;
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
	catch (...) //TODO : To change !!
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
			std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList;

			if (WUConfiguration.IsAstronomyEnabled())
			{
				PercentIlluminatedMoon.SetValue      ( m_data, "moon_phase.percentIlluminated" );
				KeywordList.push_back                (PercentIlluminatedMoon.GetHistorizable());

				AgeOfMoon.SetValue                   ( m_data, "moon_phase.ageOfMoon" );
				KeywordList.push_back                (AgeOfMoon.GetHistorizable());
			}

			context->historizeData(m_PluginName, KeywordList);
		}
	}
	catch (...)
	{
	}
}

CAstronomy::~CAstronomy()
{}