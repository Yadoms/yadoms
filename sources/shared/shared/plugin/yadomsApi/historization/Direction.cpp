#include "stdafx.h"
#include "Direction.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CDirection::CDirection(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType)
   :CSingleHistorizableData<int>(keywordName, CStandardCapacities::Direction, accessMode, 0, measureType)
{
}

CDirection::~CDirection()
{
}


} } } } // namespace shared::plugin::yadomsApi::historization

