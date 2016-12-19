#include "stdafx.h"
#include "StandardCapacities.h"
#include "StandardUnits.h"
#include "StandardValues.h"

#define DECLARE_STANDARD_CAPACITY(cppName, name, unit, keywordDataType) \
   const shared::plugin::yPluginApi::CStandardCapacity& CStandardCapacities::cppName() \
   { \
      static const shared::plugin::yPluginApi::CStandardCapacity capacity( \
         name, \
         unit, \
         keywordDataType); \
      return capacity; \
   }

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //----------------------------------------------------
         /// \brief This file merge the static declarations of variables
         ///        this trick is used to fix the "static initialization order fiasco"
         //----------------------------------------------------

         const std::string CStandardUnits::NoUnits("data.units.noUnit");
         const std::string CStandardUnits::Ampere("data.units.ampere");
         const std::string CStandardUnits::Degrees("data.units.degrees");
         const std::string CStandardUnits::DegreesCelcius("data.units.degreesCelcius");
         const std::string CStandardUnits::DegreesFarenheit("data.units.degreesFarenheit");
         const std::string CStandardUnits::hectoPascal("data.units.hectoPascal");
         const std::string CStandardUnits::Hertz("data.units.hertz");
         const std::string CStandardUnits::Kg("data.units.kg");
         const std::string CStandardUnits::Lux("data.units.lux");
         const std::string CStandardUnits::Meter("data.units.meter");
         const std::string CStandardUnits::MetersPerSecond("data.units.metersPerSecond");
         const std::string CStandardUnits::Millimeter("data.units.millimeter");
         const std::string CStandardUnits::MillimeterPerSecond("data.units.millimeterPerSecond");
         const std::string CStandardUnits::Percent("data.units.percent");
         const std::string CStandardUnits::Second("data.units.second");
         const std::string CStandardUnits::Uv("data.units.uv");
         const std::string CStandardUnits::Volt("data.units.volt");
         const std::string CStandardUnits::VoltAmpere("data.units.voltAmpere");
         const std::string CStandardUnits::Watt("data.units.watt");
         const std::string CStandardUnits::WattPerHour("data.units.wattPerHour");

         const std::string CStandardValues::On("on");
         const std::string CStandardValues::Off("off");
         const std::string CStandardValues::Dim("dim");
         const std::string CStandardValues::Open("open");
         const std::string CStandardValues::Close("close");
         const std::string CStandardValues::Stop("stop");

         DECLARE_STANDARD_CAPACITY(ApparentPower, "apparentpower", CStandardUnits::VoltAmpere, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(ArmingAlarm, "armingAlarm", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         DECLARE_STANDARD_CAPACITY(BatteryLevel, "batteryLevel", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(CameraMove, "cameraMove", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         DECLARE_STANDARD_CAPACITY(Counter, "count", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Current, "current", CStandardUnits::Ampere, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Curtain, "curtain", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         DECLARE_STANDARD_CAPACITY(DateTime, "datetime", CStandardUnits::NoUnits, EKeywordDataType::kDateTime);
         DECLARE_STANDARD_CAPACITY(Dimmable, "dimmable", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Direction, "direction", CStandardUnits::Degrees, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Distance, "distance", CStandardUnits::Meter, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Duration, "duration", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Energy, "energy", CStandardUnits::WattPerHour, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Event, "event", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         DECLARE_STANDARD_CAPACITY(Frequency, "frequency", CStandardUnits::Hertz, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Humidity, "humidity", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Illumination, "illumination", CStandardUnits::Lux, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Load, "load", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Message, "message", CStandardUnits::NoUnits, EKeywordDataType::kString);
         DECLARE_STANDARD_CAPACITY(PluginState, "pluginState", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         DECLARE_STANDARD_CAPACITY(Power, "power", CStandardUnits::Watt, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(PowerFactor, "powerFactor", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Pressure, "pressure", CStandardUnits::hectoPascal, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Rain, "rain", CStandardUnits::Millimeter, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(RainRate, "rainrate", CStandardUnits::MillimeterPerSecond, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Rssi, "rssi", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Speed, "speed", CStandardUnits::MetersPerSecond, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Switch, "switch", CStandardUnits::NoUnits, EKeywordDataType::kBool);
         DECLARE_STANDARD_CAPACITY(Temperature, "temperature", CStandardUnits::DegreesCelcius, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Text, "text", CStandardUnits::NoUnits, EKeywordDataType::kString);
         DECLARE_STANDARD_CAPACITY(UpDownStop, "upDownStop", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         DECLARE_STANDARD_CAPACITY(Uv, "uv", CStandardUnits::Uv, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(Voltage, "voltage", CStandardUnits::Volt, EKeywordDataType::kNumeric);
         DECLARE_STANDARD_CAPACITY(WeatherCondition, "weathercondition", CStandardUnits::NoUnits, EKeywordDataType::kString);
         DECLARE_STANDARD_CAPACITY(Weight, "weight", CStandardUnits::Kg, EKeywordDataType::kNumeric);
      }
   }
} // namespace shared::plugin::yPluginApi


