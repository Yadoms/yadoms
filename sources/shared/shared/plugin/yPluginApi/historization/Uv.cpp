#include "stdafx.h"
#include "Uv.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

CUv::CUv(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType)
   :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Uv, accessMode, 0.0, measureType)
{
}

CUv::~CUv()
{
}

} } } } // namespace shared::plugin::yPluginApi::historization

