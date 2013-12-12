//
// HardwarePluginManager.h
//
// yadoms-plugin manager
//
#pragma once

#include "HardwarePluginFactory.h"
#include "HardwarePluginInstance.h"
#include "../database/IHardwareRequester.h"

#include "DirectoryChangeListener.h"

//--------------------------------------------------------------
/// \brief	this class is used to manage plugin. 
///         It search for plugins into directories and generate plugin factories
//--------------------------------------------------------------
class CHardwarePluginManager
{
public:
   //--------------------------------------------------------------
   /// \brief			Map of all valid plugins (key are plugin file names, values are plugin information)
   //--------------------------------------------------------------
   typedef std::map<std::string, boost::shared_ptr<CHardwarePluginInformation> > AvalaiblePluginMap;

protected:
   //--------------------------------------------------------------
   /// \brief			Constructor (protected, use newHardwarePluginManager to create instance)
   /// \param [in]   initialDir initial plugins search directory
   /// \param [in]   database database link
   //--------------------------------------------------------------
   CHardwarePluginManager(const std::string & initialDir, boost::shared_ptr<IHardwareRequester> database);

   //--------------------------------------------------------------
   /// \brief			Initialisation, used for the 2-steps construction
   //--------------------------------------------------------------
   void init();

public:
   //--------------------------------------------------------------
   /// \brief			Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginManager();

   //--------------------------------------------------------------
   /// \brief			Create new CHardwarePluginManager instance
   /// \param [in]   initialDir: initial plugins search directory
   /// \param [in]   database: database link
   //--------------------------------------------------------------
   static boost::shared_ptr<CHardwarePluginManager> newHardwarePluginManager(const std::string & initialDir, boost::shared_ptr<IHardwareRequester> database);

   //--------------------------------------------------------------
   /// \brief           Start a registered instance of plugin
   /// \param [in] id   Instance Id
   /// \throw           std::invalid_argument if id is unknown
   /// \note            Do nothing if instance already running
   //--------------------------------------------------------------
   void startInstance(int id);

   //--------------------------------------------------------------
   /// \brief           Stop a running instance of plugin
   /// \param [in] id   Instance Id
   /// \throw           std::invalid_argument if id is unknown
   /// \note            Do nothing if instance already stopped
   //--------------------------------------------------------------
   void stopInstance(int id);

   //--------------------------------------------------------------
   /// \brief           Read the available plugin list
   /// \return          The availabe plugin map (with informations)
   //--------------------------------------------------------------
   AvalaiblePluginMap getPluginList();
   
private:
   //--------------------------------------------------------------
   /// \brief        Returns all plugin libraries installed
   /// \return       a list of all found plugins filenames
   /// \note         This function just lists plugin files by their names.
   ///               It doesn't check if plugin is valid (export expected functions)
   //--------------------------------------------------------------
   std::vector<boost::filesystem::path> findPluginFilenames();

   //--------------------------------------------------------------
   /// \brief        Update the plugin list if needed
   //--------------------------------------------------------------
   void updatePluginList();

   //--------------------------------------------------------------
   /// \brief        Build the available plugin map (update m_availablePlugins)
   //--------------------------------------------------------------
   void buildAvailablePluginList();

   //--------------------------------------------------------------
   /// \brief			Load a plugin (do nothing if already loaded)
   /// \return       Loaded plugin
   /// \throw        CInvalidPluginException if plugin is not available
   //--------------------------------------------------------------
   boost::shared_ptr<CHardwarePluginFactory> loadPlugin(const std::string& pluginName);

   //--------------------------------------------------------------
   /// \brief			Try to unload a plugin if no more used
   /// \return       true if plugin was unloaded
   //--------------------------------------------------------------
   bool unloadPlugin(const std::string& pluginName);

   //--------------------------------------------------------------
   /// \brief			Event when plugin directory changes
   /// \param [in] ev Informations about the event
   //--------------------------------------------------------------
   void onPluginDirectoryChanges(const boost::asio::dir_monitor_event& ev);

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
   typedef std::map<std::string, boost::shared_ptr<CHardwarePluginFactory> > PluginMap;
   PluginMap m_loadedPlugins;

   //--------------------------------------------------------------
   /// \brief			Map of all running instances (key are plugin instance id)
   //--------------------------------------------------------------
   typedef std::map<int, boost::shared_ptr<CHardwarePluginInstance> > PluginInstanceMap;
   PluginInstanceMap m_runningInstances;

   //--------------------------------------------------------------
   /// \brief			Database access point
   //--------------------------------------------------------------
   boost::shared_ptr<IHardwareRequester> m_database;

   //--------------------------------------------------------------
   /// \brief			Plugin path
   //--------------------------------------------------------------
   boost::filesystem::path m_pluginPath;

   //--------------------------------------------------------------
   /// \brief			Plugin directory listener
   //--------------------------------------------------------------
   boost::shared_ptr<CDirectoryChangeListener> m_pluginsDirectoryMonitor;
};
