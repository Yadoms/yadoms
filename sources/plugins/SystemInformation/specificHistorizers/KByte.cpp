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
            const std::string& KByteUnit("K");
            DECLARE_CAPACITY(KByteCapacity, "KByte", KByteUnit, EKeywordDataType::kNumeric);

            CKByte::CKByte(const std::string& keywordName,
                           const EKeywordAccessMode& accessMode,
                           const EMeasureType& measureType,
                           typeInfo::CIntTypeInfo& additionalInfo)
               : CSingleHistorizableData<long>(keywordName,
                                               KByteCapacity(),
                                               accessMode,
                                               0,
                                               measureType,
                                               additionalInfo)
            {
            }

            CKByte::~CKByte()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


