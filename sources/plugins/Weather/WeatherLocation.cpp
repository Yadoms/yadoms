#include "stdafx.h"
#include "WeatherLocation.h"

CWeatherLocation::CWeatherLocation(double latitude,
                                   double longitude)
   : m_latitude(latitude),
     m_longitude(longitude)
{
}

double CWeatherLocation::latitude() const
{
   return m_latitude;
}

double CWeatherLocation::longitude() const
{
   return m_latitude;
}

double CWeatherLocation::altitude() const
{
   return 0.0;
}
