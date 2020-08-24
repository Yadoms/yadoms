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
                    shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
                    shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CSpecialHeatingStates::CSpecialHeatingStates(const std::string& keywordName)
      : CSingleHistorizableData<ESpecialHeatingStates>(keywordName,
                                                       SpecialHeatingStatesCapacity(),
                                                       shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
