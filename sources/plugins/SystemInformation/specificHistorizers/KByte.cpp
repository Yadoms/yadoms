#include "stdafx.h"
#include "KByte.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            static const std::string& KByteUnit()
            {
               static const std::string value("K");
               return value;
            }
            DECLARE_CAPACITY(KByteCapacity, "KByte", KByteUnit(), EKeywordDataType::kNumeric);

            CKByte::CKByte(const std::string& keywordName,
                           const EKeywordAccessMode& accessMode,
                           const EMeasureType& measureType,
                           const typeInfo::CIntTypeInfo& additionalInfo)
               : CSingleHistorizableData<long>(keywordName,
                                               KByteCapacity(),
                                               accessMode,
                                               0,
                                               measureType,
                                               additionalInfo)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


