#include "stdafx.h"
#include "Byte.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            static const std::string& ByteUnit()
            {
               static const std::string value("B");
               return value;
            }
            DECLARE_CAPACITY(ByteCapacity, "Byte", ByteUnit(), EKeywordDataType::kNumeric);

            CByte::CByte(const std::string& keywordName,
                         const EKeywordAccessMode& accessMode,
                         const EMeasureType& measureType,
                         const typeInfo::CIntTypeInfo& additionalInfo)
               : CSingleHistorizableData<long>(keywordName,
                                               ByteCapacity(),
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


