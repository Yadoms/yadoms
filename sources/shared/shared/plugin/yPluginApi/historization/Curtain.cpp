#include "stdafx.h"
#include "Curtain.h"
#include "../StandardValues.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            DECLARE_ENUM_IMPLEMENTATION(ECurtainCommand,
               ((Stop))
               ((Open))
               ((Close))
            );

            CCurtain::CCurtain(const std::string& keywordName,
                               const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<ECurtainCommand>(keywordName,
                                                          CStandardCapacities::Curtain(),
                                                          EKeywordAccessMode::kGetSet,
                                                          ECurtainCommand::kStop,
                                                          EMeasureType::kAbsolute,
                                                          typeInfo::CEmptyTypeInfo::Empty,
                                                          historyDepth)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
