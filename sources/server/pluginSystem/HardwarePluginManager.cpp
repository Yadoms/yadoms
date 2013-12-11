#include "stdafx.h"
#include "HardwarePluginManager.h"
#include "HardwarePluginInstance.h"


boost::shared_ptr<CHardwarePluginManager> CHardwarePluginManager::newHardwarePluginManager(const std::string & initialDir, boost::shared_ptr<IHardwareRequester> database)
{
   boost::shared_ptr<CHardwarePluginManager> manager (new CHardwarePluginManager(initialDir, database));
   manager->init();
   return manager;
}

CHardwarePluginManager::CHardwarePluginManager(const std::string& initialDir, boost::shared_ptr<IHardwareRequester> database)
   :m_database(database), m_pluginPath(initialDir)
{
   BOOST_ASSERT(m_database);
}

CHardwarePluginManager::~CHardwarePluginManager()
{
   // Stop all instances and free plugins
   while (!m_runningInstances.empty())
   {
      stopInstance(m_runningInstances.begin()->first);
   }
}

void CHardwarePluginManager::init()
{
   // Initialize the plugin list (detect available plugins)
   updatePluginList();

   // Start the directory changes monitor
   m_pluginsDirectoryMonitor.reset(new CDirectoryChangeListener(m_pluginPath, boost::bind(&CHardwarePluginManager::onPluginDirectoryChanges, this, _1)));

   // Create and start plugin instances from database
   std::vector<boost::shared_ptr<CHardware> > databasePluginInstances = m_database->getHardwares();
   BOOST_FOREACH(boost::shared_ptr<CHardware> databasePluginInstance, databasePluginInstances)
   {
      startInstance(databasePluginInstance->getId());
   }
}

void CHardwarePluginManager::startInstance(int id)
{
   if (m_runningInstances.find(id) != m_runningInstances.end())
      return;     // Already started ==> nothing more to do

   // Get instance informations from database
   boost::shared_ptr<CHardware> databasePluginInstance (m_database->getHardware(id));

   // Load the plugin
   try
   {
      boost::shared_ptr<CHardwarePluginFactory> plugin(loadPlugin(databasePluginInstance->getPluginName()));

      // Create instance
      BOOST_ASSERT(plugin); // Plugin not loaded
      boost::shared_ptr<CHardwarePluginInstance> pluginInstance(new CHardwarePluginInstance(plugin, databasePluginInstance));
      m_runningInstances[databasePluginInstance->getId()] = pluginInstance;
   }
   catch (CInvalidPluginException& e)
   {
      YADOMS_LOG(error) << "startInstance : " << e.what();   	
   }
}

void CHardwarePluginManager::stopInstance(int id)
{
   if (m_runningInstances.find(id) == m_runningInstances.end())
      return;     // Already stopped ==> nothing more to do

   // Get the associated plugin name to unload it after instance being deleted
   std::string pluginName = m_runningInstances[id]->getPlugin()->getLibraryPath().stem().string();

   // Remove (=stop) instance
   m_runningInstances.erase(id);

   // Try to unload associated plugin (if no more used)
   unloadPlugin(pluginName);
}

std::vector<boost::filesystem::path> CHardwarePluginManager::findPluginFilenames()
{
   std::vector<boost::filesystem::path> pluginFilenames;
      
   if (boost::filesystem::exists(m_pluginPath) && boost::filesystem::is_directory(m_pluginPath))
   {
      boost::filesystem::directory_iterator endFileIterator;

      static const std::string pluginEndWithString = CDynamicLibrary::DotExtension();

      for(boost::filesystem::directory_iterator fileIterator(m_pluginPath) ; fileIterator != endFileIterator ; ++fileIterator)
      {
         if (boost::filesystem::is_regular_file(fileIterator->status()) &&
            fileIterator->path().extension() == pluginEndWithString)
         {
            pluginFilenames.push_back(fileIterator->path());
         }
      }
   }

   return pluginFilenames;
}

boost::shared_ptr<CHardwarePluginFactory> CHardwarePluginManager::loadPlugin(const std::string& pluginName)
{
   // Check if already loaded
   if (m_loadedPlugins.find(pluginName) != m_loadedPlugins.end())
      return m_loadedPlugins[pluginName];  // Plugin already loaded

   // Check if plugin is available
   if (m_availablePlugins.find(pluginName) == m_availablePlugins.end())
      throw CInvalidPluginException(pluginName);   // Invalid plugin

   // Load the plugin
   boost::shared_ptr<CHardwarePluginFactory> pNewFactory (new CHardwarePluginFactory(toPath(pluginName)));
   m_loadedPlugins[pluginName] = pNewFactory;

   return pNewFactory;
}

bool CHardwarePluginManager::unloadPlugin(const std::string& pluginName)
{
   PluginInstanceMap::const_iterator instance;
   for (instance = m_runningInstances.begin() ; instance != m_runningInstances.end() ; ++instance)
   {
      if ((*instance).second->getPlugin()->getLibraryPath().stem().string() == pluginName)
         break;
   }
   if (instance != m_runningInstances.end())
      return false;  // No unload : plugin is still used by another instance

   // Effectively unload plugin
   m_loadedPlugins.erase(pluginName);
   return true;
}

void CHardwarePluginManager::buildAvailablePluginList()
{
   // Empty the list
   m_availablePlugins.clear();

   // Search for library files
   std::vector<boost::filesystem::path> avalaiblePluginFileNames = findPluginFilenames();

   for (std::vector<boost::filesystem::path>::const_iterator libPathIt = avalaiblePluginFileNames.begin() ;
      libPathIt != avalaiblePluginFileNames.end() ; ++libPathIt)
   {
      try
      {
         // Get informations for current found plugin
         boost::shared_ptr<CHardwarePluginInformation> pluginInformation;
         const std::string& pluginName = (*libPathIt).stem().string();

         // If plugin is already loaded, use its information
         if (m_loadedPlugins.find(pluginName) != m_loadedPlugins.end())
            pluginInformation.reset(new CHardwarePluginInformation(m_loadedPlugins[pluginName]->getInformation()));
         else
            pluginInformation.reset(new CHardwarePluginInformation(CHardwarePluginFactory::getInformation(*libPathIt)));

         m_availablePlugins[pluginName] = pluginInformation;
      }
      catch (CInvalidPluginException& e)
      {
         // Invalid plugin
         YADOMS_LOG(warning) << e.what() << " found in plugin path but is not a valid plugin";
      }
   }
}

void CHardwarePluginManager::updatePluginList()
{
   // Plugin directory have change, so re-build plugin available list
   boost::lock_guard<boost::mutex> lock(m_availablePluginsMutex);
   buildAvailablePluginList();
}


CHardwarePluginManager::AvalaiblePluginMap CHardwarePluginManager::getPluginList()
{
   boost::lock_guard<boost::mutex> lock(m_availablePluginsMutex);
   AvalaiblePluginMap mapCopy = m_availablePlugins;
   return mapCopy;
}

void CHardwarePluginManager::onPluginDirectoryChanges(const boost::asio::dir_monitor_event& ev)
{
   YADOMS_LOG(debug) << "CHardwarePluginManager::onPluginDirectoryChanges" << ev.type;
   updatePluginList();
}

boost::filesystem::path CHardwarePluginManager::toPath(const std::string& pluginName) const
{
   boost::filesystem::path path(m_pluginPath);
   path /= (pluginName + CDynamicLibrary::DotExtension());
   return path;
}
