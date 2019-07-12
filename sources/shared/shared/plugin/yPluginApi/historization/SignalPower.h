#pragma once
#include <shared/Export.h>
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
            ///\brief A signal power (unit : %) historizable object
            ///\note signal power value is read-only
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CSignalPower : public CSingleHistorizableData<int>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] measureType     The measure type (normally kAbsolute)
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CSignalPower(const std::string& keywordName,
                                     const EMeasureType& measureType = EMeasureType::kAbsolute,
                                     const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CSignalPower() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
