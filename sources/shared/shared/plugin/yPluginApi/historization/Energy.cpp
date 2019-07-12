#include "stdafx.h"
#include "Energy.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CEnergy::CEnergy(const std::string& keywordName,
                             const EKeywordAccessMode& accessMode,
                             const EMeasureType& measureType,
                             const typeInfo::CDoubleTypeInfo& additionalInfo,
                             const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<Poco::Int64>(keywordName,
                                                      CStandardCapacities::Energy(),
                                                      accessMode,
                                                      0,
                                                      measureType,
                                                      additionalInfo,
                                                      historyDepth)
            {
            }

            CEnergyDouble::CEnergyDouble(const std::string& keywordName,
                                         const EKeywordAccessMode& accessMode,
                                         const EMeasureType& measureType,
                                         const typeInfo::CDoubleTypeInfo& additionalInfo,
                                         const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Energy(),
                                                 accessMode,
                                                 0,
                                                 measureType,
                                                 additionalInfo,
                                                 historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
