//
// Manager.h
//
// Yadoms-plugin manager
//
#pragma once

#include "Factory.h"
#include "Instance.h"
#include "ManagerEvent.h"
#include "database/IPluginRequester.h"
#include "database/IPluginEventLoggerRequester.h"
#include "database/IEventLoggerRequester.h"
#include "communication/command/DeviceCommand.h"
#include <shared/StringExtension.h>
#include <shared/event/EventHandler.hpp>


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
      /// \brief			Map of all valid plugins (key are plugin file names, values are plugin information)
      //--------------------------------------------------------------
      typedef std::map<std::string, boost::shared_ptr<const shared::plugin::information::IInformation> > AvalaiblePluginMap;

   public:
      //--------------------------------------------------------------
      /// \brief			Constructor (protected, use newManager to create instance)
      /// \param [in]   initialDir initial plugins search directory
      /// \param [in]   pluginDBTable           Database link to plugin
      /// \param [in]   pluginLoggerDBTable     Database link to plugin events
      /// \param [in]   mainLoggerDBTable       Database link to main events logger
      /// \param [in]   deviceDBTable           Database link to devices
      /// \param [in]   supervisor     the supervisor event handler
      /// \param [in]   pluginManagerEventId    The ID to use to send events to supervisor
      //--------------------------------------------------------------
      CManager(
         const std::string & initialDir,
         boost::shared_ptr<database::IPluginRequester> pluginDBTable,
         boost::shared_ptr<database::IPluginEventLoggerRequester> pluginLoggerDBTable,
         boost::shared_ptr<database::IEventLoggerRequester> mainLoggerDBTable,
         boost::shared_ptr<database::IDeviceRequester> deviceDBTable,
         shared::event::CEventHandler& supervisor,
         int pluginManagerEventId);

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
      AvalaiblePluginMap getPluginList();

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
      /// \param [in] id   Instance Id
      /// \throw           CInvalidParameter if instance id is unknown fails
      //--------------------------------------------------------------
      void deleteInstance(int id);

      //--------------------------------------------------------------
      /// \brief           Get the plugin instances list
      /// \return          List of instances ID of all known instances, started or not
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<database::entities::CPlugin> > getInstanceList () const;

      //--------------------------------------------------------------
      /// \brief           Get the instance configuration
      /// \param [in] id   Instance Id
      /// \return          The instance configuration, if available (empty string if not)
      /// \throw           CException if fails
      //--------------------------------------------------------------
      boost::shared_ptr<database::entities::CPlugin> getInstance(int id) const;
      
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
      /// \note            Just start instance, doesn't modify data in base
      //--------------------------------------------------------------
      void stopInstance(int id);

      //--------------------------------------------------------------
      /// \brief           Get the running state of a particular instance of plugin
      /// \param [in] id   Instance Id
      /// \return          true if instance is running
      /// \throw           CInvalidParameter if id is unknown
      /// \note            Just start instance, doesn't modify data in base
      //--------------------------------------------------------------
      bool isInstanceRunning(int id) const;

      //--------------------------------------------------------------
      /// \brief           Signal an asynchronous event on plugin manager
      /// \param [in] event   Event data
      //--------------------------------------------------------------
      void signalEvent(const CManagerEvent& event);

      //--------------------------------------------------------------
      /// \brief        Update the plugin list if needed (after plugin installation for example)
      //--------------------------------------------------------------
      void updatePluginList();

      //--------------------------------------------------------------
      /// \brief              Post a command to a device on a specific plugin
      /// \param [in] id      Plugin instance Id
      /// \param [in] message The command to post
      //--------------------------------------------------------------
      void postCommand(int id, const communication::command::CDeviceCommand & message);

   private:
      //--------------------------------------------------------------
      /// \brief        Returns all plugin directories installed
      /// \return       a list of all found plugin directories
      /// \note         This function just lists plugin dorectory names.
      ///               It doesn't check if plugin is valid (export expected functions)
      //--------------------------------------------------------------
      std::vector<boost::filesystem::path> findPluginDirectories();

      //--------------------------------------------------------------
      /// \brief        Build the available plugin map (update m_availablePlugins)
      //--------------------------------------------------------------
      void buildAvailablePluginList();

      //--------------------------------------------------------------
      /// \brief			Load a plugin (do nothing if already loaded)
      /// \param [in] pluginName Plugin name
      /// \return       Loaded plugin
      /// \throw        CInvalidPluginException if plugin is not available
      //--------------------------------------------------------------
      boost::shared_ptr<CFactory> loadPlugin(const std::string& pluginName);

      //--------------------------------------------------------------
      /// \brief			Try to unload a plugin if no more used
      /// \param [in] pluginName Plugin name
      /// \return       true if plugin was unloaded
      //--------------------------------------------------------------
      bool unloadPlugin(const std::string& pluginName);

      //--------------------------------------------------------------
      /// \brief			Get the plugin library path from the plugin name
      /// \param [in] pluginName Plugin name
      /// \return       The full plugin library path
      //--------------------------------------------------------------
      boost::filesystem::path toPath(const std::string& pluginName) const;

   private:
      //--------------------------------------------------------------
      /// \brief			The available plugin map and its mutex
      //--------------------------------------------------------------
      boost::mutex m_availablePluginsMutex;
      AvalaiblePluginMap m_availablePlugins;

      //--------------------------------------------------------------
      /// \brief			Map of all found factories (key are plugin file names)
      //--------------------------------------------------------------
      typedef std::map<std::string, boost::shared_ptr<CFactory> > PluginMap;
      PluginMap m_loadedPlugins;

      //--------------------------------------------------------------
      /// \brief			Map of all running instances (key are plugin instance id)
      //--------------------------------------------------------------
      typedef std::map<int, boost::shared_ptr<CInstance> > PluginInstanceMap;
      PluginInstanceMap m_runningInstances;

      //--------------------------------------------------------------
      /// \brief			Plugins in database
      //--------------------------------------------------------------
      boost::shared_ptr<database::IPluginRequester> m_pluginDBTable;

      //--------------------------------------------------------------
      /// \brief			Main logger in database
      //--------------------------------------------------------------
      boost::shared_ptr<database::IEventLoggerRequester> m_mainLoggerDBTable;

      //--------------------------------------------------------------
      /// \brief			Devices in database
      //--------------------------------------------------------------
      boost::shared_ptr<database::IDeviceRequester> m_deviceDBTable;

      //--------------------------------------------------------------
      /// \brief			Plugin path
      //--------------------------------------------------------------
      boost::filesystem::path m_pluginPath;

      //--------------------------------------------------------------
      /// \brief			Plugin qualifier
      //--------------------------------------------------------------
      const boost::shared_ptr<IQualifier> m_qualifier;

      //--------------------------------------------------------------
      /// \brief			Supervisor event handler
      //--------------------------------------------------------------
      shared::event::CEventHandler& m_supervisor;

      //--------------------------------------------------------------
      /// \brief			ID to use to send events to supervisor
      //--------------------------------------------------------------
      const int m_pluginManagerEventId;
   };

} // namespace pluginSystem
