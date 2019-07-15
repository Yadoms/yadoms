#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A dataTime historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CDateTime : public CSingleHistorizableData<boost::posix_time::ptime>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     To be used as increment counter (values will be added to current database value) or totalizer
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CDateTime(const std::string& keywordName,
                                  const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                  const EMeasureType& measureType = EMeasureType::kCumulative,
                                  const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CDateTime() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
