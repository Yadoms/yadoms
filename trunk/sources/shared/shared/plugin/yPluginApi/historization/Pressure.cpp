#include "stdafx.h"
#include "Pressure.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

CPressure::CPressure(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType)
   :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Pressure, EKeywordAccessMode::kGet, 0, measureType)
{
}

CPressure::~CPressure()
{
}

} } } } // namespace shared::plugin::yPluginApi::historization

