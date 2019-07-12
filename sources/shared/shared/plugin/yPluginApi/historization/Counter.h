#pragma once
#include <shared/Export.h>
#include <Poco/Types.h>
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
            ///\brief A counter historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CCounter : public CSingleHistorizableData<Poco::Int64>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     To be used as increment counter (values will be added to current database value) or totalizer
               ///\param[in] additionalInfo  The type information
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CCounter(const std::string& keywordName,
                                 const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                 const EMeasureType& measureType = EMeasureType::kCumulative,
                                 const typeInfo::CIntTypeInfo& additionalInfo = typeInfo::CIntTypeInfo::Empty,
                                 const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CCounter() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
