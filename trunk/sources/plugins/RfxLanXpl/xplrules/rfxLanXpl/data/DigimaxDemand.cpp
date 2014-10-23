#include "stdafx.h"
#include "DigimaxDemand.h"
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION(EDigimaxDemand,
   ((HeaterOn)("heater_on"))
   ((HeaterOff)("heater_off"))
   ((CoolingOn)("cooling_on"))
   ((CoolingOff)("cooling_off"))
);


const shared::plugin::yadomsApi::CStandardCapacity& DigimaxDemandCapacity = shared::plugin::yadomsApi::CStandardCapacity("digimax_demand", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordAccessMode::kGetSet, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   
CDigimaxDemand::CDigimaxDemand(const std::string& keywordName)
   :shared::plugin::yadomsApi::historization::CSingleHistorizableData<EDigimaxDemand>(keywordName, DigimaxDemandCapacity, "command")
{
}

CDigimaxDemand::~CDigimaxDemand()
{
}




















} } } // namespace xplrules::rfxLanXpl::data

