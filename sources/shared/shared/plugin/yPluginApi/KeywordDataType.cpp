#include "stdafx.h"
#include "KeywordDataType.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         DECLARE_ENUM_IMPLEMENTATION(EKeywordDataType,
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


