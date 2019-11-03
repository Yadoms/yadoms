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
            ///\brief A distance (meter) historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CDistance : public CSingleHistorizableData<double>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      Access mode
               ///\param[in] additionalInfo  The type information
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CDistance(const std::string& keywordName,
                                  const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                  const typeInfo::CDoubleTypeInfo& additionalInfo = typeInfo::CDoubleTypeInfo::Empty,
                                  const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CDistance() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
