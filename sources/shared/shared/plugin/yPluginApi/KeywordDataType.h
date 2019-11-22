#pragma once
#include <shared/enumeration/EnumHelpers.hpp>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief Keyword type
         //-----------------------------------------------------
         DECLARE_ENUM_HEADER(EKeywordDataType,
            ((NoData))
            ((String))
            ((Numeric))
            ((Bool))
            ((Json))
            ((Enum))
            ((DateTime))
         )
      }
   }
} // namespace shared::plugin::yPluginApi	


