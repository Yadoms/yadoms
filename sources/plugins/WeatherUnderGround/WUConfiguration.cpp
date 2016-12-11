#include "stdafx.h"
#include "WUConfiguration.h"
#include "WeatherUndergroundHelpers.h"
#include"NoStateException.hpp"

CWUConfiguration::CWUConfiguration()
   : m_localisation(""),
     m_countryOrState("")
{
}

CWUConfiguration::~CWUConfiguration()
{
}

void CWUConfiguration::initializeWith(const shared::CDataContainer& data)
{
   try
   {
      m_data.initializeWith(data);
   }
   catch (boost::thread_interrupted&)
   {
      std::cerr << "ERROR : Plugin Configuration could not be loaded" << std::endl;
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
      m_localisation = m_data.get<std::string>("Localisation");
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << std::endl;
   }

   //Delete space between sub-names
   return trimAll(m_localisation);
}

std::string CWUConfiguration::getCountryOrState()
{
   try
   {
      m_countryOrState = m_data.get<std::string>("Country");

      if (m_countryOrState == "US")
      {
         m_countryOrState = getState();
         if (m_countryOrState == "NA")
            throw CNoStateException("No State configured for United States of America");
      }
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << std::endl;
   }
   return m_countryOrState;
}

std::string CWUConfiguration::getState() const
{
   return m_data.get<std::string>("State");
}