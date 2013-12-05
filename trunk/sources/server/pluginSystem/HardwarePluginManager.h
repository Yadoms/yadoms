//
// HardwarePluginManager.h
//
// yadoms-plugin manager
//
#pragma once

#include "HardwarePluginFactory.h"
#include "HardwarePluginInstance.h"
#include "../database/IHardwareRequester.h"

//--------------------------------------------------------------
/// \brief	this class is used to manage plugin. 
///         It search for plugins into directories and generate plugin factories
//--------------------------------------------------------------
class CHardwarePluginManager
{
public:
   //--------------------------------------------------------------
   /// \brief			Constructor
   /// \param [in]   initialDir: initial plugins search directory
   //--------------------------------------------------------------
   CHardwarePluginManager(const std::string & initialDir, boost::shared_ptr<IHardwareRequester> database);

   //--------------------------------------------------------------
   /// \brief			Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginManager();

   //--------------------------------------------------------------
   /// \brief			Update the plugin list
   /// \note         Scan for new/deleted plugin, and create/free associated objects
   //--------------------------------------------------------------
   void updatePluginList();
   
private:
   //--------------------------------------------------------------
   /// \brief        Start the plugin manager (load plugins and start plugin instances)
   //--------------------------------------------------------------
   void Start();

   //--------------------------------------------------------------
   /// \brief        Stop the plugin manager (stop plugin instances and unload plugins)
   //--------------------------------------------------------------
   void Stop();

   //--------------------------------------------------------------
   /// \brief        Returns all plugin libraries installed
   /// \return       a list of all found plugins filenames
   /// \note         This function just lists plugin files by theyre names.
   ///               It doesn't check if plugin is valid (export expected functions)
   //--------------------------------------------------------------
   std::vector<boost::filesystem::path> findPluginFilenames();

   //--------------------------------------------------------------
   /// \brief			Build the plugin list and load plugins
   //--------------------------------------------------------------
   void loadPlugins();

private:
   //--------------------------------------------------------------
   /// \brief			Map of all found factories (key are plugin file names)
   //--------------------------------------------------------------
   typedef std::map<std::string, boost::shared_ptr<CHardwarePluginFactory> > PluginMap;
   PluginMap m_plugins;

   //--------------------------------------------------------------
   /// \brief			Map of all plugin instances (key are plugin instance names)
   //--------------------------------------------------------------
   typedef std::map<std::string, boost::shared_ptr<CHardwarePluginInstance> > PluginInstanceMap;
   PluginInstanceMap m_pluginInstances;

   //--------------------------------------------------------------
   /// \brief			Database access point
   //--------------------------------------------------------------
   boost::shared_ptr<IHardwareRequester> m_database;

   //--------------------------------------------------------------
   /// \brief			Plugin path
   //--------------------------------------------------------------
   boost::filesystem::path m_pluginPath;
};
