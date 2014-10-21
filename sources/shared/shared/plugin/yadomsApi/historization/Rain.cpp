#include "stdafx.h"
#include "Rain.h"
#include "../StandardCapacities.h"


namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

   CRain::CRain(const std::string& keywordName, const EMeasureType& measureType)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Rain, "rain", 0.0, measureType)
   {
   }

   CRain::~CRain()
   {
   }

} } } } // namespace shared::plugin::yadomsApi::historization

