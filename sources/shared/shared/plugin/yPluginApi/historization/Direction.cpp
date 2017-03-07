#include "stdafx.h"
#include "Direction.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CDirection::CDirection(const std::string& keywordName,
                                   const EKeywordAccessMode& accessMode,
                                   const EMeasureType& measureType,
                                   typeInfo::CIntTypeInfo& additionalInfo)
               : CSingleHistorizableData<int>(keywordName,
                                              CStandardCapacities::Direction(),
                                              accessMode,
                                              0,
                                              measureType,
                                              additionalInfo)
            {
            }

            CDirection::~CDirection()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


