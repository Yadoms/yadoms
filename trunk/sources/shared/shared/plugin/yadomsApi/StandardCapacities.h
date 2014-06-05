#pragma once
#include <shared/Export.h>
#include "Capacity.h"


namespace shared { namespace plugin { namespace yadomsApi
{
   //--------------------------------------------------------------
   /// \brief		Standard capacities
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CStandardCapacities
   {
   public:
      //--------------------------------------------------------------
      /// \brief		Standard capacities
      //--------------------------------------------------------------
      static const std::string& Temperature;       // Temperature (degrees Celcius)
      static const std::string& BatteryLevel;      // Battery level capacity (percent)
      static const std::string& Rssi;              // Rssi measure capacity (percent)
   };

} } } // namespace shared::plugin::yadomsApi

