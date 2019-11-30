#include "stdafx.h"
#include "Tempo.h"
#include "TeleInfoCapacities.h"

namespace specificHistorizers
{
	DECLARE_ENUM_IMPLEMENTATION(EWESPeriodTempo,
	((LowCostBlueDays))
		((LowCostWhiteDays))
		((LowCostRedDays))
		((NormalCostBlueDays))
		((NormalCostWhiteDays))
		((NormalCostRedDays))
		((NotDefined))
		((FrameError))
		((SignalLoss))
		);

   CPeriodTempo::CPeriodTempo(const std::string& keywordName)
      : CSingleHistorizableData<EWESPeriodTempo>(
		  keywordName,
		  CTeleInfoCapacities::PeriodCapacity(),
          shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {}

   CPeriodTempo::~CPeriodTempo()
   {}
} // namespace specificHistorizers