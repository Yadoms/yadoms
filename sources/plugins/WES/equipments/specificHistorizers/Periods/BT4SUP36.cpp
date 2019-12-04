#include "stdafx.h"
#include "BT4SUP36.h"
#include "TeleInfoCapacities.h"

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESPeriodBT4SUP36,
   ((WinterNormalCost))
	   ((WinterLowCost))
	   ((SummerNormalCost))
	   ((SummerLowCost))
	   ((NotDefined))
	   );

   CPeriodBT4SUP36::CPeriodBT4SUP36(const std::string& keywordName)
      : CSingleHistorizableData<EWESPeriodBT4SUP36>(
		  keywordName,
		  CTeleInfoCapacities::PeriodCapacity(),
          shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {}

   CPeriodBT4SUP36::~CPeriodBT4SUP36()
   {}
} // namespace specificHistorizers