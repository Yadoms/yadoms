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
                                       const EKeywordAccessMode& accessMode)
               : CSingleHistorizableData<EArmingAlarmStatus>(keywordName,
                                                             CStandardCapacities::ArmingAlarm(),
                                                             accessMode,
                                                             EArmingAlarmStatus::kDisarmed,
                                                             EMeasureType::kAbsolute)
            {
            }

            CArmingAlarm::~CArmingAlarm()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


