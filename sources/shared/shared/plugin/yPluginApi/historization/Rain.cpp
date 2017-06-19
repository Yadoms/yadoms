#include "stdafx.h"
#include "Rain.h"
#include "../StandardCapacities.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CRain::CRain(const std::string& keywordName,
                         const EKeywordAccessMode& accessMode,
                         const EMeasureType& measureType,
                         typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Rain(),
                                                 accessMode,
                                                 0.0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CRain::~CRain()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


