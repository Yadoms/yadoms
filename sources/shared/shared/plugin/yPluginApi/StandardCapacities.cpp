#include "stdafx.h"
#include "StandardCapacities.h"
#include "StandardUnits.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         DECLARE_CAPACITY(CStandardCapacities::ApparentPower, "apparentpower", CStandardUnits::VoltAmpere(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::ArmingAlarm, "armingAlarm", CStandardUnits::NoUnits(), EKeywordDataType::kEnum);
         DECLARE_CAPACITY(CStandardCapacities::BatteryLevel, "batteryLevel", CStandardUnits::Percent(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::CameraMove, "cameraMove", CStandardUnits::NoUnits(), EKeywordDataType::kEnum);
         DECLARE_CAPACITY(CStandardCapacities::Counter, "count", CStandardUnits::NoUnits(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Current, "current", CStandardUnits::Ampere(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Curtain, "curtain", CStandardUnits::NoUnits(), EKeywordDataType::kEnum);
         DECLARE_CAPACITY(CStandardCapacities::DateTime, "datetime", CStandardUnits::NoUnits(), EKeywordDataType::kDateTime);
         DECLARE_CAPACITY(CStandardCapacities::Dimmable, "dimmable", CStandardUnits::Percent(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Direction, "direction", CStandardUnits::Degrees(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Distance, "distance", CStandardUnits::Meter(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Duration, "duration", CStandardUnits::NoUnits(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Energy, "energy", CStandardUnits::WattPerHour(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Event, "event", CStandardUnits::NoUnits(), EKeywordDataType::kNoData);
         DECLARE_CAPACITY(CStandardCapacities::Frequency, "frequency", CStandardUnits::Hertz(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Humidity, "humidity", CStandardUnits::Percent(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Illumination, "illumination", CStandardUnits::Lux(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Load, "load", CStandardUnits::Percent(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Message, "message", CStandardUnits::NoUnits(), EKeywordDataType::kJson);
         DECLARE_CAPACITY(CStandardCapacities::PluginState, "pluginState", CStandardUnits::NoUnits(), EKeywordDataType::kEnum);
         DECLARE_CAPACITY(CStandardCapacities::Power, "power", CStandardUnits::Watt(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::PowerFactor, "powerFactor", CStandardUnits::NoUnits(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Pressure, "pressure", CStandardUnits::hectoPascal(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Rain, "rain", CStandardUnits::Millimeter(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::RainRate, "rainrate", CStandardUnits::MillimeterPerSecond(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Rssi, "rssi", CStandardUnits::Percent(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Speed, "speed", CStandardUnits::MetersPerSecond(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Switch, "switch", CStandardUnits::NoUnits(), EKeywordDataType::kBool);
         DECLARE_CAPACITY(CStandardCapacities::Temperature, "temperature", CStandardUnits::DegreesCelcius(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Text, "text", CStandardUnits::NoUnits(), EKeywordDataType::kString);
         DECLARE_CAPACITY(CStandardCapacities::UpDownStop, "upDownStop", CStandardUnits::NoUnits(), EKeywordDataType::kEnum);
         DECLARE_CAPACITY(CStandardCapacities::Uv, "uv", CStandardUnits::Uv(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::Voltage, "voltage", CStandardUnits::Volt(), EKeywordDataType::kNumeric);
         DECLARE_CAPACITY(CStandardCapacities::WeatherCondition, "weathercondition", CStandardUnits::NoUnits(), EKeywordDataType::kString);
         DECLARE_CAPACITY(CStandardCapacities::Weight, "weight", CStandardUnits::Kg(), EKeywordDataType::kNumeric);
      }
   }
} // namespace shared::plugin::yPluginApi


