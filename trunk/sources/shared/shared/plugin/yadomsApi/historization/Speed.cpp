#include "stdafx.h"
#include "Speed.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CSpeed::CSpeed(const std::string& keywordName, const EMeasureType& measureType)
   :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Speed, "speed", 0.0, measureType)
{
}

CSpeed::~CSpeed()
{
}


} } } } // namespace shared::plugin::yadomsApi::historization

