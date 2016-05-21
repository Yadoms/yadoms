#include "stdafx.h"
#include "CartelectronicTICPeriod.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace rfxcomMessages { namespace specificHistorizers
{

DECLARE_ENUM_IMPLEMENTATION(EPeriod,
    ((AllHours))
    ((LowCostHours))
	((PeakCostHours))
	((NormalCostHours))
	((MobilePeakCostHours))
	((LowCostBlueDays))
	((LowCostWhiteDays))
	((LowCostRedDays))
	((NormalCostBlueDays))
	((NormalCostWhiteDays))
	((NormalCostRedDays))
);

const shared::plugin::yPluginApi::CStandardCapacity& PeriodCapacity = shared::plugin::yPluginApi::CStandardCapacity("Period", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

CCartelectronicTICPeriod::CCartelectronicTICPeriod(const std::string& keywordName)
   :CSingleHistorizableData<EPeriod>(keywordName, PeriodCapacity, shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
{
}

CCartelectronicTICPeriod::~CCartelectronicTICPeriod()
{}

} } // namespace teleInfoUSB::specificHistorizer