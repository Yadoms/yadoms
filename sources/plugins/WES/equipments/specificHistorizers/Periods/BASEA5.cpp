#include "stdafx.h"
#include "BASEA5.h"
#include "TeleInfoCapacities.h"

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESPeriodBASEA5,
   ((WinterNormalCost))
	   ((WinterLowCost))
	   ((SummerNormalCost))
	   ((SummerLowCost))
	   ((PeakCostHours))
	   ((NotDefined))
	   ((FrameError))
	   ((SignalLoss))
	   );

   CPeriodBaseA5::CPeriodBaseA5(const std::string& keywordName)
      : CSingleHistorizableData<EWESPeriodBASEA5>(keywordName,
		                                            CTeleInfoCapacities::PeriodCapacity(),
                                                    shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {}

   CPeriodBaseA5::~CPeriodBaseA5()
   {}
} // namespace specificHistorizers