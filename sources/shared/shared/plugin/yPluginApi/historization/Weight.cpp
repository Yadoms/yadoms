#include "stdafx.h"
#include "Weight.h"
#include "../StandardCapacities.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CWeight::CWeight(const std::string& keywordName,
                             const EKeywordAccessMode& accessMode,
                             const EMeasureType& measureType,
                             typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Weight(),
                                                 accessMode,
                                                 0.0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CWeight::~CWeight()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


