#include "stdafx.h"
#include "Base.h"
#include "TeleInfoCapacities.h"

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EWESPeriodBase,
	  ((AllHours))
      ((NotDefined))
      ((FrameError))
      ((SignalLoss))
   );

   CPeriodBase::CPeriodBase(const std::string& keywordName)
      : CSingleHistorizableData<EWESPeriodBase>(keywordName,
		                                        CTeleInfoCapacities::PeriodCapacity(),
                                                shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {}

   CPeriodBase::~CPeriodBase()
   {}
} // namespace specificHistorizers