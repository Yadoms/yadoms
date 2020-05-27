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
            ///\brief A Illumination(W/m²) historizable object
            //-----------------------------------------------------
            class CIlluminationWm2 : public CSingleHistorizableData<double>
            {
            public:
               explicit CIlluminationWm2(const std::string& keywordName,
                                         const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                         const EMeasureType& measureType = EMeasureType::kAbsolute,
                                         const typeInfo::CDoubleTypeInfo& additionalInfo = typeInfo::CDoubleTypeInfo::Empty,
                                         const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CIlluminationWm2() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
