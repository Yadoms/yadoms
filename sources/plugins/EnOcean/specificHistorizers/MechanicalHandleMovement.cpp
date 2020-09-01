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
                    yApi::CStandardUnits::NoUnit(),
                    yApi::EKeywordDataType::kEnum);

   CMechanicalHandleMovement::CMechanicalHandleMovement(const std::string& keywordName)
      : CSingleHistorizableData<EMechanicalHandleMovement>(keywordName,
                                                           MechanicalHandleMovementCapacity(),
                                                           yApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers
