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
            ///\brief A direction historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CDirection : public CSingleHistorizableData<int>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               ///\param[in] additionalInfo  Additional information
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CDirection(const std::string& keywordName,
                                   const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                   const EMeasureType& measureType = EMeasureType::kAbsolute,
                                   typeInfo::CIntTypeInfo& additionalInfo = typeInfo::CIntTypeInfo::Empty,
                                   const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CDirection() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
