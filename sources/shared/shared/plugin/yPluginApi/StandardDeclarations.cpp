#include "stdafx.h"
#include "StandardCapacities.h"
#include "StandardUnits.h"
#include "StandardValues.h"
#include "shared/StringExtension.h"

namespace shared { namespace plugin { namespace yPluginApi {

   //----------------------------------------------------
   /// \brief This file merge the static declarations of variables
   ///        this trick is used to fix the "static initialization order fiasco"
   //----------------------------------------------------

   const std::string& CStandardUnits::NoUnits(CStringExtension::EmptyString);
   const std::string& CStandardUnits::Ampere("A");
   const std::string& CStandardUnits::Degrees("°");
   const std::string& CStandardUnits::DegreesCelcius("°C");
   const std::string& CStandardUnits::DegreesFarenheit("°F");
   const std::string& CStandardUnits::hectoPascal("hPa");
   const std::string& CStandardUnits::Hertz("Hz");
   const std::string& CStandardUnits::Kg("Kg");
   const std::string& CStandardUnits::Meter("m");
   const std::string& CStandardUnits::MetersPerSecond("m/s");
   const std::string& CStandardUnits::Millimeter("mm");
   const std::string& CStandardUnits::MillimeterPerSecond("mm/s");
   const std::string& CStandardUnits::Percent("%");
   const std::string& CStandardUnits::Uv("UV");
   const std::string& CStandardUnits::Volt("V");
   const std::string& CStandardUnits::Watt("W");
   const std::string& CStandardUnits::WattPerHour("Wh");

   const std::string& CStandardValues::On("on");
   const std::string& CStandardValues::Off("off");
   const std::string& CStandardValues::Dim("dim");
   const std::string& CStandardValues::Open("open");
   const std::string& CStandardValues::Close("close");
   const std::string& CStandardValues::Stop("stop");
   
   const CStandardCapacity& CStandardCapacities::ArmingAlarm = CStandardCapacity("armingAlarm", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::BatteryLevel = CStandardCapacity("batteryLevel", CStandardUnits::Percent, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::CameraMove = CStandardCapacity("cameraMove", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::Counter = CStandardCapacity("count", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Current = CStandardCapacity("current", CStandardUnits::Ampere, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Curtain = CStandardCapacity("curtain", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::DateTime = CStandardCapacity("datetime", CStandardUnits::NoUnits, EKeywordDataType::kString);
   const CStandardCapacity& CStandardCapacities::Dimmable = CStandardCapacity("dimmable", CStandardUnits::Percent, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Direction = CStandardCapacity("direction", CStandardUnits::Degrees, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Energy = CStandardCapacity("energy", CStandardUnits::WattPerHour, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Event = CStandardCapacity("event", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::Frequency = CStandardCapacity("frequency", CStandardUnits::Hertz, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Humidity = CStandardCapacity("humidity", CStandardUnits::Percent, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Load = CStandardCapacity("load", CStandardUnits::Percent, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Message = CStandardCapacity("message", CStandardUnits::NoUnits, EKeywordDataType::kString);
   const CStandardCapacity& CStandardCapacities::Power = CStandardCapacity("power", CStandardUnits::Watt, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::PowerFactor = CStandardCapacity("powerFactor", CStandardUnits::NoUnits, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Pressure = CStandardCapacity("pressure", CStandardUnits::hectoPascal, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Rain = CStandardCapacity("rain", CStandardUnits::Millimeter, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::RainRate = CStandardCapacity("rainrate", CStandardUnits::MillimeterPerSecond, EKeywordDataType::kNumeric);
	const CStandardCapacity& CStandardCapacities::Rssi = CStandardCapacity("rssi", CStandardUnits::Percent, EKeywordDataType::kNumeric);
	const CStandardCapacity& CStandardCapacities::Speed = CStandardCapacity("speed", CStandardUnits::MetersPerSecond, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Switch = CStandardCapacity("switch", CStandardUnits::NoUnits, EKeywordDataType::kBool);
   const CStandardCapacity& CStandardCapacities::Temperature = CStandardCapacity("temperature", CStandardUnits::DegreesCelcius, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::UpDownStop = CStandardCapacity("upDownStop", CStandardUnits::NoUnits, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::Uv = CStandardCapacity("uv", CStandardUnits::Uv, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Voltage = CStandardCapacity("voltage", CStandardUnits::Volt, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Weight = CStandardCapacity("weight", CStandardUnits::Kg, EKeywordDataType::kNumeric);

} } } // namespace shared::plugin::yPluginApi

