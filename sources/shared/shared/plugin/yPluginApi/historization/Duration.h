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
            ///\brief A duration (s) historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CDuration : public CSingleHistorizableData<double>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               ///\param[in] additionalInfo  The additional info (min, max, precision...)
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CDuration(const std::string& keywordName,
                                  const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                  const EMeasureType& measureType = EMeasureType::kAbsolute,
                                  typeInfo::CDoubleTypeInfo& additionalInfo = typeInfo::CDoubleTypeInfo::Empty,
                                  const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CDuration() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
