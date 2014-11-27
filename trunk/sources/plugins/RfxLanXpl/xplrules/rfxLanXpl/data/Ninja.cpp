#include "stdafx.h"
#include "Ninja.h"
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION(ENinjaCommand,
      ((On)("on"))
      ((Off)("off"))
      ((StepUp)("step_up"))
      ((StepDown)("step_down"))
      ((RunUp)("run_up"))
      ((RunDown)("run_down"))
      ((Stop)("stop"))
);


const shared::plugin::yadomsApi::CStandardCapacity& NinjaCapacity = shared::plugin::yadomsApi::CStandardCapacity("ninja", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CNinja::CNinja(const std::string& keywordName)
   :shared::plugin::yadomsApi::historization::CSingleHistorizableData<ENinjaCommand>(keywordName, NinjaCapacity, shared::plugin::yadomsApi::EKeywordAccessMode::kGetSet)
{
}

CNinja::~CNinja()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

