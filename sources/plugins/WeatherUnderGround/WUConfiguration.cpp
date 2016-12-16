#include "stdafx.h"
#include "WUConfiguration.h"
#include "WeatherUndergroundHelpers.h"
#include"NoStateException.hpp"

CWUConfiguration::CWUConfiguration()
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