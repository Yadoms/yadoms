#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
#include "typeInfo/IntTypeInfo.h"

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
               explicit CBatteryLevel(const std::string& keywordName,
                                      const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                      const EMeasureType& measureType = EMeasureType::kAbsolute,
                                      typeInfo::CIntTypeInfo& additionalInfo = typeInfo::CIntTypeInfo::Empty);

               virtual ~CBatteryLevel();

            protected:
               // Normalize the battery level [0; 100]
               int Normalize(int value) override;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


