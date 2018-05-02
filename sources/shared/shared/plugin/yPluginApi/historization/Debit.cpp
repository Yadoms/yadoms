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
                           typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Debit(),
                                                 accessMode,
                                                 0.0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CDebit::~CDebit()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


