#include "stdafx.h"
#include "WUConfiguration.h"
#include <shared/StringExtension.h>
#include <shared/Log.h>

CWUConfiguration::CWUConfiguration()
{
   m_isWindEnabled = false;
   m_isStandardInformationEnabled = false;
}

CWUConfiguration::~CWUConfiguration()
{}

void CWUConfiguration::initializeWith(const shared::CDataContainer &data)
{
   m_data.initializeWith(data);

   try
   {
	   m_isWindEnabled        = m_data.get<bool>("Information.content.Wind");
	   m_isStandardInformationEnabled = m_data.get<bool>("Information.content.Standard");
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

std::string CWUConfiguration::getLocalisation() const
{
   return m_data.get<std::string>("Localisation");
}

std::string CWUConfiguration::getCountryOrState() const
{
   std::string country = m_data.get<std::string>("Country");

   if ( country == "US" )
   {
      country = getState();
      //TODO : Faire une vrai Exception
      if ( country == "NA" ) throw;
   }

   return country;
}

std::string CWUConfiguration::getState() const
{
   return m_data.get<std::string>("State");
}

bool CWUConfiguration::IsWindEnabled (void) const                 {return m_isWindEnabled;}
bool CWUConfiguration::IsStandardInformationEnabled (void ) const {return m_isStandardInformationEnabled;}

bool CWUConfiguration::IsAstronomyEnabled (void ) const
{
	return m_data.get<bool>("Astronomy.content.Astronomy");
}

bool CWUConfiguration::IsForecast10DaysEnabled (void ) const
{
	return m_data.get<bool>("Forecast10Days.content.Forecast10Days");
}