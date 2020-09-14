#include "stdafx.h"
#include "Fan6Speeds.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EFan6Speeds,
                               ((Off))
                               ((Auto))
                               ((Speed0))
                               ((Speed1))
                               ((Speed2))
                               ((Speed3))
                               ((Speed4))
                               ((Speed5))
   );

   DECLARE_CAPACITY(Fan6SpeedsCapacity,
                    "Fan6Speeds",
                    yApi::CStandardUnits::NoUnit(),
                    yApi::EKeywordDataType::kEnum);

   CFan6Speeds::CFan6Speeds(const std::string& keywordName)
      : CSingleHistorizableData<EFan6Speeds>(keywordName,
                                             Fan6SpeedsCapacity(),
                                             yApi::EKeywordAccessMode::kGetSet)
   {
   }
} // namespace specificHistorizers
