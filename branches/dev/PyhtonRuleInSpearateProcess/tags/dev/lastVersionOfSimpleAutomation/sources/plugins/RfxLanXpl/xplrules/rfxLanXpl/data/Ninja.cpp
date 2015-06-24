#include "stdafx.h"
#include "Ninja.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

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


const shared::plugin::yPluginApi::CStandardCapacity& NinjaCapacity = shared::plugin::yPluginApi::CStandardCapacity("ninja", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

   
CNinja::CNinja(const std::string& keywordName)
   :shared::plugin::yPluginApi::historization::CSingleHistorizableData<ENinjaCommand>(keywordName, NinjaCapacity, shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
{
}

CNinja::~CNinja()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

