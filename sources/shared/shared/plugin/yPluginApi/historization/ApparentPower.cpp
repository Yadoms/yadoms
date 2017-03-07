#include "stdafx.h"
#include "ApparentPower.h"
#include "../StandardCapacities.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CApparentPower::CApparentPower(const std::string& keywordName,
                                           const EKeywordAccessMode& accessMode,
                                           const EMeasureType& measureType,
                                           typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::ApparentPower(),
                                                 accessMode,
                                                 0.0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CApparentPower::~CApparentPower()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


