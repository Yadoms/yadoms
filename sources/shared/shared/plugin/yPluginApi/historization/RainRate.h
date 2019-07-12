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
            ///\brief A rain historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CRainRate : public CSingleHistorizableData<double>
            {
            public:
               explicit CRainRate(const std::string& keywordName,
                                  const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                  const EMeasureType& measureType = EMeasureType::kCumulative,
                                  const typeInfo::CDoubleTypeInfo& additionalInfo = typeInfo::CDoubleTypeInfo::Empty,
                                  const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CRainRate() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
