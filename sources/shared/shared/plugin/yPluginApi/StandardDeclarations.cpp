#include "stdafx.h"
#include "StandardCapacities.h"
#include "StandardUnits.h"
#include "StandardValues.h"

namespace shared { namespace plugin { namespace yPluginApi {

   //----------------------------------------------------
   /// \brief This file merge the static declarations of variables
   ///        this trick is used to fix the "static initialization order fiasco"
   //----------------------------------------------------

   const std::string& CStandardUnits::NoUnits("data.units.noUnit");
   const std::string& CStandardUnits::Ampere("data.units.ampere");
   const std::string& CStandardUnits::Decibel("data.units.db");
   const std::string& CStandardUnits::DecibelPower("data.units.dbm");
   const std::string& CStandardUnits::Degrees("data.units.degrees");
   const std::string& CStandardUnits::DegreesCelcius("data.units.degreesCelcius");
   const std::string& CStandardUnits::DegreesFarenheit("data.units.degreesFarenheit");
   const std::string& CStandardUnits::hectoPascal("data.units.hectoPascal");
   const std::string& CStandardUnits::Hertz("data.units.hertz");
   const std::string& CStandardUnits::Kg("data.units.kg");
   const std::string& CStandardUnits::Lux("data.units.lux");
   const std::string& CStandardUnits::Meter("data.units.meter");
   const std::string& CStandardUnits::MetersPerSecond("data.units.metersPerSecond");
   const std::string& CStandardUnits::Millimeter("data.units.millimeter");
   const std::string& CStandardUnits::MillimeterPerSecond("data.units.millimeterPerSecond");
   const std::string& CStandardUnits::Percent("data.units.percent");
   const std::string& CStandardUnits::Second("data.units.second");
   const std::string& CStandardUnits::Uv("data.units.uv");
   const std::string& CStandardUnits::Volt("data.units.volt");
   const std::string& CStandardUnits::VoltAmpere("data.units.voltAmpere");
   const std::string& CStandardUnits::Watt("data.units.watt");
   const std::string& CStandardUnits::WattPerHour("data.units.wattPerHour");

   const std::string& CStandardValues::On("on");
   const std::string& CStandardValues::Off("off");
   const std::string& CStandardValues::Dim("dim");
   const std::string& CStandardValues::Open("open");
   const std::string& CStandardValues::Close("close");
   const std::string& CStandardValues::Stop("stop");
 
   const CStandardCapacity& CStandardCapacities::Alarm = CStandardCapacity("alarm", CStandardUnits::NoUnits, EKeywordDataType::kBool);
   const CStandardCapacity& CStandardCapacities::ApparentPower = CStandardCapacity("apparentpower", CStandardUnits::VoltAmpere, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::ArmingAlarm = CStandardCapacity("armingAlarm", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::BatteryLevel = CStandardCapacity("batteryLevel", CStandardUnits::Percent, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::CameraMove = CStandardCapacity("cameraMove", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::Counter = CStandardCapacity("count", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Current = CStandardCapacity("current", CStandardUnits::Ampere, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Curtain = CStandardCapacity("curtain", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::DateTime = CStandardCapacity("datetime", CStandardUnits::NoUnits, EKeywordDataType::kDateTime);
   const CStandardCapacity& CStandardCapacities::Dimmable = CStandardCapacity("dimmable", CStandardUnits::Percent, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Direction = CStandardCapacity("direction", CStandardUnits::Degrees, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Distance = CStandardCapacity("distance", CStandardUnits::Meter, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Duration = CStandardCapacity("duration", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Energy = CStandardCapacity("energy", CStandardUnits::WattPerHour, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Event = CStandardCapacity("event", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::Frequency = CStandardCapacity("frequency", CStandardUnits::Hertz, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Humidity = CStandardCapacity("humidity", CStandardUnits::Percent, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Illumination = CStandardCapacity("illumination", CStandardUnits::Lux, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Load = CStandardCapacity("load", CStandardUnits::Percent, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Message = CStandardCapacity("message", CStandardUnits::NoUnits, EKeywordDataType::kString);
   const CStandardCapacity& CStandardCapacities::PluginState = CStandardCapacity("pluginState", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::Power = CStandardCapacity("power", CStandardUnits::Watt, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::PowerFactor = CStandardCapacity("powerFactor", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Pressure = CStandardCapacity("pressure", CStandardUnits::hectoPascal, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Rain = CStandardCapacity("rain", CStandardUnits::Millimeter, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::RainRate = CStandardCapacity("rainrate", CStandardUnits::MillimeterPerSecond, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Rssi = CStandardCapacity("rssi", CStandardUnits::Percent, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Speed = CStandardCapacity("speed", CStandardUnits::MetersPerSecond, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::SignalLevel = CStandardCapacity("signalLevel", CStandardUnits::Decibel, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::SignalPower = CStandardCapacity("signalPower", CStandardUnits::DecibelPower, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Switch = CStandardCapacity("switch", CStandardUnits::NoUnits, EKeywordDataType::kBool);
   const CStandardCapacity& CStandardCapacities::Tamper = CStandardCapacity("tamper", CStandardUnits::NoUnits, EKeywordDataType::kBool);
   const CStandardCapacity& CStandardCapacities::Temperature = CStandardCapacity("temperature", CStandardUnits::DegreesCelcius, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Text = CStandardCapacity("text", CStandardUnits::NoUnits, EKeywordDataType::kString);
   const CStandardCapacity& CStandardCapacities::UpDownStop = CStandardCapacity("upDownStop", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::Uv = CStandardCapacity("uv", CStandardUnits::Uv, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Voltage = CStandardCapacity("voltage", CStandardUnits::Volt, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::WeatherCondition = CStandardCapacity("weathercondition", CStandardUnits::NoUnits, EKeywordDataType::kString);
   const CStandardCapacity& CStandardCapacities::Weight = CStandardCapacity("weight", CStandardUnits::Kg, EKeywordDataType::kNumeric);

} } } // namespace shared::plugin::yPluginApi

