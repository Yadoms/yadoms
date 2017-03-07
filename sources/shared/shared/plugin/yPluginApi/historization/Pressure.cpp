#include "stdafx.h"
#include "Pressure.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CPressure::CPressure(const std::string& keywordName,
                                 const EKeywordAccessMode& accessMode,
                                 const EMeasureType& measureType,
                                 typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Pressure(),
                                                 accessMode,
                                                 0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CPressure::~CPressure()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


