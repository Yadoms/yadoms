#include "stdafx.h"
#include "WUConfiguration.h"
#include <shared/StringExtension.h>
#include <shared/Log.h>
#include "WeatherUndergroundHelpers.h"
#include"NoStateException.hpp"

CWUConfiguration::CWUConfiguration() :
	     m_Localisation   (""),
         m_CountryOrState ("")
{}

CWUConfiguration::~CWUConfiguration()
{}

void CWUConfiguration::initializeWith(const shared::CDataContainer &data)
{   
   try
   {
	   m_data.initializeWith(data);
   }
	catch (boost::thread_interrupted&)
	{
		YADOMS_LOG(error) << "ERROR : Plugin Configuration could not be loaded"  << std::endl;
	}
}

std::string CWUConfiguration::getAPIKey() const
{
   return m_data.get<std::string>("APIKey");
}

std::string CWUConfiguration::getLocalisation()
{
   try
   {
      m_Localisation = m_data.get<std::string>("Localisation");
   }
   catch (shared::exception::CException& e)
   {
	   YADOMS_LOG(warning) << e.what() << std::endl;
   }

   //Delete space between sub-names
   return trimAll( m_Localisation );
}

std::string CWUConfiguration::getCountryOrState()
{
   try
   {
      m_CountryOrState = m_data.get<std::string>("Country");

      if ( m_CountryOrState == "US" )
      {
         m_CountryOrState = getState();
         if ( m_CountryOrState == "NA" ) 
		     throw CNoStateException ("No State configured for United States of America");
      }
   }
   catch (shared::exception::CException& e)
   {
	   YADOMS_LOG(warning) << e.what() << std::endl;
   }
   return m_CountryOrState;
}

std::string CWUConfiguration::getState() const
{
   return m_data.get<std::string>("State");
}

bool CWUConfiguration::IsLiveConditionsEnabled (void ) const
{
	return m_data.get<bool>("LiveConditions");
}

bool CWUConfiguration::IsAstronomyEnabled (void ) const
{
	return m_data.get<bool>("Astronomy");
}

bool CWUConfiguration::IsForecast10DaysEnabled (void ) const
{
	return m_data.get<bool>("Forecast10Days");
}

bool CWUConfiguration::IsConditionsIndividualKeywordsEnabled (void ) const
{
	return m_data.get<bool>("IndividualKeywordsLiveConditions");
}

bool CWUConfiguration::IsRainIndividualKeywordsEnabled (void ) const
{
	return m_data.get<bool>("IndividualRainKeywordsForecast10days");
}