#include "stdafx.h"
#include "Debit.h"
#include "../StandardCapacities.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CDebit::CDebit(const std::string& keywordName,
                           const EKeywordAccessMode& accessMode,
                           const EMeasureType& measureType,
                           typeInfo::CDoubleTypeInfo& additionalInfo,
                           const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Debit(),
                                                 accessMode,
                                                 0.0,
                                                 measureType,
                                                 additionalInfo,
                                                 historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
