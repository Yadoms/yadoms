#include "stdafx.h"
#include "DayOfWeek.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EDayOfWeek,
                               ((Monday))
                               ((Thuesday))
                               ((Wednesday))
                               ((Thursday))
                               ((Friday))
                               ((Saturday))
                               ((Sunday))
   );

   DECLARE_CAPACITY(EDayOfWeekCapacity,
                    "DayOfWeek",
                    yApi::CStandardUnits::NoUnit(),
                    yApi::EKeywordDataType::kEnum);

   CDayOfWeek::CDayOfWeek(const std::string& keywordName)
      : CSingleHistorizableData<EDayOfWeek>(keywordName,
                                            EDayOfWeekCapacity(),
                                            yApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
