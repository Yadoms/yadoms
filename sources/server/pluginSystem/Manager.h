//
// Manager.h
//
// Yadoms-plugin manager
//
#pragma once
#include "IFactory.h"
#include "IInstance.h"
#include "database/IDataProvider.h"
#include "database/IPluginRequester.h"
#include <shared/event/EventHandler.hpp>
#include "dataAccessLayer/IDataAccessLayer.h"
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/IDeviceCommand.h>
#include <shared/plugin/yPluginApi/IExtraQuery.h>
#include <shared/plugin/yPluginApi/historization/PluginState.h>
#include <IPathProvider.h>
#include "communication/callback/ISynchronousCallback.h"
#include <shared/ILocation.h>
#include "task/Scheduler.h"

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	this class is used to manage plugin. 
   ///         It search for plugins into directories and generate plugin factories
   //--------------------------------------------------------------
   class CManager
   {
   public:
      //--------------------------------------------------------------
      /// \brief			Constructor (protected, use newManager to create instance)
      /// \param [in]   pathProvider            Yadoms paths provider
      /// \param [in]   yadomsVersion           The Yadoms version
      /// \param [in]   dataProvider            Database link
      /// \param [in]   dataAccessLayer         The database access layer
      /// \param [in]   locationProvider        The location provider
      /// \param [in]   taskScheduler           The task scheduler
      //--------------------------------------------------------------
      CManager(boost::shared_ptr<const IPathProvider> pathProvider,
               const shared::versioning::CSemVer& yadomsVersion,
               boost::shared_ptr<database::IDataProvider> dataProvider,
               boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
               boost::shared_ptr<shared::ILocation> locationProvider,
               boost::shared_ptr<task::CScheduler> taskScheduler);

      //--------------------------------------------------------------
      /// \brief			Destructor
      //--------------------------------------------------------------
      virtual ~CManager();

      //--------------------------------------------------------------
      /// \brief			Start the manager (try to start all active plugins)
      /// \param[in]    timeout  Timeout waiting plugins to start
      /// \details      This function is blocking until all plugins are started or timeout
      //--------------------------------------------------------------
      void start(const boost::posix_time::time_duration& timeout);

      //--------------------------------------------------------------
      /// \brief			Stop all plugin instance
      //--------------------------------------------------------------
      void stop();

      //--------------------------------------------------------------
      /// \brief           Read the available plugin list
      /// \return          The available plugin map (with information)
      //--------------------------------------------------------------
      IFactory::AvailablePluginMap getPluginList() const;

      //--------------------------------------------------------------
      /// \brief           Read the plugin quality indicator
      /// \param [in] pluginName  Plugin name
      /// \return          The plugin quality indicator, from 0 (worst) to 100 (best),
      ///                  or IQualifier::kNoEnoughData if no measure available.
      /// \throw           CInvalidPluginException if plugin is not available
      //--------------------------------------------------------------
      int getPluginQualityIndicator(const std::string& pluginName) const;

      //--------------------------------------------------------------
      /// \brief           Create a new instance of a plugin
      /// \param [in] data Data fo the plugin instance to create (instance name, plugin name, configuration, etc...)
      /// \return          Id of the created instance
      /// \throw           CException if fails
      /// \throw           CDatabaseException if duplicate record (instance name must be unique)
      //--------------------------------------------------------------
      int createInstance(const database::entities::CPlugin& data);

      //--------------------------------------------------------------
      /// \brief           Delete a plugin instance
      /// \param [in] id   Instance to delete
      /// \throw           CInvalidParameter if instance id is unknown
      //--------------------------------------------------------------
      void deleteInstance(int id);

      //--------------------------------------------------------------
      /// \brief           Get the log of a plugin instance
      /// \param [in] id   Instance to get the log
      /// \return          The log of the instance, if available (empty string if not)
      //--------------------------------------------------------------
      std::string getInstanceLog(int id) const;

      //--------------------------------------------------------------
      /// \brief           Get the plugin instances list
      /// \return          List of instances ID of all known instances, started or not
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<database::entities::CPlugin>> getInstanceList() const;

      //--------------------------------------------------------------
      /// \brief           Get the instance configuration
      /// \param [in] id   Instance Id
      /// \return          The instance configuration, if available (empty string if not)
      /// \throw           CException if fails
      //--------------------------------------------------------------
      boost::shared_ptr<database::entities::CPlugin> getInstance(int id) const;

      //--------------------------------------------------------------
      /// \brief           Get a running instance
      /// \param [in] id   Instance Id
      /// \return          The corresponding instance
      /// \throw           CPluginException if instance is unknown or not running
      //--------------------------------------------------------------
      boost::shared_ptr<IInstance> getRunningInstance(int id) const;

      //--------------------------------------------------------------
      /// \brief           Update instance data
      /// \param [in] newData   The new data
      //--------------------------------------------------------------
      void updateInstance(const database::entities::CPlugin& newData);

      //--------------------------------------------------------------
      /// \brief           Start a registered instance of plugin
      /// \param [in] id   Instance Id
      /// \throw           CInvalidParameter if id is unknown
      /// \note            Just start instance, does not modify data in base
      //--------------------------------------------------------------
      void startInstance(int id);

      //--------------------------------------------------------------
      /// \brief           Stop a running instance of plugin
      /// \param [in] id   Instance Id
      /// \throw           CInvalidParameter if id is unknown
      //--------------------------------------------------------------
      void requestStopInstance(int id);

      //-----------------------------------------------------
      ///\brief               Stop the instance and wait for stopped
      ///\param[in] id        The instance ID
      ///\throw               CPluginException if timeout
      //-----------------------------------------------------
      void stopInstance(int id);

      //--------------------------------------------------------------
      /// \brief           Kill a running instance of plugin
      /// \param [in] id   Instance Id
      /// \throw           CInvalidParameter if id is unknown
      //--------------------------------------------------------------
      void killInstance(int id);

      //--------------------------------------------------------------
      /// \brief           Get the running state of a particular instance of plugin
      /// \param [in] id   Instance Id
      /// \return          true if instance is running
      /// \throw           CInvalidParameter if id is unknown
      //--------------------------------------------------------------
      bool isInstanceRunning(int id) const;

      //--------------------------------------------------------------
      /// \brief           Get the state of a particular instance of plugin
      /// \param [in] id   Instance Id
      /// \return          The instance state
      /// \throw           CInvalidParameter if id is unknown
      //--------------------------------------------------------------
      shared::plugin::yPluginApi::historization::EPluginState getInstanceState(int id) const;

      //--------------------------------------------------------------
      /// \brief           Get the state of a particular instance of plugin (with message Id if any)
      /// \param [in] id   Instance Id
      /// \return          The instance state data
      /// \throw           CInvalidParameter if id is unknown
      //--------------------------------------------------------------
      boost::shared_ptr<shared::CDataContainer> getInstanceFullState(int id) const;


      //--------------------------------------------------------------
      /// \brief        Update the plugin list if needed (after plugin installation for example)
      //--------------------------------------------------------------
      void updatePluginList();

      //--------------------------------------------------------------
      /// \brief                 Post a command to a device on a specific plugin
      /// \param [in] id         Plugin instance Id
      /// \param [in] command    The command to post
      //--------------------------------------------------------------
      void postCommand(int id,
                       const boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand>& command) const;

      //--------------------------------------------------------------
      /// \brief                 Post an extra command to a device on a specific plugin
      /// \param [in] id         Plugin instance Id
      /// \param [in] query      The query to post
      /// \return                The task id
      //--------------------------------------------------------------
      std::string postExtraQuery(int id, boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> query) const;

      //--------------------------------------------------------------
      /// \brief                 Post a manually device creation request to a plugin
      /// \param [in] id         Plugin instance Id
      /// \param [in] request    Request data
      //--------------------------------------------------------------
      void postManuallyDeviceCreationRequest(int id,
                                             boost::shared_ptr<shared::plugin::yPluginApi::
                                                IManuallyDeviceCreationRequest>& request) const;

      //--------------------------------------------------------------
      /// \brief                 Post a binding query request to a plugin
      /// \param [in] id         Plugin instance Id
      /// \param [in] request    Request data
      //--------------------------------------------------------------
      void postBindingQueryRequest(int id,
                                   boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest>& request) const;

      //--------------------------------------------------------------
      /// \brief                 Post a device configuration schema request to a plugin
      /// \param [in] deviceId   Device Id
      /// \param [in] callback   Request callback
      //--------------------------------------------------------------
      void postDeviceConfigurationSchemaRequest(int deviceId,
                                                communication::callback::ISynchronousCallback<boost::shared_ptr<shared::CDataContainer>>&
                                                callback) const;

      //--------------------------------------------------------------
      /// \brief                       Post a new device configuration to a plugin
      /// \param [in] deviceId         Device Id
      /// \param [in] configuration    New device configuration
      //--------------------------------------------------------------
      void postSetDeviceConfiguration(int deviceId,
                                      const boost::shared_ptr<shared::CDataContainer>& configuration) const;

      //--------------------------------------------------------------
      /// \brief                 Start all instances matching the plugin name
      /// \param [in] pluginName The plugin name
      //--------------------------------------------------------------
      void startAllInstancesOfPlugin(const std::string& pluginName);

      //--------------------------------------------------------------
      /// \brief                 Stop all instances matching the plugin name
      /// \param [in] pluginName The plugin name
      /// \return                The IDs of instances who stop was requested
      //--------------------------------------------------------------
      std::vector<int> stopAllInstancesOfPlugin(const std::string& pluginName);

      //--------------------------------------------------------------
      /// \brief                 Stop all instances matching the plugin name
      /// \param [in] pluginName The plugin name
      /// \param [in] timeout    Timeout waiting plugin stop
      //--------------------------------------------------------------
      void stopAllInstancesOfPluginAndWaitForStopped(const std::string& pluginName,
                                                     const boost::posix_time::time_duration& timeout = boost::posix_time
                                                        ::seconds(30));

      //--------------------------------------------------------------
      /// \brief                 Notify a plugin when a device is removed
      /// \param [in] deviceId   The removed device ID
      //--------------------------------------------------------------
      void notifyDeviceRemoved(int deviceId) const;

   private:
      //-----------------------------------------------------
      ///\brief               Start all instances
      ///\param[out] startedInstanceIds Instances started
      //-----------------------------------------------------
      void startAllInstances(std::set<int>& startedInstanceIds);

      //-----------------------------------------------------
      ///\brief               Start all specified instances
      ///\param[in] instances Instances to start
      ///\param[out] startedInstanceIds Instances started
      ///\return              true if all instances were successfully started
      //-----------------------------------------------------
      bool startInstances(const std::vector<boost::shared_ptr<database::entities::CPlugin>>& instances,
                          std::set<int>& startedInstanceIds);

      //-----------------------------------------------------
      ///\brief               Stop all started instances
      //-----------------------------------------------------
      void stopInstances();

      void onPluginStopped(int pluginInstanceId);

      void startInternalPlugin();


      //--------------------------------------------------------------
      /// \brief			The plugin system factory
      //--------------------------------------------------------------
      boost::shared_ptr<IFactory> m_factory;

      //--------------------------------------------------------------
      /// \brief			The available plugin map
      //--------------------------------------------------------------
      IFactory::AvailablePluginMap m_availablePlugins;
      mutable boost::recursive_mutex m_availablePluginsMutex;

      //--------------------------------------------------------------
      /// \brief			Global database accessor
      //--------------------------------------------------------------
      boost::shared_ptr<database::IDataProvider> m_dataProvider;

      //--------------------------------------------------------------
      /// \brief			Shortcut to plugins in database
      //--------------------------------------------------------------
      boost::shared_ptr<database::IPluginRequester> m_pluginDBTable;

      //--------------------------------------------------------------
      /// \brief			Plugin qualifier
      //--------------------------------------------------------------
      const boost::shared_ptr<IQualifier> m_qualifier;

      //--------------------------------------------------------------
      /// \brief			Supervisor event handler
      //--------------------------------------------------------------
      boost::shared_ptr<shared::event::CEventHandler> m_supervisor;

      //--------------------------------------------------------------
      /// \brief			Data access layer
      //--------------------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IDataAccessLayer> m_dataAccessLayer;

      //--------------------------------------------------------------
      /// \brief			Task scheduler
      //--------------------------------------------------------------
      boost::shared_ptr<task::CScheduler> m_taskScheduler;

      //--------------------------------------------------------------
      /// \brief			Map of all running instances, and its mutex (key are plugin instance id)
      //--------------------------------------------------------------
      std::map<int, boost::shared_ptr<IInstance>> m_runningInstances;
      mutable boost::recursive_mutex m_runningInstancesMutex;
   };
} // namespace pluginSystem
