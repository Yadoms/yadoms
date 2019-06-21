#include "stdafx.h"
#include "SignalPower.h"
#include "../StandardCapacities.h"
#include "typeInfo/PercentageTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            static const typeInfo::CPercentageTypeInfo PercentageTypeInfo;

            CSignalPower::CSignalPower(const std::string& keywordName,
                                       const EMeasureType& measureType,
                                       const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<int>(keywordName,
                                              CStandardCapacities::SignalPower(),
                                              EKeywordAccessMode::kGet,
                                              0,
                                              measureType,
                                              PercentageTypeInfo,
                                              historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
