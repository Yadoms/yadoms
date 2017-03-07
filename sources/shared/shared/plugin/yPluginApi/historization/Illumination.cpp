#include "stdafx.h"
#include "Illumination.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CIllumination::CIllumination(const std::string& keywordName,
                                         const EKeywordAccessMode& accessMode,
                                         const EMeasureType& measureType,
                                         typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Illumination(),
                                                 accessMode,
                                                 0.0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CIllumination::~CIllumination()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
