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
            ///\brief A signal Level (unit : dB) historizable object
            ///\note signal Level value is read-only
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CSignalLevel : public CSingleHistorizableData<int>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] measureType     The measure type (normally kAbsolute)
               ///\param[in] additionalInfo  The type information
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CSignalLevel(const std::string& keywordName,
                                     const EMeasureType& measureType = EMeasureType::kAbsolute,
                                     const typeInfo::CIntTypeInfo& additionalInfo = typeInfo::CIntTypeInfo::Empty,
                                     const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CSignalLevel() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
