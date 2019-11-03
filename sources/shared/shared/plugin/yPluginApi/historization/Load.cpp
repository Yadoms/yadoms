#include "stdafx.h"
#include "Load.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CLoad::CLoad(const std::string& keywordName,
                         const EKeywordAccessMode& accessMode,
                         const typeInfo::CDoubleTypeInfo& additionalInfo,
                         const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Load(),
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
