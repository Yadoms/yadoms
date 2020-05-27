#pragma once
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
            DECLARE_ENUM_HEADER(EArmingAlarmStatus,
                                ((Disarmed))
                                ((ArmedAtHome))
                                ((ArmedAway))
            ) ;

            //-----------------------------------------------------
            ///\brief A arming alarm status historizable object
            //-----------------------------------------------------
            class CArmingAlarm : 
            public
            CSingleHistorizableData<EArmingAlarmStatus>
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
