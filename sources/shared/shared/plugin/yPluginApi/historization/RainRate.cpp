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
                                 const EMeasureType& measureType,
                                 typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::RainRate(),
                                                 EKeywordAccessMode::kGet,
                                                 0.0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CRainRate::~CRainRate()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


