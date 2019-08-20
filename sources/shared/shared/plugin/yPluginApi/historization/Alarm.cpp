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
            CAlarm::CAlarm(const std::string& keywordName,
                           const EKeywordAccessMode& accessMode,
                           const typeInfo::CBoolTypeInfo& additionalInfo,
                           const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<bool>(keywordName,
                                               CStandardCapacities::Alarm(),
                                               accessMode,
                                               false,
                                               EMeasureType::kAbsolute,
                                               additionalInfo,
                                               historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
