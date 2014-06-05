#include "stdafx.h"
#include "StandardCapacities.h"


namespace shared { namespace plugin { namespace yadomsApi
{

const std::string& CStandardCapacities::Temperature("temperature");
const std::string& CStandardCapacities::BatteryLevel("batteryLevel");
const std::string& CStandardCapacities::Rssi("rssi");

const bool CStandardCapacities::Exists(const std::string& capacity)
{
   //TODO
   return true;
}

} } } // namespace shared::plugin::yadomsApi

