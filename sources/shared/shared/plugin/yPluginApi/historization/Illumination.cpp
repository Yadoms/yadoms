#include "stdafx.h"
#include "Illumination.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CIllumination::CIllumination(const std::string& keywordName,
                                         const EKeywordAccessMode& accessMode,
                                         const EMeasureType& measureType,
                                         typeInfo::CDoubleTypeInfo& additionalInfo,
                                         const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Illumination(),
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
