#include "stdafx.h"
#include "HpHc.h"
#include "TeleInfoCapacities.h"

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESPeriodHpHc,
      ((LowCostHours))
      ((NormalCostHours))
	  ((NotDefined))
	  ((FrameError))
	  ((SignalLoss))
   );

   CPeriodHpHc::CPeriodHpHc(const std::string& keywordName)
      : CSingleHistorizableData<EWESPeriodHpHc>(
		  keywordName,
		  CTeleInfoCapacities::PeriodCapacity(),
          shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {}

   CPeriodHpHc::~CPeriodHpHc()
   {}
} // namespace specificHistorizers