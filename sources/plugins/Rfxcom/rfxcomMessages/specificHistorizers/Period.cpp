#include "stdafx.h"
#include "Period.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace teleInfo
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

CPeriod::CPeriod(const std::string& keywordName)
   :CSingleHistorizableData<EPeriod>(keywordName, PeriodCapacity, shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
{
}

CPeriod::~CPeriod()
{}

} // namespace teleInfo