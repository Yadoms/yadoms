#include "stdafx.h"
#include "Period.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

//TODO V2 : To be copy in a library teleInfo
namespace linky
{
   namespace specificHistorizers
   {
      DECLARE_ENUM_IMPLEMENTATION(EPeriod,
         ((AllHours)("TH"))
         ((LowCostHours)("HC"))
         ((PeakCostHours)("HP"))
         ((NormalCostHours)("HN"))
         ((MobilePeakCostHours)("PM"))
         ((LowCostBlueDays)("HCJB"))
         ((LowCostWhiteDays)("HCJW"))
         ((LowCostRedDays)("HCJR"))
         ((NormalCostBlueDays)("HPJB"))
         ((NormalCostWhiteDays)("HPJW"))
         ((NormalCostRedDays)("HPJR"))
       );

      DECLARE_CAPACITY(PeriodCapacity, "Period", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

      CPeriod::CPeriod(const std::string& keywordName)
         : CSingleHistorizableData<EPeriod>(keywordName,
                                            PeriodCapacity(),
                                            shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
      {
      }

      CPeriod::~CPeriod()
      {
      }
   }
} // namespace linky::specificHistorizer


