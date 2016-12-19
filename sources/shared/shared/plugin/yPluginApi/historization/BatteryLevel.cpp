#include "stdafx.h"
#include "BatteryLevel.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CBatteryLevel::CBatteryLevel(const std::string& keywordName,
                                         const EMeasureType& measureType,
                                         typeInfo::CIntTypeInfo& additionalInfo)
               : CSingleHistorizableData<int>(keywordName,
                                              CStandardCapacities::BatteryLevel(),
                                              EKeywordAccessMode::kGet,
                                              0,
                                              measureType,
                                              additionalInfo)
            {
            }

            CBatteryLevel::~CBatteryLevel()
            {
            }

            int CBatteryLevel::Normalize(int value)
            {
               if (value > 100)
                  return 100;
               if (value < 0)
                  return 0;
               return value;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


