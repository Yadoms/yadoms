#include "stdafx.h"
#include "StandardCapacities.h"


namespace shared { namespace plugin { namespace yadomsApi
{

const CCapacity& CStandardCapacities::getTemperatureSensorCapacity()
{
   static const CCapacity TemperatureSensorCapacity("TemperatureSensor", true, false);
   return TemperatureSensorCapacity;
}

const CCapacity& CStandardCapacities::getBatteryLevelCapacity()
{
   static const CCapacity BatteryLevelCapacity("BatteryLevel", true, false);
   return BatteryLevelCapacity;
}

const CCapacity& CStandardCapacities::getRssiMeasureCapacity()
{
   static const CCapacity RssiMeasureCapacity("RssiMeasure", true, false);
   return RssiMeasureCapacity;
}

} } } // namespace shared::plugin::yadomsApi

