#pragma once
#include "SingleHistorizableData.hpp"
#include "../typeInfo/StringTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A text historizable object
            //-----------------------------------------------------
            class CText : public CSingleHistorizableData<std::string>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               ///\param[in] additionalInfo  The type information
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CText(const std::string& keywordName,
                              const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                              const EMeasureType& measureType = EMeasureType::kAbsolute,
                              const typeInfo::CStringTypeInfo& additionalInfo = typeInfo::CStringTypeInfo::Empty,
                              const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CText() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
