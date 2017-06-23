#include "stdafx.h"
#include "StandardCapacities.h"
#include "StandardUnits.h"
#include "StandardValues.h"


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
         const std::string CStandardUnits::Decibel("data.units.db");
         const std::string CStandardUnits::DecibelPerMilliWatt("data.units.dbm");
         const std::string CStandardUnits::CubicMetre("data.units.cubicMetre");
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

         DECLARE_CAPACITY(CStandardCapacities::Alarm, "alarm", CStandardUnits::NoUnits, EKeywordDataType::kBool);
         DECLARE_CAPACITY(CStandardCapacities::ApparentPower, "apparentpower", CStandardUnits::VoltAmpere, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::ArmingAlarm, "armingAlarm", CStandardUnits::NoUnits, EKeywordDataType::kEnum);
         DECLARE_CAPACITY(CStandardCapacities::BatteryLevel, "batteryLevel", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::CameraMove, "cameraMove", CStandardUnits::NoUnits, EKeywordDataType::kEnum);
         DECLARE_CAPACITY(CStandardCapacities::Counter, "count", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Current, "current", CStandardUnits::Ampere, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Curtain, "curtain", CStandardUnits::NoUnits, EKeywordDataType::kEnum);
         DECLARE_CAPACITY(CStandardCapacities::DateTime, "datetime", CStandardUnits::NoUnits, EKeywordDataType::kDateTime);
         DECLARE_CAPACITY(CStandardCapacities::Dimmable, "dimmable", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Direction, "direction", CStandardUnits::Degrees, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Distance, "distance", CStandardUnits::Meter, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Duration, "duration", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Energy, "energy", CStandardUnits::WattPerHour, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Event, "event", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         DECLARE_CAPACITY(CStandardCapacities::Frequency, "frequency", CStandardUnits::Hertz, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Humidity, "humidity", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Illumination, "illumination", CStandardUnits::Lux, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Load, "load", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Message, "message", CStandardUnits::NoUnits, EKeywordDataType::kJson);
         DECLARE_CAPACITY(CStandardCapacities::PluginState, "pluginState", CStandardUnits::NoUnits, EKeywordDataType::kEnum);
         DECLARE_CAPACITY(CStandardCapacities::Power, "power", CStandardUnits::Watt, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::PowerFactor, "powerFactor", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Pressure, "pressure", CStandardUnits::hectoPascal, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Rain, "rain", CStandardUnits::Millimeter, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::RainRate, "rainrate", CStandardUnits::MillimeterPerSecond, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Rssi, "rssi", CStandardUnits::DecibelPerMilliWatt, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::SignalLevel, "signalLevel", CStandardUnits::Decibel, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::SignalPower, "signalPower", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Speed, "speed", CStandardUnits::MetersPerSecond, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Switch, "switch", CStandardUnits::NoUnits, EKeywordDataType::kBool);
         DECLARE_CAPACITY(CStandardCapacities::Tamper, "tamper", CStandardUnits::NoUnits, EKeywordDataType::kBool);
         DECLARE_CAPACITY(CStandardCapacities::Temperature, "temperature", CStandardUnits::DegreesCelcius, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Text, "text", CStandardUnits::NoUnits, EKeywordDataType::kString);
         DECLARE_CAPACITY(CStandardCapacities::UpDownStop, "upDownStop", CStandardUnits::NoUnits, EKeywordDataType::kEnum);
         DECLARE_CAPACITY(CStandardCapacities::Uv, "uv", CStandardUnits::Uv, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Voltage, "voltage", CStandardUnits::Volt, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Volume, "volume", CStandardUnits::CubicMetre, EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::WeatherCondition, "weathercondition", CStandardUnits::NoUnits, EKeywordDataType::kString);
         DECLARE_CAPACITY(CStandardCapacities::Weight, "weight", CStandardUnits::Kg, EKeywordDataType::kNumeric);
      }
   }
} // namespace shared::plugin::yPluginApi


