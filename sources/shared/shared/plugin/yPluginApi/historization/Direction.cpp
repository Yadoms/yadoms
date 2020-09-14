#include "stdafx.h"
#include "Direction.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CDirection::CDirection(const std::string& keywordName,
                                   const EKeywordAccessMode& accessMode,
                                   const EMeasureType& measureType,
                                   const typeInfo::CIntTypeInfo& additionalInfo,
                                   const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<int>(keywordName,
                                              CStandardCapacities::Direction(),
                                              accessMode,
                                              0,
                                              measureType,
                                              additionalInfo,
                                              historyDepth)
            {
            }

            CDirectionDouble::CDirectionDouble(const std::string& keywordName,
                                               const EKeywordAccessMode& accessMode,
                                               const EMeasureType& measureType,
                                               const typeInfo::CDoubleTypeInfo& additionalInfo,
                                               const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Direction(),
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
