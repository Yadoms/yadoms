#include "stdafx.h"
#include "Fan4Speeds.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EFan4Speeds,
                               ((Auto))
                               ((Speed0))
                               ((Speed1))
                               ((Speed2))
                               ((Speed3))
   );

   DECLARE_CAPACITY(Fan4SpeedsCapacity,
                    "Fan4Speeds",
                    shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
                    shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CFan4Speeds::CFan4Speeds(const std::string& keywordName)
      : CSingleHistorizableData<EFan4Speeds>(keywordName,
                                             Fan4SpeedsCapacity(),
                                             shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }
} // namespace specificHistorizers
