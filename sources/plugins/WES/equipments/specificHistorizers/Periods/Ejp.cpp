#include "stdafx.h"
#include "Ejp.h"
#include "TeleInfoCapacities.h"

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESPeriodEjp,
	   ((NormalCostHours))
	   ((MobilePeakCostHours))
       ((NotDefined))
   );

   CPeriodEjp::CPeriodEjp(const std::string& keywordName)
      : CSingleHistorizableData<EWESPeriodEjp>(
		  keywordName,
		  CTeleInfoCapacities::PeriodCapacity(),
          shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {}

   CPeriodEjp::~CPeriodEjp()
   {}
} // namespace specificHistorizers