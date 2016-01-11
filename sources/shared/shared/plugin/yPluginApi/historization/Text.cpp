#include "stdafx.h"
#include "Text.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

CText::CText(const std::string& keywordName, const EKeywordAccessMode& accessMode, const EMeasureType& measureType, typeInfo::CStringTypeInfo & additionalInfo)
   :CSingleHistorizableData<std::string>(keywordName, CStandardCapacities::Text, accessMode, std::string(), measureType, additionalInfo)
{
}

CText::~CText()
{
}


} } } } // namespace shared::plugin::yPluginApi::historization

