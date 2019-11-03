#include "stdafx.h"
#include "SignalLevel.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CSignalLevel::CSignalLevel(const std::string& keywordName,
                                       const EMeasureType& measureType,
                                       const typeInfo::CIntTypeInfo& additionalInfo,
                                       const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<int>(keywordName,
                                              CStandardCapacities::SignalLevel(),
                                              EKeywordAccessMode::kGet,
                                              0,
                                              measureType,
                                              additionalInfo,
                                              historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
