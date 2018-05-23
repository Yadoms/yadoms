#pragma once
#include <shared/Export.h>
#include <shared/plugin/yPluginApi/StandardCapacity.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
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
            //                                                   ----------------------------------------------------------------------------------------------
            static const CStandardCapacity& Alarm();            // alarm             | -                     | A on/off alarm value 
            static const CStandardCapacity& ApparentPower();    // apparentpower     | Volt Ampere           | Apparent Voltage
            static const CStandardCapacity& ArmingAlarm();      // armingAlarm       | -                     | Alarme arming status (disarmed, armedAtHome, armedAway...)
            static const CStandardCapacity& BatteryLevel();     // batteryLevel      | percent               | Battery level capacity
            static const CStandardCapacity& CameraMove();       // cameraMove        | -                     | Camera movements
            static const CStandardCapacity& ColorRGB();         // colorrgb          | -                     | A RGB color : red, green, blue
            static const CStandardCapacity& ColorRGBW();        // colorrgbw         | -                     | A RGBW color : red, green, blue, illumination
            static const CStandardCapacity& Counter();          // count             | -                     | Generic counter
            static const CStandardCapacity& Current();          // current           | Ampere                | Current measure
            static const CStandardCapacity& Curtain();          // curtain           | none                  | Curtain command "on", "off", "stop"
            static const CStandardCapacity& DateTime();         // datetime          | none                  | a date time 
            static const CStandardCapacity& Debit();            // debit             | m3/s                  | A debit value, in m3/s
            static const CStandardCapacity& Dimmable();         // dimmable          | none                  | A value from 0-100 (0 is off, 100 is on)
            static const CStandardCapacity& Direction();        // direction         | degrees               | Direction (for compass, wind direction)
            static const CStandardCapacity& Distance();         // length            | meter                 | A distance, in meter	  
            static const CStandardCapacity& Duration();         // duration          | none                  | A Duration (duration type)
            static const CStandardCapacity& ElectricLoad();     // electric load     | Ampere/hour           | Electric load in Ah
            static const CStandardCapacity& Energy();           // energy            | Watt/hour             | Energy measure
            static const CStandardCapacity& Event();            // event             | -                     | Just an event (no associated data)
            static const CStandardCapacity& Frequency();        // frequency         | Hertz                 | Frequency
            static const CStandardCapacity& Humidity();         // humidity          | percent               | Humidity
            static const CStandardCapacity& Illumination();     // illumination      | lux                   | Illumination (lux)
            static const CStandardCapacity& Load();             // load              | percent               | Load (%)
            static const CStandardCapacity& Message();          // message           | -                     | A message (contains "from", "to" and "body" fields)
            static const CStandardCapacity& PluginState();      // pluginState       | -                     | Specific capacity for plugin states
            static const CStandardCapacity& Power();            // power             | Watt                  | Power
            static const CStandardCapacity& PowerFactor();      // powerFactor       | -                     | Power factor (-1 to 1)
            static const CStandardCapacity& Pressure();         // pressure          | hPa                   | Pressure
            static const CStandardCapacity& Rain();             // rain              | mm                    | Rain counter
            static const CStandardCapacity& RainRate();         // rain rate         | mm/s                  | Rain rate
            static const CStandardCapacity& Rssi();             // rssi              | dBm                   | Rssi measure capacity
            static const CStandardCapacity& SignalLevel();      // signal level      | dB                    | An signal level in decibels
            static const CStandardCapacity& SignalPower();      // signal strength   | percent               | An signal strength indication in %
            static const CStandardCapacity& Speed();            // speed             | m/s                   | A speed value, in m/s
            static const CStandardCapacity& Switch();           // switch            | -                     | A on/off value
            static const CStandardCapacity& Tamper();           // tamper            | -                     | A on/off tamper value (alarm systems)
            static const CStandardCapacity& Temperature();      // temperature       | degrees Celcius       | Temperature
            static const CStandardCapacity& Text();             // text              | -                     | A text
            static const CStandardCapacity& UpDownStop();       // upDownStop        | -                     | Used for device accepting orders Up, Down Stop
            static const CStandardCapacity& Uv();               // uv                | uv                    | UV index
            static const CStandardCapacity& Voltage();          // voltage           | Volt                  | Voltage
            static const CStandardCapacity& Volume();           // volume            | m3                    | Volume
            static const CStandardCapacity& WeatherCondition(); // weatherCondition  | -                     | Weather Condition (Cloudy, Snow, PartlySunny, Sunny, Rain, Sleet, Storm, Fog)
            static const CStandardCapacity& Weight();           // weight            | Kg                    | Weight
         };
      }
   }
} // namespace shared::plugin::yPluginApi


