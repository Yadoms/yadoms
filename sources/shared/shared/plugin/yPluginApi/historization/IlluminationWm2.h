#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
#include "typeInfo/DoubleTypeInfo.h"

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
            class YADOMS_SHARED_EXPORT CIlluminationWm2 : public CSingleHistorizableData<double>
            {
            public:
               explicit CIlluminationWm2(const std::string& keywordName,
                                         const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                         const EMeasureType& measureType = EMeasureType::kAbsolute,
                                         typeInfo::CDoubleTypeInfo& additionalInfo = typeInfo::CDoubleTypeInfo::Empty);

               virtual ~CIlluminationWm2() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
