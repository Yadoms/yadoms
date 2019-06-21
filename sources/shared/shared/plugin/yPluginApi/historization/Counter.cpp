#include "stdafx.h"
#include "Counter.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CCounter::CCounter(const std::string& keywordName,
                               const EKeywordAccessMode& accessMode,
                               const EMeasureType& measureType,
                               const typeInfo::CIntTypeInfo& additionalInfo,
                               const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<Poco::Int64>(keywordName,
                                                      CStandardCapacities::Counter(),
                                                      accessMode,
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
