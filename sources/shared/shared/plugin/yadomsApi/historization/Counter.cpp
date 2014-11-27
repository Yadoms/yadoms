#include "stdafx.h"
#include "Counter.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

   CCounter::CCounter(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType)
      :CSingleHistorizableData<int>(keywordName, CStandardCapacities::Counter, accessMode, 0, measureType)
   {
   }

   CCounter::~CCounter()
   {
   }

} } } } // namespace shared::plugin::yadomsApi::historization

