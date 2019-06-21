#include "stdafx.h"
#include "Text.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CText::CText(const std::string& keywordName,
                         const EKeywordAccessMode& accessMode,
                         const EMeasureType& measureType,
                         const typeInfo::CStringTypeInfo& additionalInfo,
                         const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<std::string>(keywordName,
                                                      CStandardCapacities::Text(),
                                                      accessMode,
                                                      std::string(),
                                                      measureType,
                                                      additionalInfo,
                                                      historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
