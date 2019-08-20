#include "stdafx.h"
#include "Tamper.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CTamper::CTamper(const std::string& keywordName,
                             const EKeywordAccessMode& accessMode,
                             const typeInfo::CBoolTypeInfo& additionalInfo,
                             const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<bool>(keywordName,
                                               CStandardCapacities::Tamper(),
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
