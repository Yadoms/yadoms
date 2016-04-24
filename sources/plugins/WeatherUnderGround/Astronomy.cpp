#include "stdafx.h"
#include "Astronomy.h"
#include "ErrorAnswerHandler.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CAstronomy::CAstronomy(boost::shared_ptr<yApi::IYPluginApi> context, IWUConfiguration& WUConfiguration, const std::string & PluginName, const std::string & Prefix):
           m_Localisation         ( WUConfiguration.getLocalisation() ),
           m_CountryOrState       ( WUConfiguration.getCountryOrState() ),
           m_PluginName           ( PluginName ),
           m_MoonCharacteristics  ( PluginName, Prefix + "Moon" )
{
	m_CatchError = false;
	m_URL.str("");
	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/astronomy/q/" << m_CountryOrState << "/" << m_Localisation << ".json";

   try 
   {
	   InitializeVariables( context, WUConfiguration );
   }
   catch (shared::exception::CException& e)
   {
	   YADOMS_LOG(warning) << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;

       context->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError" );
	   m_CatchError = true;
   }
}

void CAstronomy::InitializeVariables ( boost::shared_ptr<yApi::IYPluginApi> context, 
	                                           IWUConfiguration& WUConfiguration
								     )
{
	   if (WUConfiguration.IsAstronomyEnabled())
	   {
		shared::CDataContainer details;
		details.set("provider", "weather-underground");
		details.set("shortProvider", "wu");

         m_MoonCharacteristics.Initialize  ( context, details );

         m_MoonCharacteristics.AddUnit(
                                          shared::plugin::yPluginApi::CStandardCapacities::Load.getName(),
                                          shared::plugin::yPluginApi::CStandardCapacities::Load.getUnit() 
                                      );
	   }
}

void CAstronomy::OnUpdate( boost::shared_ptr<yApi::IYPluginApi> context, IWUConfiguration& WUConfiguration )
{
	try
	{
	  m_Localisation = WUConfiguration.getLocalisation();

	  //read the country or State code
	  m_CountryOrState = WUConfiguration.getCountryOrState();
	
	  m_URL.str("");

	  m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/astronomy/q/" << m_CountryOrState << "/" << m_Localisation << ".json";

      InitializeVariables ( context, WUConfiguration );
   }
   catch (shared::exception::CException& e)
   {
	   YADOMS_LOG(warning) << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;

       context->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError" );
	   m_CatchError = true;
   }
}

bool CAstronomy::Request( boost::shared_ptr<yApi::IYPluginApi> context )
{
	try
	{
		m_CatchError = false;
	   m_data = m_webServer.SendGetRequest( m_URL.str() );
	}
    catch (shared::exception::CException& e)
	{
      YADOMS_LOG(warning) << "Astronomy :" << e.what()  << std::endl;
	  context->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection" );
	  m_CatchError = true;
	}

   return m_CatchError;
}

void CAstronomy::Parse( boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration )
{
	try
	{
		ErrorAnswerHandler Response( context, m_data );
		m_CatchError = Response.ContainError();

		if ( !m_CatchError )
		{
			std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList;

			if (WUConfiguration.IsAstronomyEnabled())
			{
            m_MoonCharacteristics.SetParameters  (m_data,
                                                  "moon_phase.percentIlluminated",
                                                  "moon_phase.ageOfMoon" );

            KeywordList.push_back                (m_MoonCharacteristics.GetHistorizable());
			}

			context->historizeData(m_PluginName, KeywordList);
		}
	}
   catch (shared::exception::CException& e)
	{
      YADOMS_LOG(warning) << e.what() << std::endl;
	}
}

bool CAstronomy::IsModuleInFault ( void )
{
	return m_CatchError;
}

CAstronomy::~CAstronomy()
{}
