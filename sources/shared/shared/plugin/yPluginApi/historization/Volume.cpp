#include "stdafx.h"
#include "Volume.h"
#include "../StandardCapacities.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CVolume::CVolume(const std::string& keywordName,
                             const EKeywordAccessMode& accessMode,
                             const EMeasureType& measureType,
                             const typeInfo::CDoubleTypeInfo& additionalInfo,
                             const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Volume(),
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
