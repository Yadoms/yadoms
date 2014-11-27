#include "stdafx.h"
#include "Energy.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

   CEnergy::CEnergy(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Energy, accessMode, 0.0, measureType)
   {
   }

   CEnergy::~CEnergy()
   {
   }

} } } } // namespace shared::plugin::yadomsApi::historization

