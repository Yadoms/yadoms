#include "stdafx.h"
#include "WeatherConfiguration.h"
#include "shared/Log.h"


void CWeatherConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_configuration.initializeWith(data);
}

void CWeatherConfiguration::trace() const
{
   try
   {
      YADOMS_LOG(information) << "Configuration :";
      YADOMS_LOG(information) << "  -  apiKey : " << apiKey();
      YADOMS_LOG(information) << "  -  useSpecificLocation : " << useSpecificLocation();
      if (useSpecificLocation())
      {
         YADOMS_LOG(information) << "  -     longitude : " << specificLocationLongitude();
         YADOMS_LOG(information) << "  -     latitude : " << specificLocationLatitude();
      }
   }
   catch (const shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Parameter not found : " << e.what();
   }
   catch (const shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Parameter value out of range : " << e.what();
   }
}

std::string CWeatherConfiguration::apiKey() const
{
   return m_configuration.get<std::string>("apiKey");
}

bool CWeatherConfiguration::useSpecificLocation() const
{
   return m_configuration.get<bool>("useSpecificLocation");
}

double CWeatherConfiguration::specificLocationLongitude() const
{
   return m_configuration.get<double>("useSpecificLocation.content.longitude");
}

double CWeatherConfiguration::specificLocationLatitude() const
{
   return m_configuration.get<double>("useSpecificLocation.content.latitude");
}
