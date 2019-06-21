#include "stdafx.h"
#include "Distance.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CDistance::CDistance(const std::string& keywordName,
                                 const EKeywordAccessMode& accessMode,
                                 typeInfo::CDoubleTypeInfo& additionalInfo,
                                 const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Distance(),
                                                 accessMode,
                                                 0.0,
                                                 EMeasureType::kAbsolute,
                                                 additionalInfo,
                                                 historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
