#include "stdafx.h"
#include "BlindLockingMode.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EBlindLockingMode,
                               ((DoNotChangeOrNoLock))
                               ((Blockage))
                               ((Alarm))
                               ((Deblockage))
   );

   DECLARE_CAPACITY(BlindLockingModeCapacity,
                    "BlindLockingMode",
                    shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
                    shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CBlindLockingMode::CBlindLockingMode(const std::string& keywordName)
      : CSingleHistorizableData<EBlindLockingMode>(keywordName,
                                                   BlindLockingModeCapacity(),
                                                   yApi::EKeywordAccessMode::kGetSet)
   {
   }
} // namespace specificHistorizers
