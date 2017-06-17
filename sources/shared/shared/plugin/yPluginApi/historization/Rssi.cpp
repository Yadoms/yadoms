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
                         typeInfo::CIntTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Rssi(),
                                                 accessMode,
                                                 0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CRssi::~CRssi()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


