#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
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
            ///\brief               The alarm arming status
            //-----------------------------------------------------   
            DECLARE_ENUM_HEADER_SHARED(EArmingAlarmStatus, YADOMS_SHARED_EXPORT,
               ((Disarmed))
               ((ArmedAtHome))
               ((ArmedAway))
            ) ;

            //-----------------------------------------------------
            ///\brief A arming alarm status historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CArmingAlarm : public CSingleHistorizableData<EArmingAlarmStatus>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CArmingAlarm(const std::string& keywordName,
                                     const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                  const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CArmingAlarm() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


