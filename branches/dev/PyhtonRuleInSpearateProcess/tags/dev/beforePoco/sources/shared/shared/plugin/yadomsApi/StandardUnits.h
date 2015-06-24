#pragma once
#include <shared/Export.h>


namespace shared { namespace plugin { namespace yadomsApi
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
      static const std::string& NoUnits;
      static const std::string& DegreesCelcius;    // �C                
      static const std::string& DegreesFarenheit;  // �F
      static const std::string& Kg;                // kg              
      static const std::string& Meter;             // m
      static const std::string& Percent;           // %
      static const std::string& Volt;              // V
      static const std::string& Ampere;            // A
   };

} } } // namespace shared::plugin::yadomsApi

