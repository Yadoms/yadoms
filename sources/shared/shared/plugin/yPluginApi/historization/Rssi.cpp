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
               : CSingleHistorizableData<int>(keywordName,
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


            int CRssi::Normalize(int rssi)
            {
               if (rssi > 100)
                  return 100;
               if (rssi < 0)
                  return 0;
               return rssi;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


