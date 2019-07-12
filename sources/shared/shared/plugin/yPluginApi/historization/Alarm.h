#pragma once
#include <shared/Export.h>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include "../typeInfo/BoolTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A Alarm historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CAlarm : public CSingleHistorizableData<bool>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] additionalInfo  The type information
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CAlarm(const std::string& keywordName,
                               const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGetSet,
                               const typeInfo::CBoolTypeInfo& additionalInfo = typeInfo::CBoolTypeInfo::Empty,
                               const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CAlarm() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
