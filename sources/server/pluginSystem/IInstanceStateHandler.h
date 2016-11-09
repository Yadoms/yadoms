#pragma once
#include <shared/plugin/yPluginApi/historization/PluginState.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief A plugin instance state handler
   //-----------------------------------------------------
   class IInstanceStateHandler
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IInstanceStateHandler() {}

      //-----------------------------------------------------
      ///\brief Set the plugin instance state
      ///\param    [in]    state           The new state
      ///\param    [in]    customMessageId The associated message ID (translatable in the locales file of the plugin). Ignored if state != kCustom or kError.
      ///\param    [in]    customMessageData The message data (used as translation parameters). Ignored if state != kCustom or kError.
      //-----------------------------------------------------
      virtual void setState(const shared::plugin::yPluginApi::historization::EPluginState& state,
                            const std::string& customMessageId = std::string(),
                            const std::string& customMessageData = std::string()) = 0;
   };

} // namespace pluginSystem

