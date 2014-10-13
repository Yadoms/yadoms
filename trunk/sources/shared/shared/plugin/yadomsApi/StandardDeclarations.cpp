#include "stdafx.h"
#include "StandardCapacities.h"
#include "StandardUnits.h"
#include "StandardValues.h"
#include "shared/StringExtension.h"

namespace shared {   namespace plugin {      namespace yadomsApi      {

   //----------------------------------------------------
   /// \brief This file merge the static declarations of variables
   ///        this trick is used to fix the "static initialization order fiasco"
   //----------------------------------------------------

   const std::string& CStandardUnits::NoUnits(shared::CStringExtension::EmptyString);
   const std::string& CStandardUnits::Ampere("A");
   const std::string& CStandardUnits::DegreesCelcius("°C");
   const std::string& CStandardUnits::DegreesFarenheit("°F");
   const std::string& CStandardUnits::hectoPascal("hPa");
   const std::string& CStandardUnits::Hertz("Hz");
   const std::string& CStandardUnits::Kg("Kg");
   const std::string& CStandardUnits::Meter("m");
   const std::string& CStandardUnits::Millimeter("mm");
   const std::string& CStandardUnits::Percent("%");
   const std::string& CStandardUnits::Volt("V");
   const std::string& CStandardUnits::Watt("W");
   const std::string& CStandardUnits::WattPerHour("Wh");

   const std::string& CStandardValues::On("on");
   const std::string& CStandardValues::Off("off");
   const std::string& CStandardValues::Dim("dim");
   const std::string& CStandardValues::Open("open");
   const std::string& CStandardValues::Close("close");
   const std::string& CStandardValues::Stop("stop");
   
   const CStandardCapacity& CStandardCapacities::BatteryLevel = CStandardCapacity("batteryLevel", CStandardUnits::Percent, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Counter = CStandardCapacity("count", CStandardUnits::NoUnits, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Current = CStandardCapacity("current", CStandardUnits::Ampere, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Curtain = CStandardCapacity("curtain", CStandardUnits::NoUnits, EKeywordAccessMode::kSet, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::Energy = CStandardCapacity("energy", CStandardUnits::WattPerHour, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Frequency = CStandardCapacity("frequency", CStandardUnits::Hertz, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Humidity = CStandardCapacity("humidity", CStandardUnits::Percent, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Message = CStandardCapacity("message", CStandardUnits::NoUnits, EKeywordAccessMode::kGetSet, EKeywordDataType::kString);
   const CStandardCapacity& CStandardCapacities::Power = CStandardCapacity("power", CStandardUnits::Watt, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Pressure = CStandardCapacity("pressure", CStandardUnits::hectoPascal, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Rain = CStandardCapacity("rain", CStandardUnits::Millimeter, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Rssi = CStandardCapacity("rssi", CStandardUnits::Percent, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Switch = CStandardCapacity("switch", CStandardUnits::NoUnits, EKeywordAccessMode::kSet, EKeywordDataType::kNoData);
   const CStandardCapacity& CStandardCapacities::Temperature = CStandardCapacity("temperature", CStandardUnits::DegreesCelcius, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Voltage = CStandardCapacity("voltage", CStandardUnits::Volt, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
   const CStandardCapacity& CStandardCapacities::Weight = CStandardCapacity("weight", CStandardUnits::Kg, EKeywordAccessMode::kGet, EKeywordDataType::kNumeric);
  
} } } // namespace shared::plugin::yadomsApi

