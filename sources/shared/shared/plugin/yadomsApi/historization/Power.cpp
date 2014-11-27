#include "stdafx.h"
#include "Power.h"
#include "../StandardCapacities.h"


namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CPower::CPower(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType)
   :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Power, accessMode, 0.0, measureType)
{
}

CPower::~CPower()
{
}


} } } } // namespace shared::plugin::yadomsApi::historization

