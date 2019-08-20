#include "Humidity.h"
#include "../StandardCapacities.h"
#include <shared/Log.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CHumidity::CHumidity(const std::string& keywordName,
                                 const EKeywordAccessMode& accessMode,
                                 const EMeasureType& measureType,
                                 const typeInfo::CDoubleTypeInfo& additionalInfo,
                                 const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Humidity(),
                                                 accessMode,
                                                 0,
                                                 measureType,
                                                 additionalInfo,
                                                 historyDepth)
            {
            }


            double CHumidity::Normalize(double value)
            {
               if (value > 100)
                  return 100.0;
               if (value < 0)
                  return 0.0;
               return value;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
