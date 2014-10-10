#include "stdafx.h"
#include "DigimaxDemand.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION_CUSTOM(EDigimaxDemand,
   ((HeaterOn)("heater_on"))
   ((HeaterOff)("heater_off"))
   ((CoolingOn)("cooling_on"))
   ((CoolingOff)("cooling_off"))
);


const shared::plugin::yadomsApi::CStandardCapacity& DigimaxDemandCapacity = shared::plugin::yadomsApi::CStandardCapacity("digimax_demand", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordAccessMode::kSet, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CDigimaxDemand::CDigimaxDemand(const std::string& keywordName)
   :CHistorizableEnum<EDigimaxDemand>(keywordName, DigimaxDemandCapacity, "command")
{
}

CDigimaxDemand::~CDigimaxDemand()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

