#pragma once
#include <shared/Export.h>
#include <shared/plugin/yadomsApi/StandardCapacity.h>

namespace shared { namespace plugin { namespace yadomsApi {

   //--------------------------------------------------------------
   /// \brief		Standard capacities
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CStandardCapacities
   {
   public:
      //--------------------------------------------------------------
      /// \brief		Standard capacities
      //--------------------------------------------------------------
      static const CStandardCapacity & Temperature;       // temperature       | degrees Celcius       | Temperature
      static const CStandardCapacity & BatteryLevel;      // batteryLevel      | percent               | Battery level capacity
      static const CStandardCapacity & Rssi;              // rssi              | percent               | Rssi measure capacity
      static const CStandardCapacity & Switch;            // switch            | none                  | A string state (to read a switch, drive a lamp, etc...). Can be a single on/off switch or a dimmable one
      static const CStandardCapacity & Message;           // message           |                       | A message (contains "from", "to" and "body" fields)
   };

} } } // namespace shared::plugin::yadomsApi

