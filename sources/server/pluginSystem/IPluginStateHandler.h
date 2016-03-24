#pragma once

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The plugins state management interface
   //-----------------------------------------------------
   class IPluginStateHandler
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IPluginStateHandler() {}

      //-----------------------------------------------------
      ///\brief               Signal a normal stop on a plugin instance
      ///\param[in] ruleId    The instance ID
      //-----------------------------------------------------
      virtual void signalNormalStop(int instanceId) = 0;

      //-----------------------------------------------------
      ///\brief               Signal an error on a plugin instance
      ///\param[in] ruleId    The instance ID
      ///\param[in] error     Error message
      //-----------------------------------------------------
      virtual void signalError(int instanceId, const std::string& error) = 0;

      //-----------------------------------------------------
      ///\brief               Signal error on starting plugin instances
      ///\param[in] ruleId    The instance ID
      ///\param[in] error     Error message
      //-----------------------------------------------------
      virtual void signalStartError(int instanceId, const std::string& error) = 0;
   };
	
} // namespace pluginSystem	
	
	