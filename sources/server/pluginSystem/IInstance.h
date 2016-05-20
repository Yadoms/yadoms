#pragma once
#include <server/database/sqlite/requesters/Plugin.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IDeviceCommand.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/IExtraCommand.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief A plugin instance
   //-----------------------------------------------------
   class IInstance
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IInstance() {}

      //-----------------------------------------------------
      ///\brief               Request to stop instance
      //-----------------------------------------------------
      virtual void requestStop() = 0;

      //-----------------------------------------------------
      ///\brief               Kill the instance
      //-----------------------------------------------------
      virtual void kill() = 0;

      //--------------------------------------------------------------
      /// \brief			            Notify the plugin about its configuration changed
      /// \param  newConfiguration  The new configuration to apply
      //--------------------------------------------------------------
      virtual void updateConfiguration(const shared::CDataContainer & newConfiguration) = 0;

      //-----------------------------------------------------
      ///\brief               Get information about this instance
      ///\return              Instance information
      //-----------------------------------------------------
      virtual boost::shared_ptr<const database::entities::CPlugin> about() const = 0;

      //-----------------------------------------------------
      ///\brief               Get information about the plugin associated with this instance
      ///\return              Plugin information
      //-----------------------------------------------------
      virtual boost::shared_ptr<const shared::plugin::information::IInformation> aboutPlugin() const = 0;

      //--------------------------------------------------------------
      /// \brief			            Post a device command to the plugin
      /// \param  information       Device command
      //--------------------------------------------------------------
      virtual void postDeviceCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> deviceCommand) = 0;

      //--------------------------------------------------------------
      /// \brief                 Post a custom query request to a plugin
      /// \param [in] request    Request data
      //--------------------------------------------------------------
      virtual void postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request) = 0;

      //--------------------------------------------------------------
      /// \brief			            Post a device command to the plugin
      /// \param  information       Device command
      //--------------------------------------------------------------
      virtual void postExtraCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IExtraCommand> extraCommand) = 0;

      //--------------------------------------------------------------
      /// \brief                 Post a manually device creation request to plugin
      /// \param [in] request    Request data
      //--------------------------------------------------------------
      virtual void postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request) = 0;
   };
	
} // namespace pluginSystem	
	
	