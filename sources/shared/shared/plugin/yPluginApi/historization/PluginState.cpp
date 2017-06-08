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
            );

            DECLARE_CAPACITY(PluginStateCapacity, "pluginState_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

            CPluginState::CPluginState(const std::string& keywordName,
                                       const EKeywordAccessMode& accessMode)
               : CSingleHistorizableData<EPluginState>(keywordName,
                                                       PluginStateCapacity(),
                                                       accessMode)
            {
            }

            CPluginState::~CPluginState()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


