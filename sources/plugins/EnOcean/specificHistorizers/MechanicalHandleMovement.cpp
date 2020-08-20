#include "stdafx.h"
#include "MechanicalHandleMovement.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EMechanicalHandleMovement,
                               ((ToUp))
                               ((ToDown))
                               ((ToHorizontal))
   );

   DECLARE_CAPACITY(MechanicalHandleMovementCapacity,
                    "MechanicalHandleMovement",
                    shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
                    shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CMechanicalHandleMovement::CMechanicalHandleMovement(const std::string& keywordName)
      : CSingleHistorizableData<EMechanicalHandleMovement>(keywordName,
                                                           MechanicalHandleMovementCapacity(),
                                                           shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
