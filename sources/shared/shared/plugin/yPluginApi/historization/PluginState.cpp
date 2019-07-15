#include "stdafx.h"
#include "PluginState.h"
#include "../StandardCapacity.h"
#include "../StandardUnits.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            DECLARE_ENUM_IMPLEMENTATION(EPluginState,
               ((Unknown))
               ((Error))
               ((Stopped))
               ((Running))
               ((Custom))
               ((WaitDebugger))
            );

            DECLARE_CAPACITY(PluginStateCapacity, "pluginState_capacity", CStandardUnits::NoUnit(), EKeywordDataType::kEnum);

            CPluginState::CPluginState(const std::string& keywordName,
                                       const EKeywordAccessMode& accessMode)
               : CSingleHistorizableData<EPluginState>(keywordName,
                                                       PluginStateCapacity(),
                                                       accessMode)
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
