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
            ///\brief A rssi (signal strength) historizable object
            ///\note Rssi value is read-only
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CRssi : public CSingleHistorizableData<double>
            {
            public:
               explicit CRssi(const std::string& keywordName,
                              const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                              const EMeasureType& measureType = EMeasureType::kAbsolute,
                              typeInfo::CIntTypeInfo& additionalInfo = typeInfo::CIntTypeInfo::Empty);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CRssi();

            protected:
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


