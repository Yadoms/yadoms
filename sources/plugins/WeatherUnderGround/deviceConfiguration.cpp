#include "stdafx.h"
#include "deviceConfiguration.h"

CdeviceConfiguration::CdeviceConfiguration(const shared::CDataContainer& data)
{
   initializeWith(data);
}

CdeviceConfiguration::~CdeviceConfiguration()
{
}

void CdeviceConfiguration::initializeWith(const shared::CDataContainer& data)
{
   try
   {
      m_data.initializeWith(data);
   }
   catch (boost::thread_interrupted&)
   {
      std::cerr << "ERROR : Device Configuration could not be loaded" << std::endl;
   }
}

bool CdeviceConfiguration::isLiveConditionsEnabled() const
{
   return m_data.get<bool>("LiveConditions");
}

bool CdeviceConfiguration::isAstronomyEnabled() const
{
   return m_data.get<bool>("Astronomy");
}

bool CdeviceConfiguration::isForecast10DaysEnabled() const
{
   return m_data.get<bool>("Forecast10Days");
}

bool CdeviceConfiguration::isConditionsIndividualKeywordsEnabled() const
{
   return m_data.get<bool>("IndividualKeywordsLiveConditions");
}

bool CdeviceConfiguration::isRainIndividualKeywordsEnabled() const
{
   return m_data.get<bool>("IndividualRainKeywordsForecast10days");
}

