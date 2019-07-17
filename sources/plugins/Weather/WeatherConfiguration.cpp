#include "stdafx.h"
#include "WeatherConfiguration.h"
#include "shared/Log.h"
#include "WeatherLocation.h"


void CWeatherConfiguration::initializeWith(const shared::CDataContainer& data)
{
   data.printToLog(YADOMS_LOG(debug));
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
         YADOMS_LOG(information) << "  -     longitude : " << specificLocation()->longitude();
         YADOMS_LOG(information) << "  -     latitude : " << specificLocation()->latitude();
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
   return m_configuration.get<bool>("useSpecificLocation.checkbox");
}

boost::shared_ptr<const shared::ILocation> CWeatherConfiguration::specificLocation() const
{
   return boost::make_shared<CWeatherLocation>(
      m_configuration.get<double>("useSpecificLocation.content.latitude"),
      m_configuration.get<double>("useSpecificLocation.content.longitude"));
}

