#include "stdafx.h"
#include "Frequency.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

   CFrequency::CFrequency(const std::string& keywordName, const EMeasureType& measureType)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Frequency, "frequency", 0.0, measureType)
   {
   }

   CFrequency::~CFrequency()
   {
   }

} } } } // namespace shared::plugin::yadomsApi::historization

