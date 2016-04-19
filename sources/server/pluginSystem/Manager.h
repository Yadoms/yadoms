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
#include "dataAccessLayer/IEventLogger.h"
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/IDeviceCommand.h>
#include <shared/plugin/yPluginApi/IExtraCommand.h>
#include <shared/plugin/yPluginApi/historization/PluginState.h>

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	this class is used to manage plugin. 
   ///         It search for plugins into directories and generate plugin factories
   //--------------------------------------------------------------
   class CManager //TODO faire le ménage dans les méthodes et membres
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Type of instance stop event
      //--------------------------------------------------------------
      enum EInstanceStopEventType
      {
         kNormal = 0,      // Instance abnormal stopped
         kAbnormal
      };


      //--------------------------------------------------------------
      /// \brief			Constructor (protected, use newManager to create instance)
      /// \param [in]   initialDir              Initial plugins search directory
      /// \param [in]   dataProvider            Database link
      /// \param [in]   dataAccessLayer         The database access layer
      /// \param [in]   supervisor              The supervisor event handler
      /// \param[in]    eventLogger             Main event logger
      //--------------------------------------------------------------
      CManager(
         const std::string& initialDir,
         boost::shared_ptr<database::IDataProvider> dataProvider,
         boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
         boost::shared_ptr<shared::event::CEventHandler> supervisor,
         boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger);

      //--------------------------------------------------------------
      /// \brief			Initialization, used for the 2-steps construction
      //--------------------------------------------------------------
      void start();

      //--------------------------------------------------------------
      /// \brief			Destructor
      //--------------------------------------------------------------
      virtual ~CManager();

      //--------------------------------------------------------------
      /// \brief			Stop all plugin instance
      //--------------------------------------------------------------
      void stop();

      //--------------------------------------------------------------
      /// \brief           Read the available plugin list
      /// \return          The available plugin map (with informations)
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
      boost::shared_ptr<IInstance> CManager::getRunningInstance(int id) const;

      //--------------------------------------------------------------
      /// \brief           Get the instance configuration
      /// \param [in] id   Instance Id
      /// \param [in] newConfiguration   The instance new configuration
      /// \throw           CNotSupported if request to apply unsupported modifications
      /// \throw           CException if fails
      //--------------------------------------------------------------
      void updateInstance(const database::entities::CPlugin& newData);

      //--------------------------------------------------------------
      /// \brief           Start a registered instance of plugin
      /// \param [in] id   Instance Id
      /// \throw           CInvalidParameter if id is unknown
      /// \note            Just start instance, doesn't modify data in base
      //--------------------------------------------------------------
      void startInstance(int id);

      //--------------------------------------------------------------
      /// \brief           Stop a running instance of plugin
      /// \param [in] id   Instance Id
      /// \throw           CInvalidParameter if id is unknown
      //--------------------------------------------------------------
      void requestStopInstance(int id);

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
      shared::CDataContainer getInstanceFullState(int id) const;


      //--------------------------------------------------------------
      /// \brief        Update the plugin list if needed (after plugin installation for example)
      //--------------------------------------------------------------
      void updatePluginList();

      //--------------------------------------------------------------
      /// \brief                 Post a command to a device on a specific plugin
      /// \param [in] id         Plugin instance Id
      /// \param [in] command    The command to post
      //--------------------------------------------------------------
      void postCommand(int id, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command);

      //--------------------------------------------------------------
      /// \brief                 Post an extra command to a device on a specific plugin
      /// \param [in] id         Plugin instance Id
      /// \param [in] command    The command to post
      //--------------------------------------------------------------
      void postExtraCommand(int id, boost::shared_ptr<const shared::plugin::yPluginApi::IExtraCommand> command);

      //--------------------------------------------------------------
      /// \brief                 Post a manually device creation request to a plugin
      /// \param [in] id         Plugin instance Id
      /// \param [in] request    Request data
      //--------------------------------------------------------------
      void postManuallyDeviceCreationRequest(int id, boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest>& request);

      //--------------------------------------------------------------
      /// \brief                 Post a binding query request to a plugin
      /// \param [in] id         Plugin instance Id
      /// \param [in] request    Request data
      //--------------------------------------------------------------
      void postBindingQueryRequest(int id, boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest>& request);

      //--------------------------------------------------------------
      /// \brief                 Start all instances matching the plugin name
      /// \param [in] pluginName The plugin name
      //--------------------------------------------------------------
      void startAllInstancesOfPlugin(const std::string& pluginName);

      //--------------------------------------------------------------
      /// \brief                 Stop all instances matching the plugin name
      /// \param [in] pluginName The plugin name
      //--------------------------------------------------------------
      void stopAllInstancesOfPlugin(const std::string& pluginName);

      //-----------------------------------------------------
      ///\brief               Method of the thread managing plugin asynchronous events
      //-----------------------------------------------------
      void pluginEventsThreadDoWork();

   private:

      //-----------------------------------------------------
      ///\brief               Start all instances
      //-----------------------------------------------------
      void startAllInstances();

      //-----------------------------------------------------
      ///\brief               Start all specified instances
      ///\param[in] instances Instances to start
      ///\return              true if all instances were successfully started
      //-----------------------------------------------------
      bool startInstances(const std::vector<boost::shared_ptr<database::entities::CPlugin>>& instances);

      //-----------------------------------------------------
      ///\brief               Stop all started instances
      //-----------------------------------------------------
      void stopInstances();

      //-----------------------------------------------------
      ///\brief               Stop the instance and wait for stopped
      ///\param[in] id        The instance ID
      ///\throw               CPluginException if timeout
      //-----------------------------------------------------
      void stopInstanceAndWaitForStopped(int id);

      //-----------------------------------------------------
      ///\brief               Called when instance is stopped
      ///\param[in] id        The instance ID
      ///\param[in] error     Error associated to event (empty if not error)
      //-----------------------------------------------------
      void onInstanceStopped(int id, const std::string& error = std::string());

      //-----------------------------------------------------
      ///\brief               Record instance started in base
      ///\param[in] id        The instance ID
      //-----------------------------------------------------
      void recordInstanceStarted(int id);

      //-----------------------------------------------------
      ///\brief               Record instance stopped in base
      ///\param[in] id        The instance ID
      ///\param[in] error     Error associated to event (empty if not error)
      //-----------------------------------------------------
      void recordInstanceStopped(int id, const std::string& error = std::string());

      void startInternalPlugin();
      void stopInternalPlugin();

   private:
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

      //-----------------------------------------------------
      ///\brief               Flag indicating that Yadoms is being shutdown, so don't record rules stop in database
      //-----------------------------------------------------
      bool m_yadomsShutdown;

      //-----------------------------------------------------
      ///\brief               Event handler to manage events on all plugins
      //-----------------------------------------------------
      boost::shared_ptr<shared::event::CEventHandler> m_pluginManagerEventHandler;

      //-----------------------------------------------------
      ///\brief               Thread managing plugin asynchronous events
      //-----------------------------------------------------
      boost::shared_ptr<boost::thread> m_pluginEventsThread;

      //--------------------------------------------------------------
      /// \brief			Map of all running instances, and its mutex (key are plugin instance id)
      //--------------------------------------------------------------
      typedef std::map<int, boost::shared_ptr<IInstance>> PluginInstanceMap;
      PluginInstanceMap m_runningInstances;
      mutable boost::recursive_mutex m_runningInstancesMutex;

      //-----------------------------------------------------
      ///\brief               The handlers to notify when an instance stop (potentially several handlers for one instance)
      //-----------------------------------------------------
      mutable boost::recursive_mutex m_instanceStopNotifiersMutex;
      std::map<int, std::set<boost::shared_ptr<shared::event::CEventHandler>>> m_instanceStopNotifiers;
   };
} // namespace pluginSystem
