#pragma once
#include "SingleHistorizableData.hpp"
#include "../typeInfo/IntTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A rssi (dBm) historizable object
            ///\note Rssi value is read-only
            //-----------------------------------------------------
            class CRssi : public CSingleHistorizableData<double>
            {
            public:
               explicit CRssi(const std::string& keywordName,
                              const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                              const EMeasureType& measureType = EMeasureType::kAbsolute,
                              const typeInfo::CIntTypeInfo& additionalInfo = typeInfo::CIntTypeInfo::Empty,
                              const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CRssi() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
