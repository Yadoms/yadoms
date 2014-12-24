#include "stdafx.h"
#include "Distance.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   CDistance::CDistance(const std::string& keywordName, const EKeywordAccessMode& accessMode)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Distance, accessMode, 0.0)
   {
   }

   CDistance::~CDistance()
   {
   }

} } } } // namespace shared::plugin::yPluginApi::historization
