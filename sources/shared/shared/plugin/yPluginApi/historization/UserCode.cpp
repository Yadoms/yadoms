#include "stdafx.h"
#include "UserCode.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CUserCode::CUserCode(const std::string& keywordName,
                     const EKeywordAccessMode& accessMode,
                     const EMeasureType& measureType,
                     typeInfo::CByteArrayTypeInfo& additionalInfo)
               : CSingleHistorizableData<std::string>(keywordName,
                                                 CStandardCapacities::UserCode(),
                                                 accessMode,
                                                 NULL,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CUserCode::~CUserCode()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


