#include "stdafx.h"
#include "Rssi.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CRssi::CRssi(const std::string& keywordName,
                         const EKeywordAccessMode& accessMode,
                         const EMeasureType& measureType,
                         const typeInfo::CIntTypeInfo& additionalInfo,
                         const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Rssi(),
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
