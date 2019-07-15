#pragma once
#include <shared/enumeration/EnumHelpers.hpp>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief               History depth policy
         ///\note    Used to say Yadoms history policy for keyword
         ///<         - kDefault : Default policy : keep 30-days values (by default, see yadoms.ini), and hour/day/month/year summary
         ///<         - kNoHistory : No history, keep only last acquisition
         //-----------------------------------------------------
         DECLARE_ENUM_HEADER_SHARED(EHistoryDepth, YADOMS_SHARED_EXPORT,
            ((Default)(0))
            ((NoHistory)(1))
         )
      }
   }
} // namespace shared::plugin::yPluginApi::historization
