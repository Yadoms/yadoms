#include "stdafx.h"
#include "SpecialHeatingStates.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(ESpecialHeatingStates,
                               ((DefinedBySetPoint))
                               ((FrostProtection))
                               ((AutomaticControl))
   );

   DECLARE_CAPACITY(SpecialHeatingStatesCapacity,
                    "SpecialHeatingStates",
                    yApi::CStandardUnits::NoUnit(),
                    yApi::EKeywordDataType::kEnum);

   CSpecialHeatingStates::CSpecialHeatingStates(const std::string& keywordName)
      : CSingleHistorizableData<ESpecialHeatingStates>(keywordName,
                                                       SpecialHeatingStatesCapacity(),
                                                       yApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
