#include "stdafx.h"
#include "Current.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

   CCurrent::CCurrent(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Current, "current", accessMode, 0.0, measureType)
   {
   }

   CCurrent::~CCurrent()
   {
   }

} } } } // namespace shared::plugin::yadomsApi::historization

