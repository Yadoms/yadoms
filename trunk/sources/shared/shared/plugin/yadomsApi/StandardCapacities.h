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
      //                                                    capacity name      |        unit           |          description
      //                                                  ----------------------------------------------------------------------------------------------
      static const CStandardCapacity & BatteryLevel;      // batteryLevel      | percent               | Battery level capacity
      static const CStandardCapacity & Counter;           // count             |  -                    | Generic counter
      static const CStandardCapacity & Current;           // current           | Ampere                | Current measure
      static const CStandardCapacity & Curtain;           // curtain           | none                  | Curtain command "on", "off", "stop"
      static const CStandardCapacity & Direction;         // direction         | degrees               | Direction (for compass, wind direction)
      static const CStandardCapacity & Energy;            // energy            | Watt/hour             | Energy measure
      static const CStandardCapacity & Frequency;         // frequency         | Hertz                 | Frequency
      static const CStandardCapacity & Humidity;          // humidity          | percent               | Humidity
      static const CStandardCapacity & Message;           // message           |  -                    | A message (contains "from", "to" and "body" fields)
      static const CStandardCapacity & Power;             // power             | Watt                  | Power
      static const CStandardCapacity & PowerFactor;       // powerFactor       |  -                    | Power factor (-1 to 1)
      static const CStandardCapacity & Pressure;          // pressure          | hPa                   | Pressure
      static const CStandardCapacity & Rain;              // rain              | mm                    | Rain counter
      static const CStandardCapacity & Restart;           // restart           |  -                    | Command to restart something (system, real devices,...)
      static const CStandardCapacity & Rssi;              // rssi              | percent               | Rssi measure capacity
		static const CStandardCapacity & Shutdown;          // shutdown          |  -                    | Command to shutdown something (system, real devices,...)
		static const CStandardCapacity & Speed;             // speed             | m/s                   | A speed value, in m/s
      static const CStandardCapacity & Switch;            // switch            | none                  | A value from 0-100 (0 is off, 100 is on)
      static const CStandardCapacity & Temperature;       // temperature       | degrees Celcius       | Temperature
      static const CStandardCapacity & Uv;                // uv                | uv                    | UV index
      static const CStandardCapacity & Voltage;           // voltage           | Volt                  | Voltage
      static const CStandardCapacity & Weight;            // weight            | Kg                    | Weight
   };

} } } // namespace shared::plugin::yadomsApi

