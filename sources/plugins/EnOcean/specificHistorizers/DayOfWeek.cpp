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
                    shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
                    shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CDayOfWeek::CDayOfWeek(const std::string& keywordName)
      : CSingleHistorizableData<EDayOfWeek>(keywordName,
                                            EDayOfWeekCapacity(),
                                            shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }
} // namespace specificHistorizers
