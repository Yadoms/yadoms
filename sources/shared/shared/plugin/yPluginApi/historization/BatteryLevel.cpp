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
            typeInfo::CPercentageTypeInfo CBatteryLevel::PercentageTypeInfo;

            CBatteryLevel::CBatteryLevel(const std::string& keywordName,
                                         const EKeywordAccessMode& accessMode,
                                         const EMeasureType& measureType,
                                         const typeInfo::ITypeInfo& additionalInfo,
                                         const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<int>(keywordName,
                                              CStandardCapacities::BatteryLevel(),
                                              accessMode,
                                              0,
                                              measureType,
                                              additionalInfo,
                                              historyDepth)
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
