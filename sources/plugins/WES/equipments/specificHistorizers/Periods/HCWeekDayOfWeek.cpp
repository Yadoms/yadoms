#include "stdafx.h"
#include "HCWeekDayOfWeek.h"
#include "TeleInfoCapacities.h"

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESPeriodHCWeekDayofweek,
   ((LowCostWeek))
	   ((NormalCostWeek))
	   ((LowCostWeekEnd))
	   ((NormalCostWeekEnd))
	   ((LowCostDayOfWeek))
	   ((NormalCostDayOfWeek))
	   ((NotDefined))
	   ((FrameError))
	   ((SignalLoss))
	   );

   CPeriodHCWeekDayofweek::CPeriodHCWeekDayofweek(const std::string& keywordName)
      : CSingleHistorizableData<EWESPeriodHCWeekDayofweek>(
		  keywordName,
		  CTeleInfoCapacities::PeriodCapacity(),
          shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {}

   CPeriodHCWeekDayofweek::~CPeriodHCWeekDayofweek()
   {}
} // namespace specificHistorizers