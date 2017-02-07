#include "stdafx.h"
#include "Alarm.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CAlarm::CAlarm(const std::string& keywordName, const EKeywordAccessMode& accessMode, typeInfo::CBoolTypeInfo &ti)
               : CSingleHistorizableData<bool>(keywordName, CStandardCapacities::Alarm, accessMode, false, EMeasureType::kAbsolute, ti)
            {
            }

            CAlarm::~CAlarm()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


