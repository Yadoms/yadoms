#pragma once
#include <shared/Export.h>


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
                                                   //        value      |    Unit               |     Description
      static const std::string& Temperature;       // temperature       | degrees Celcius       | Temperature
      static const std::string& BatteryLevel;      // batteryLevel      | percent               | Battery level capacity
      static const std::string& Rssi;              // rssi              | percent               | Rssi measure capacity
      static const std::string& Switch;            // switch            | none                  | A string state (to read a switch, drive a lamp, etc...). Can be a single on/off switch or a dimmable one
      static const std::string& Message;           // message           |                       | A message (contains "from", "to" and "body" fields)
   };

} } } // namespace shared::plugin::yadomsApi

