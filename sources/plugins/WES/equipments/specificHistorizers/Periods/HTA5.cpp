#include "stdafx.h"
#include "HTA5.h"
#include "TeleInfoCapacities.h"

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESPeriodHTA5,
   ((WinterNormalCost))
	   ((WinterLowCost))
	   ((SummerNormalCost))
	   ((SummerLowCost))
	   ((PeakCostHours))
	   ((NotDefined))
	   );

   CPeriodHTA5::CPeriodHTA5(const std::string& keywordName)
      : CSingleHistorizableData<EWESPeriodHTA5>(
		  keywordName,
		  CTeleInfoCapacities::PeriodCapacity(),
          shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {}

   CPeriodHTA5::~CPeriodHTA5()
   {}
} // namespace specificHistorizers