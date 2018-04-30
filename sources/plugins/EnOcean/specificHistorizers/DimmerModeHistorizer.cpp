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
      shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
      shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CDimmerModeHistorizer::CDimmerModeHistorizer(const std::string& keywordName)
      : CSingleHistorizableData<EDimmerMode>(keywordName,
                                             DimmerModeCapacity(),
                                             shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CDimmerModeHistorizer::~CDimmerModeHistorizer()
   {
   }
} // namespace specificHistorizers


