#include "stdafx.h"
#include "RainRate.h"
#include "../StandardCapacities.h"


namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

   CRainRate::CRainRate(const std::string& keywordName, const EMeasureType& measureType)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::RainRate, EKeywordAccessMode::kGet, 0.0, measureType)
   {
   }

   CRainRate::~CRainRate()
   {
   }

} } } } // namespace shared::plugin::yPluginApi::historization

