#include "stdafx.h"
#include "UvDescription.h"
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION(EUvDescription,
   ((Low)("low"))
   ((Medium)("medium"))
   ((High)("high"))
   ((VeryHigh)("very high"))
   ((Dangerous)("dangerous"))
);

const shared::plugin::yadomsApi::CStandardCapacity& UvDescriptionCapacity = shared::plugin::yadomsApi::CStandardCapacity("UvDescription", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CUvDescription::CUvDescription(const std::string& keywordName)
   :shared::plugin::yadomsApi::historization::CSingleHistorizableData<EUvDescription>(keywordName, UvDescriptionCapacity, "command", shared::plugin::yadomsApi::EKeywordAccessMode::kGet)
{
}

CUvDescription::~CUvDescription()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

