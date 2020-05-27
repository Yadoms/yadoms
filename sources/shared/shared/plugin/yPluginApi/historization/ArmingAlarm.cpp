#include "stdafx.h"
#include "ArmingAlarm.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            DECLARE_ENUM_IMPLEMENTATION(EArmingAlarmStatus,
               ((Disarmed))
               ((ArmedAtHome))
               ((ArmedAway))
            );

            CArmingAlarm::CArmingAlarm(const std::string& keywordName,
                                       const EKeywordAccessMode& accessMode,
                                       const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<EArmingAlarmStatus>(keywordName,
                                                             CStandardCapacities::ArmingAlarm(),
                                                             accessMode,
                                                             EArmingAlarmStatus::kDisarmed,
                                                             EMeasureType::kAbsolute,
                                                             typeInfo::CEmptyTypeInfo::Empty,
                                                             historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
