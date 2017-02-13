#include "stdafx.h"
#include "SignalPower.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            
            CSignalPower::CSignalPower(const std::string& keywordName, const EMeasureType& measureType, typeInfo::CIntTypeInfo& additionalInfo)
               : CSingleHistorizableData<int>(  keywordName, CStandardCapacities::SignalPower(), EKeywordAccessMode::kGet, 0, measureType, additionalInfo)
            {
            }

            CSignalPower::~CSignalPower()
            {
            }

         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization



