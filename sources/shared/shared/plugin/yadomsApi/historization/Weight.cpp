#include "stdafx.h"
#include "Weight.h"
#include "../StandardCapacities.h"


namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

   CWeight::CWeight(const std::string& keywordName, const EMeasureType& measureType)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Weight, "weight", 0.0, measureType)
   {
   }

   CWeight::~CWeight()
   {
   }


} } } } // namespace shared::plugin::yadomsApi::historization

