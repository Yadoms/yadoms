#include "stdafx.h"
#include "Rssi.h"
#include "../StandardCapacities.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CRssi::CRssi(const std::string& keywordName,
                         const EMeasureType& measureType,
                         typeInfo::CIntTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                              CStandardCapacities::Rssi(),
                                              EKeywordAccessMode::kGet,
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


