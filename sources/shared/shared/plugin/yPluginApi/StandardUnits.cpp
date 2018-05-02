#include "stdafx.h"
#include "StandardCapacities.h"
#include "StandardUnits.h"
#include "StandardValues.h"

#define DECLARE_UNIT(cppName, name) \
   const std::string& CStandardUnits::cppName() \
   { \
      static const std::string unit(name); \
      return unit; \
   }

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         DECLARE_UNIT(NoUnit, "data.units.noUnit");
         DECLARE_UNIT(Ampere, "data.units.ampere");
         DECLARE_UNIT(AmperePerHOur, "data.units.ampereHour");
         DECLARE_UNIT(CubicMetre, "data.units.cubicMetre");
         DECLARE_UNIT(CubicMeterPerSecond, "data.units.cubicMeterPerSecond");
         DECLARE_UNIT(Decibel, "data.units.decibel");
         DECLARE_UNIT(DecibelPerMilliWatt, "data.units.decibelPerMilliWatt");
         DECLARE_UNIT(Degrees, "data.units.degrees");
         DECLARE_UNIT(DegreesCelcius, "data.units.degreesCelcius");
         DECLARE_UNIT(DegreesFarenheit, "data.units.degreesFarenheit");
         DECLARE_UNIT(hectoPascal, "data.units.hectoPascal");
         DECLARE_UNIT(Hertz, "data.units.hertz");
         DECLARE_UNIT(Kg, "data.units.kg");
         DECLARE_UNIT(Lux, "data.units.lux");
         DECLARE_UNIT(Meter, "data.units.meter");
         DECLARE_UNIT(MetersPerSecond, "data.units.metersPerSecond");
         DECLARE_UNIT(Millimeter, "data.units.millimeter");
         DECLARE_UNIT(MillimeterPerSecond, "data.units.millimeterPerSecond");
         DECLARE_UNIT(Percent, "data.units.percent");
         DECLARE_UNIT(Second, "data.units.second");
         DECLARE_UNIT(Uv, "data.units.uv");
         DECLARE_UNIT(Volt, "data.units.volt");
         DECLARE_UNIT(VoltAmpere, "data.units.voltAmpere");
         DECLARE_UNIT(Watt, "data.units.watt");
         DECLARE_UNIT(WattPerHour, "data.units.wattPerHour");
      }
   }
} // namespace shared::plugin::yPluginApi


