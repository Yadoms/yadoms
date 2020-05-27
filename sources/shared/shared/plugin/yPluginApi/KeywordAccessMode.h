#pragma once
#include <shared/enumeration/EnumHelpers.hpp>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief Keyword access mode
         //-----------------------------------------------------
         DECLARE_ENUM_HEADER(EKeywordAccessMode,
            ((NoAccess))
            ((Get))
            ((GetSet))
         )
      }
   }
} // namespace shared::plugin::yPluginApi	


