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
   const std::string& CStandardUnits::DegreesCelcius("°C");
   const std::string& CStandardUnits::DegreesFarenheit("°F");
   const std::string& CStandardUnits::Kg("Kg");
   const std::string& CStandardUnits::Meter("m");
   const std::string& CStandardUnits::Percent("%");
   const std::string& CStandardUnits::Volt("V");
   const std::string& CStandardUnits::Ampere("A");

   const std::string& CStandardValues::On("on");
   const std::string& CStandardValues::Off("off");
   const std::string& CStandardValues::Dim("dim");
   const std::string& CStandardValues::Open("open");
   const std::string& CStandardValues::Close("close");
   const std::string& CStandardValues::Stop("stop");
   
   const CStandardCapacity& CStandardCapacities::Temperature = CStandardCapacity("temperature", CStandardUnits::DegreesCelcius, kGet, kNumeric);
   const CStandardCapacity& CStandardCapacities::BatteryLevel = CStandardCapacity("batteryLevel", CStandardUnits::Percent, kGet, kNumeric);
   const CStandardCapacity& CStandardCapacities::Rssi = CStandardCapacity("rssi", CStandardUnits::Percent, kGet, kNumeric);
   const CStandardCapacity& CStandardCapacities::Switch = CStandardCapacity("switch", CStandardUnits::NoUnits, kSet, kNoData);
   const CStandardCapacity& CStandardCapacities::Message = CStandardCapacity("message", CStandardUnits::NoUnits, kGetSet, kString);
  
} } } // namespace shared::plugin::yadomsApi

