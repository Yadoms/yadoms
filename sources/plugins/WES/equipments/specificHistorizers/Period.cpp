#include "stdafx.h"
#include "Period.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESTeleInfoPeriod,
      ((NotDefined))
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
      ((WinterNormalCost))
      ((WinterLowCost))
      ((SummerNormalCost))
      ((SummerLowCost))
      ((WinterCost))
   );

   DECLARE_CAPACITY(PeriodCapacity, "Period", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CPeriod::CPeriod(const std::string& keywordName)
      : CSingleHistorizableData<EWESTeleInfoPeriod>(keywordName,
                                                    PeriodCapacity(),
                                                    shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }

   CPeriod::~CPeriod()
   {
   }
} // namespace specificHistorizers