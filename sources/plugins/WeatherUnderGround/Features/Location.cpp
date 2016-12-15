#include "stdafx.h"
#include "Location.h"
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>


namespace location
{
   CLocation::CLocation(double longitude,
                        double latitude,
                        double altitude)
      : m_longitude(longitude),
        m_latitude(latitude),
         m_altitude(altitude)
   {}

   CLocation::~CLocation()
   {
   }

   double CLocation::latitude() const
   {
      return m_latitude;
   }

   double CLocation::longitude() const
   {
      return m_longitude;
   }

   double CLocation::altitude() const
   {
      return m_altitude;
   }
} // namespace location