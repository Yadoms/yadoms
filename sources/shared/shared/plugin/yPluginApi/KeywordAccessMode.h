#pragma once
#include <shared/Export.h>
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
         DECLARE_ENUM_HEADER_SHARED(EKeywordAccessMode, YADOMS_SHARED_EXPORT,
            ((NoAccess))
            ((Get))
            ((GetSet))
         )
      }
   }
} // namespace shared::plugin::yPluginApi	


