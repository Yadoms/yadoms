#include "stdafx.h"
#include "UpDownStop.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            DECLARE_ENUM_IMPLEMENTATION(EUpDownStopCommand,
               ((Stop))
               ((Up))
               ((Down))
            );

            CUpDownStop::CUpDownStop(const std::string& keywordName,
                                     const EKeywordAccessMode& accessMode,
                                     const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<EUpDownStopCommand>(keywordName,
                                                             CStandardCapacities::UpDownStop(),
                                                             accessMode,
                                                             EUpDownStopCommand::kStop,
                                                             EMeasureType::kAbsolute,
                                                             typeInfo::CEmptyTypeInfo::Empty,
                                                             historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
