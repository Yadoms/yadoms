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

         const CStandardCapacity CStandardCapacities::ApparentPower("apparentpower", CStandardUnits::VoltAmpere, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::ArmingAlarm("armingAlarm", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         const CStandardCapacity CStandardCapacities::BatteryLevel("batteryLevel", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::CameraMove("cameraMove", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         const CStandardCapacity CStandardCapacities::Counter("count", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Current("current", CStandardUnits::Ampere, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Curtain("curtain", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         const CStandardCapacity CStandardCapacities::DateTime("datetime", CStandardUnits::NoUnits, EKeywordDataType::kDateTime);
         const CStandardCapacity CStandardCapacities::Dimmable("dimmable", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Direction("direction", CStandardUnits::Degrees, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Distance("distance", CStandardUnits::Meter, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Duration("duration", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Energy("energy", CStandardUnits::WattPerHour, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Event("event", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         const CStandardCapacity CStandardCapacities::Frequency("frequency", CStandardUnits::Hertz, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Humidity("humidity", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Illumination("illumination", CStandardUnits::Lux, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Load("load", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Message("message", CStandardUnits::NoUnits, EKeywordDataType::kString);
         const CStandardCapacity CStandardCapacities::PluginState("pluginState", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         const CStandardCapacity CStandardCapacities::Power("power", CStandardUnits::Watt, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::PowerFactor("powerFactor", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Pressure("pressure", CStandardUnits::hectoPascal, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Rain("rain", CStandardUnits::Millimeter, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::RainRate("rainrate", CStandardUnits::MillimeterPerSecond, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Rssi("rssi", CStandardUnits::Percent, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Speed("speed", CStandardUnits::MetersPerSecond, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Switch("switch", CStandardUnits::NoUnits, EKeywordDataType::kBool);
         const CStandardCapacity CStandardCapacities::Temperature("temperature", CStandardUnits::DegreesCelcius, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Text("text", CStandardUnits::NoUnits, EKeywordDataType::kString);
         const CStandardCapacity CStandardCapacities::UpDownStop("upDownStop", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
         const CStandardCapacity CStandardCapacities::Uv("uv", CStandardUnits::Uv, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::Voltage("voltage", CStandardUnits::Volt, EKeywordDataType::kNumeric);
         const CStandardCapacity CStandardCapacities::WeatherCondition("weathercondition", CStandardUnits::NoUnits, EKeywordDataType::kString);
         const CStandardCapacity CStandardCapacities::Weight("weight", CStandardUnits::Kg, EKeywordDataType::kNumeric);
      }
   }
} // namespace shared::plugin::yPluginApi


