#include "stdafx.h"
#include "WUConfiguration.h"
#include"noStateException.hpp"
#include "Features/Location.h"

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

bool CWUConfiguration::pluginLocationEnabled() const
{
   return m_data.get<bool>("location.checkbox");
}

boost::shared_ptr<const shared::ILocation> CWUConfiguration::getLocation() const
{
   boost::shared_ptr<const shared::ILocation> location;

   return boost::make_shared<location::CLocation>(m_data.get<double>("location.content.longitude"),
                                                  m_data.get<double>("location.content.latitude"),
                                                  0);
}

bool CWUConfiguration::isLiveConditionsEnabled() const
{
   return m_data.get<bool>("LiveConditions.checkbox");
}

bool CWUConfiguration::isAstronomyEnabled() const
{
   return m_data.get<bool>("Astronomy.checkbox");
}

bool CWUConfiguration::isForecast10DaysEnabled() const
{
   return m_data.get<bool>("Forecast10Days.checkbox");
}

bool CWUConfiguration::isConditionsIndividualKeywordsEnabled() const
{
   return m_data.get<bool>("LiveConditions.content.individualKeywords");
}

bool CWUConfiguration::isRainIndividualKeywordsEnabled() const
{
   return m_data.get<bool>("Forecast10Days.content.individualRainForecast");
}

bool CWUConfiguration::isTempIndividualKeywordsEnabled() const
{
   return m_data.get<bool>("Forecast10Days.content.individualTempForecast");
}