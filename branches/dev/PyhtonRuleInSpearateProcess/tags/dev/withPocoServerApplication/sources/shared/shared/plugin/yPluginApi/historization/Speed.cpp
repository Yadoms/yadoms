#include "stdafx.h"
#include "Speed.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

   CSpeed::CSpeed(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType, typeInfo::CDoubleTypeInfo & additionalInfo)
      :CSingleHistorizableData<double>(keywordName, CStandardCapacities::Speed, accessMode, 0.0, measureType, additionalInfo)
{
}

CSpeed::~CSpeed()
{
}


} } } } // namespace shared::plugin::yPluginApi::historization

