#include "stdafx.h"
#include "StandardCapacities.h"
#include "StandardUnits.h"


namespace shared {   namespace plugin {      namespace yadomsApi      {

   const CStandardCapacity& CStandardCapacities::Temperature = CStandardCapacity("temperature", CStandardUnits::DegreesCelcius, kReadOnly, kDecimal);
   const CStandardCapacity& CStandardCapacities::BatteryLevel = CStandardCapacity("batteryLevel", CStandardUnits::Percent, kReadOnly, kInteger);
   const CStandardCapacity& CStandardCapacities::Rssi = CStandardCapacity("rssi", CStandardUnits::Percent, kReadOnly, kInteger);
   const CStandardCapacity& CStandardCapacities::Switch = CStandardCapacity("switch", CStandardUnits::NoUnits, kWriteOnly, kNoData);
   const CStandardCapacity& CStandardCapacities::Message = CStandardCapacity("message", CStandardUnits::NoUnits, kReadWrite, kString);

} } } // namespace shared::plugin::yadomsApi

