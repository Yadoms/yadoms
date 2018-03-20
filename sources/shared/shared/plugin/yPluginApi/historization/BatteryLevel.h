#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
#include "typeInfo/PercentageTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A battery level historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CBatteryLevel : public CSingleHistorizableData<int>
            {
            public:
               static typeInfo::CPercentageTypeInfo PercentageTypeInfo;

               explicit CBatteryLevel(const std::string& keywordName,
                                      const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                      const EMeasureType& measureType = EMeasureType::kAbsolute,
                                      const typeInfo::ITypeInfo& additionalInfo = PercentageTypeInfo);

               virtual ~CBatteryLevel();

            protected:
               // Normalize the battery level [0; 100]
               int Normalize(int value) override;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
