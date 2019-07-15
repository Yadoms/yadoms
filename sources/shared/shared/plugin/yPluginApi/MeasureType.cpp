#include "stdafx.h"
#include "MeasureType.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         DECLARE_ENUM_IMPLEMENTATION(EMeasureType,
            ((Absolute))
            ((Increment))
            ((Cumulative))
         )
      }
   }
} // namespace shared::plugin::yPluginApi::historization
