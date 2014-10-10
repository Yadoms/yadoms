#include "stdafx.h"
#include "MertikStatus.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION_CUSTOM(EMertikStatus,
   ((On)("on"))
   ((Off)("off"))
   ((StepUp)("step_up"))
   ((StepDown)("step_down"))
   ((RunUp)("run_up"))
   ((RunDown)("run_down"))
   ((Stop)("stop"))
);

const shared::plugin::yadomsApi::CStandardCapacity& MertikStatusCapacity = shared::plugin::yadomsApi::CStandardCapacity("mertik", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordAccessMode::kGet, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CMertickStatus::CMertickStatus(const std::string& keywordName)
   :CHistorizableEnum<EMertikStatus>(keywordName, MertikStatusCapacity, "status")
{
}

CMertickStatus::~CMertickStatus()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

