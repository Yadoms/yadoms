#include "stdafx.h"
#include "BT5SUP36.h"
#include "TeleInfoCapacities.h"

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESPeriodBT5SUP36,
   ((WinterNormalCost))
	   ((WinterLowCost))
	   ((SummerNormalCost))
	   ((SummerLowCost))
	   ((PeakCostHours))
	   ((NotDefined))
	   ((FrameError))
	   ((SignalLoss))
	   );

   CPeriodBT5SUP36::CPeriodBT5SUP36(const std::string& keywordName)
      : CSingleHistorizableData<EWESPeriodBT5SUP36>(
		  keywordName,
		  CTeleInfoCapacities::PeriodCapacity(),
          shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {}

   CPeriodBT5SUP36::~CPeriodBT5SUP36()
   {}
} // namespace specificHistorizers