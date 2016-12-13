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
   return m_data.get<bool>("deviceToCreate.content.LiveConditions.radio");
}

bool CdeviceConfiguration::isAstronomyEnabled() const
{
   return m_data.get<bool>("deviceToCreate.content.Astronomy.radio");
}

bool CdeviceConfiguration::isForecast10DaysEnabled() const
{
   return m_data.get<bool>("deviceToCreate.content.Forecast10Days.radio");
}

bool CdeviceConfiguration::isConditionsIndividualKeywordsEnabled() const
{
   return m_data.get<bool>("deviceToCreate.content.LiveConditions.content.IndividualKeywords");
}

bool CdeviceConfiguration::isRainIndividualKeywordsEnabled() const
{
   return m_data.get<bool>("deviceToCreate.content.Forecast10Days.content.individualRainForecast");
}

bool CdeviceConfiguration::isTempIndividualKeywordsEnabled() const
{
   return m_data.get<bool>("deviceToCreate.content.Forecast10Days.content.individualTempForecast");
}