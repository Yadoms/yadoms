#pragma once

#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace weatherunderground
{
   namespace helper
   {
      //-----------------------------------------------------
      ///\brief  All periods
      //-----------------------------------------------------   
      DECLARE_ENUM_HEADER(EPeriod,
         ((NoPeriod))
         ((Day))
         ((Hour))
      ) ;

      //--------------------------------------------------------------
      /// \brief	The map of returned icons from the web site
      //--------------------------------------------------------------
      typedef std::map<std::string, std::string> EnumValuesNames;
      extern const EnumValuesNames EEnumTypeNames;
   }
}