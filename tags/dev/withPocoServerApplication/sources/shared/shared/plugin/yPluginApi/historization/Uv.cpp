#include "stdafx.h"
#include "Uv.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

   CUv::CUv(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType, typeInfo::CDoubleTypeInfo & additionalInfo)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Uv, accessMode, 0.0, measureType, additionalInfo)
   {
   }

   CUv::~CUv()
   {
   }

} } } } // namespace shared::plugin::yPluginApi::historization

