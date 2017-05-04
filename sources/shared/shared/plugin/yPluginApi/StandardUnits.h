#pragma once
#include <shared/Export.h>


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //--------------------------------------------------------------
         /// \brief		Standard units
         //--------------------------------------------------------------
         class YADOMS_SHARED_EXPORT CStandardUnits
         {
         public:
            //--------------------------------------------------------------
            /// \brief		Standard units
            //--------------------------------------------------------------
            static const std::string NoUnits;
            static const std::string Ampere; // A
            static const std::string Degrees; // °
            static const std::string DegreesCelcius; // °C
            static const std::string DegreesFarenheit; // °F
            static const std::string hectoPascal; // hPa
            static const std::string Hertz; // Hz
            static const std::string Kg; // kg
            static const std::string Lux; // lux
            static const std::string Meter; // m
            static const std::string MetersPerSecond; // m/s
            static const std::string Millimeter; // mm
            static const std::string MillimeterPerSecond; // mm/s
            static const std::string Percent; // %
            static const std::string Second; // s
            static const std::string Uv; // UV
            static const std::string Volt; // V
            static const std::string VoltAmpere; // VA
            static const std::string Watt; // W
            static const std::string WattPerHour; // Wh
         };
      }
   }
} // namespace shared::plugin::yPluginApi


