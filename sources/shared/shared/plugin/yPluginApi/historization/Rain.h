#pragma once
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
            class CRain : public CSingleHistorizableData<double>
            {
            public:
               explicit CRain(const std::string& keywordName,
                              const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                              const EMeasureType& measureType = EMeasureType::kCumulative,
                              const typeInfo::CDoubleTypeInfo& additionalInfo = typeInfo::CDoubleTypeInfo::Empty,
                              const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CRain() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
