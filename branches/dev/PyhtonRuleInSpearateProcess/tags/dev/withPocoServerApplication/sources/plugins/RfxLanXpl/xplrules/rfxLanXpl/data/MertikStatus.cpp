#include "stdafx.h"
#include "MertikStatus.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION(EMertikStatus,
   ((On)("on"))
   ((Off)("off"))
   ((StepUp)("step_up"))
   ((StepDown)("step_down"))
   ((RunUp)("run_up"))
   ((RunDown)("run_down"))
   ((Stop)("stop"))
);

const shared::plugin::yPluginApi::CStandardCapacity& MertikStatusCapacity = shared::plugin::yPluginApi::CStandardCapacity("mertik", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

   
CMertickStatus::CMertickStatus(const std::string& keywordName)
   :shared::plugin::yPluginApi::historization::CSingleHistorizableData<EMertikStatus>(keywordName, MertikStatusCapacity, shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
{
}

CMertickStatus::~CMertickStatus()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

