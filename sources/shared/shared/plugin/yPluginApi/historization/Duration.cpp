#include "stdafx.h"
#include "Duration.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   CDuration::CDuration(const std::string& keywordName, const EKeywordAccessMode& accessMode)
      :CSingleHistorizableData<boost::posix_time::time_duration>(keywordName, CStandardCapacities::Duration, accessMode, 0.0)
   {
   }

   CDuration::~CDuration()
   {
   }

} } } } // namespace shared::plugin::yPluginApi::historization
