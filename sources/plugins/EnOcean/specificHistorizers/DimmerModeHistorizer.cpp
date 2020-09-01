#include "stdafx.h"
#include "DimmerModeHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EDimmerMode,
                               ((SwitchToValue))
                               ((DimToValueWithTimer1))
                               ((DimToValueWithTimer2))
                               ((DimToValueWithTimer3))
                               ((StopDimming))
   );


   DECLARE_CAPACITY(DimmerModeCapacity,
                    "DimmerMode",
                    yApi::CStandardUnits::NoUnit(),
                    yApi::EKeywordDataType::kEnum);

   CDimmerModeHistorizer::CDimmerModeHistorizer(const std::string& keywordName)
      : CSingleHistorizableData<EDimmerMode>(keywordName,
                                             DimmerModeCapacity(),
                                             yApi::EKeywordAccessMode::kGetSet)
   {
   }
} // namespace specificHistorizers
