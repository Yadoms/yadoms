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

            DECLARE_CAPACITY(PluginStateCapacity, "pluginState", CStandardUnits::NoUnit(), EKeywordDataType::kEnum); //TODO doublon avec CStandardCapacities::PluginState

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
