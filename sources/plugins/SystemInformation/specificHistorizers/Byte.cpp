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
            const std::string& ByteUnit("B");
            DECLARE_CAPACITY(ByteCapacity, "Byte", ByteUnit, EKeywordDataType::kNumeric);

            CByte::CByte(const std::string& keywordName,
                         const EKeywordAccessMode& accessMode,
                         const EMeasureType& measureType,
                         typeInfo::CIntTypeInfo& additionalInfo)
               : CSingleHistorizableData<long>(keywordName,
                                               ByteCapacity(),
                                               accessMode,
                                               0,
                                               measureType,
                                               additionalInfo)
            {
            }

            CByte::~CByte()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


