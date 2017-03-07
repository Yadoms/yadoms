#include "stdafx.h"
#include "Frequency.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CFrequency::CFrequency(const std::string& keywordName,
                                   const EKeywordAccessMode& accessMode,
                                   const EMeasureType& measureType,
                                   typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Frequency(),
                                                 accessMode,
                                                 0.0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CFrequency::~CFrequency()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


