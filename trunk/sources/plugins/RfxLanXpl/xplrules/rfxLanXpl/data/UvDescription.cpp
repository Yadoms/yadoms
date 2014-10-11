#include "stdafx.h"
#include "UvDescription.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION_CUSTOM(EUvDescription,
   ((Low)("low"))
   ((Medium)("medium"))
   ((High)("high"))
   ((VeryHigh)("very high"))
   ((Dangerous)("dangerous"))
);

   const shared::plugin::yadomsApi::CStandardCapacity& UvDescriptionCapacity = shared::plugin::yadomsApi::CStandardCapacity("UvDescription", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordAccessMode::kGet, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CUvDescription::CUvDescription(const std::string& keywordName)
   :CHistorizableEnum<EUvDescription>(keywordName, UvDescriptionCapacity, "command")
{
}

CUvDescription::~CUvDescription()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

