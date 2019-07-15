#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
#include "../typeInfo/DoubleTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A Illumination(Lux) historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CIllumination : public CSingleHistorizableData<double>
            {
            public:
               explicit CIllumination(const std::string& keywordName,
                                      const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                      const EMeasureType& measureType = EMeasureType::kAbsolute,
                                      const typeInfo::CDoubleTypeInfo& additionalInfo = typeInfo::CDoubleTypeInfo::Empty,
                                      const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CIllumination() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
