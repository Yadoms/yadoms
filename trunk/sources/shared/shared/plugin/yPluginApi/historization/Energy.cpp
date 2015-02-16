#include "stdafx.h"
#include "Energy.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

   CEnergy::CEnergy(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType, typeInfo::CDoubleTypeInfo & additionalInfo)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Energy, accessMode, 0.0, measureType, additionalInfo)
   {
   }

   CEnergy::~CEnergy()
   {
   }

} } } } // namespace shared::plugin::yPluginApi::historization

