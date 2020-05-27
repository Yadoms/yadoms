#include "stdafx.h"
#include "RainRate.h"
#include "../StandardCapacities.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CRainRate::CRainRate(const std::string& keywordName,
                                 const EKeywordAccessMode& accessMode,
                                 const EMeasureType& measureType,
                                 const typeInfo::CDoubleTypeInfo& additionalInfo,
                                 const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::RainRate(),
                                                 accessMode,
                                                 0.0,
                                                 measureType,
                                                 additionalInfo,
                                                 historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
