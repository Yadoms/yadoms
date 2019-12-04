#include "stdafx.h"
#include "TJEJP.h"
#include "TeleInfoCapacities.h"

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESPeriodTJEJP,
   ((MobilePeakCostHours))
	   ((WinterCost))
	   ((SummerNormalCost))
	   ((SummerLowCost))
	   ((NotDefined))
	   );

   CPeriodTJEJP::CPeriodTJEJP(const std::string& keywordName)
      : CSingleHistorizableData<EWESPeriodTJEJP>(
		  keywordName,
		  CTeleInfoCapacities::PeriodCapacity(),
          shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {}

   CPeriodTJEJP::~CPeriodTJEJP()
   {}
} // namespace specificHistorizers