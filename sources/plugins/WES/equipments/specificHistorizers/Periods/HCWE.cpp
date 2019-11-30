#include "stdafx.h"
#include "HCWE.h"
#include "TeleInfoCapacities.h"

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESPeriodHCWE,
      ((LowCostHours))
      ((NormalCostHours))
      ((WEHours))
	  ((NotDefined))
	  ((FrameError))
	  ((SignalLoss))
   );

   CPeriodHCWE::CPeriodHCWE(const std::string& keywordName)
      : CSingleHistorizableData<EWESPeriodHCWE>(
		  keywordName,
		  CTeleInfoCapacities::PeriodCapacity(),
          shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {}

   CPeriodHCWE::~CPeriodHCWE()
   {}
} // namespace specificHistorizers