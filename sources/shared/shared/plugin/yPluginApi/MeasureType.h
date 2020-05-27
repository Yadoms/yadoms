#pragma once
#include <shared/enumeration/EnumHelpers.hpp>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief               Measure Type
         ///\note    Used to say Yadoms if data need post-processing before inserting in database
         ///<         - kAbsolute : Absolute data, like temperature, humidity
         ///<         - kIncrement : Say Yadoms that value must be added to current value. Ex : pulse counters (water, energy counters...)
         ///>         - kCumulative : the current value is a total value (like some energy power)
         //-----------------------------------------------------
         DECLARE_ENUM_HEADER(EMeasureType,
            ((Absolute)(0))
            ((Increment)(1))
            ((Cumulative)(3))
         )
      }
   }
} // namespace shared::plugin::yPluginApi::historization
