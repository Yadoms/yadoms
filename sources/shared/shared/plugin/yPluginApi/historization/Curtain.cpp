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
                               const EKeywordAccessMode& keywordAccessMode,
                               const EHistoryDepth& historyDepth)
               : CSingleHistorizableData<ECurtainCommand>(keywordName,
                                                          CStandardCapacities::Curtain(),
                                                          keywordAccessMode,
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
