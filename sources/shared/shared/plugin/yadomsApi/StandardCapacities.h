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
      /// \brief		Standard capacities (please keep in alphabetical order)
      //--------------------------------------------------------------
      static const CStandardCapacity & BatteryLevel;      // batteryLevel      | percent               | Battery level capacity
      static const CStandardCapacity & Counter;           // count             | none                  | Generic counter
      static const CStandardCapacity & Curtain;           // curtain           | none                  | Curtain command "on", "off", "stop"
      static const CStandardCapacity & Humidity;          // humidity          | percent               | Humidity
      static const CStandardCapacity & Message;           // message           |                       | A message (contains "from", "to" and "body" fields)
      static const CStandardCapacity & Rssi;              // rssi              | percent               | Rssi measure capacity
      static const CStandardCapacity & Switch;            // switch            | none                  | A value from 0-100 (0 is off, 100 is on)
      static const CStandardCapacity & Temperature;       // temperature       | degrees Celcius       | Temperature

   };

} } } // namespace shared::plugin::yadomsApi

