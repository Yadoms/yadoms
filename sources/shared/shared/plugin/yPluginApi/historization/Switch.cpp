#include "stdafx.h"
#include "Switch.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CSwitch::CSwitch(const std::string& keywordName,
                             const EKeywordAccessMode& accessMode,
                             const typeInfo::CBoolTypeInfo& additionalInfo,
                             const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<bool>(keywordName,
                                               CStandardCapacities::Switch(),
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
