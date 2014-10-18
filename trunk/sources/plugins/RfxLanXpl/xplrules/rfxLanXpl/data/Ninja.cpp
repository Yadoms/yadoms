#include "stdafx.h"
#include "Ninja.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION_CUSTOM(ENinjaCommand,
      ((On)("on"))
      ((Off)("off"))
      ((StepUp)("step_up"))
      ((StepDown)("step_down"))
      ((RunUp)("run_up"))
      ((RunDown)("run_down"))
      ((Stop)("stop"))

);


const shared::plugin::yadomsApi::CStandardCapacity& NinjaCapacity = shared::plugin::yadomsApi::CStandardCapacity("ninja", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordAccessMode::kGetSet, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CNinja::CNinja(const std::string& keywordName)
   :CHistorizableEnum<ENinjaCommand>(keywordName, NinjaCapacity, "command")
{
}

CNinja::~CNinja()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

