#include "stdafx.h"
#include "Pressure.h"
#include "../StandardCapacities.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CPressure::CPressure(const std::string& keywordName, const EMeasureType& measureType)
   :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Pressure, "pressure", 0, measureType)
{
}

CPressure::~CPressure()
{
}

} } } } // namespace shared::plugin::yadomsApi::historization

