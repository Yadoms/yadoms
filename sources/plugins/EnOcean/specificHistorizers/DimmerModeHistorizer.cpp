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


   CDimmerModeHistorizer::CDimmerModeHistorizer(const std::string& keywordName)
      : CSingleHistorizableData<EDimmerMode>(keywordName,
                                             shared::plugin::yPluginApi::CStandardCapacity("DimmerMode",
                                                                                           shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
                                                                                           shared::plugin::yPluginApi::EKeywordDataType::kEnum),
                                             shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
   {
   }

   CDimmerModeHistorizer::~CDimmerModeHistorizer()
   {
   }
} // namespace specificHistorizers


