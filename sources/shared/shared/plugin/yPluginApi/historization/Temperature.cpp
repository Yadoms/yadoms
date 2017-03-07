#include "stdafx.h"
#include "Temperature.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CTemperature::CTemperature(const std::string& keywordName,
                                       const EKeywordAccessMode& accessMode,
                                       const EMeasureType& measureType,
                                       typeInfo::CDoubleTypeInfo& additionalInfo)
               : CSingleHistorizableData<double>(keywordName,
                                                 CStandardCapacities::Temperature(),
                                                 accessMode,
                                                 0.0,
                                                 measureType,
                                                 additionalInfo)
            {
            }

            CTemperature::~CTemperature()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


