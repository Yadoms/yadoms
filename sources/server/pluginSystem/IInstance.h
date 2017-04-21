#pragma once
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IDeviceCommand.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/IExtraQuery.h>
#include <shared/plugin/yPluginApi/IDeviceConfigurationSchemaRequest.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>
#include <server/database/entities/Entities.h>
#include <shared/plugin/yPluginApi/ISetDeviceConfiguration.h>

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
      /// \param [in] request    Request
      //--------------------------------------------------------------
      virtual void postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request) = 0;

      //--------------------------------------------------------------
      /// \brief                 Post a device configuration schema request to a plugin
      /// \param [in] request    Request
      //--------------------------------------------------------------
      virtual void postDeviceConfigurationSchemaRequest(boost::shared_ptr<shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest> request) = 0;

      //--------------------------------------------------------------
      /// \brief			            Post a device command extra query to instance
      /// \param  extraQuery        The extra query
      /// \param  taskId            The taskId which have posted the query
      //--------------------------------------------------------------
      virtual void postExtraQuery(boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> extraQuery, const std::string & taskId) = 0;

      //--------------------------------------------------------------
      /// \brief			            Set the configuration of a device
      /// \param  command           The set device configuration command
      //--------------------------------------------------------------
      virtual void postSetDeviceConfiguration(boost::shared_ptr<const shared::plugin::yPluginApi::ISetDeviceConfiguration> command) = 0;

      //--------------------------------------------------------------
      /// \brief                 Post a manually device creation request to plugin
      /// \param [in] request    Request data
      //--------------------------------------------------------------
      virtual void postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request) = 0;

      //--------------------------------------------------------------
      /// \brief			            Post a device removed notification to the plugin
      /// \param  event             Device removed notification
      //--------------------------------------------------------------
      virtual void postDeviceRemoved(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceRemoved> event) = 0;
   };
	
} // namespace pluginSystem	
	
	