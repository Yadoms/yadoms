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
            ///\brief Plugin state
            //-----------------------------------------------------
            DECLARE_ENUM_HEADER(EPluginState,
                                ((Unknown))
                                ((Error))
                                ((Stopped))
                                ((Running))
                                ((Custom))
                                ((WaitDebugger))
            )


            //-----------------------------------------------------
            ///\brief The plugin state historizable object
            //-----------------------------------------------------
            class CPluginState final : public CSingleHistorizableData<EPluginState>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               //-----------------------------------------------------
               explicit CPluginState(const std::string& keywordName,
                                     const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet);

               CPluginState(const CPluginState&) = delete;
               CPluginState(CPluginState&&) = delete;
               CPluginState& operator=(const CPluginState&) = delete;
               CPluginState& operator=(CPluginState&&) = delete;

               ~CPluginState() override = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
