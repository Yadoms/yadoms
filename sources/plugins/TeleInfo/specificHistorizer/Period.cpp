#include "stdafx.h"
#include "Period.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

//TODO V2 : To be copy in a library teleInfo
namespace teleInfo
{
   namespace specificHistorizers
   {
      DECLARE_ENUM_IMPLEMENTATION(ETeleInfoPeriod,
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

      DECLARE_CAPACITY(PeriodCapacity, "Period", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kEnum);

      CPeriod::CPeriod(const std::string& keywordName)
         : CSingleHistorizableData<ETeleInfoPeriod>(keywordName,
                                                    PeriodCapacity(),
                                                    shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
      {
      }

      CPeriod::~CPeriod()
      {
      }
   }
} // namespace teleInfo::specificHistorizer


