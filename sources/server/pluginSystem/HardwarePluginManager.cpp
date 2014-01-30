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
   stop();
}

void CHardwarePluginManager::stop()
{
   YADOMS_LOG(info) << "CHardwarePluginManager stop plugins...";

   //plugins request to stop
   std::pair<int, boost::shared_ptr<CHardwarePluginInstance> > instance;
   BOOST_FOREACH(instance, m_runningInstances)
   {
      if(instance.second.get() != NULL)
         instance.second->requestToStop();
   }

   // wait for all plugins to stop
   while (!m_runningInstances.empty())
   {
      //stopInstance(m_runningInstances.begin()->first);
      std::pair<int, boost::shared_ptr<CHardwarePluginInstance> > instance2;
      BOOST_FOREACH(instance2, m_runningInstances)
      {
         if(instance2.second.get() != NULL && instance2.second->getStatus() == CThreadBase::kStopped)
         {
            doStopInstance(instance2.first);
            break; //break to restart the foreach
         }
      }
   }

   YADOMS_LOG(info) << "CHardwarePluginManager all plugins are stopped";
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
      if (databasePluginInstance->getEnabled())
         doStartInstance(databasePluginInstance->getId());
   }
}

void CHardwarePluginManager::startInstance(int id)
{
   // Start instance (throw if fails)
   doStartInstance(id);

   // Update instance state in database
   m_database->enableInstance(id, true);
}

void CHardwarePluginManager::stopInstance(int id)
{
   // Start instance (throw if fails)
   doStopInstance(id);

   // Update instance state in database
   m_database->enableInstance(id, false);
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
            pluginFilenames.push_back(fileIterator->path().filename());
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
         const std::string& pluginName = CDynamicLibrary::ToLibName((*libPathIt).string());

         // If plugin is already loaded, use its information
         if (m_loadedPlugins.find(pluginName) != m_loadedPlugins.end())
            m_availablePlugins[pluginName] = m_loadedPlugins[pluginName]->getInformation();
         else
            m_availablePlugins[pluginName] = CHardwarePluginFactory::getInformation(toPath(pluginName));
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

std::string CHardwarePluginManager::getPluginConfigurationSchema(const std::string& pluginName) const
{
   // If plugin is already loaded, use its information
   if (m_loadedPlugins.find(pluginName) != m_loadedPlugins.end())
      return m_loadedPlugins.at(pluginName)->getConfigurationSchema();

   return CHardwarePluginFactory::getConfigurationSchema(toPath(pluginName));
}

int CHardwarePluginManager::createInstance(const std::string& instanceName, const std::string& pluginName,
                                           std::string& configuration)
{
   // First step, record instance in database, to get its ID
   boost::shared_ptr<CHardware> dbRecord(new CHardware);
   dbRecord->setName(instanceName).setPluginName(pluginName).setConfiguration(configuration).setEnabled(true).setDeleted(false);
   int instanceId = m_database->addHardware(dbRecord);

   // Next create instance
   startInstance(instanceId);

   return instanceId;
}

void CHardwarePluginManager::deleteInstance(int id)
{
   // First step, stop instance
   stopInstance(id);

   // Next, delete in database (or flag as deleted)
   m_database->removeHardware(id);
}

boost::shared_ptr<std::vector<int> > CHardwarePluginManager::getInstanceList () const
{
   boost::shared_ptr<std::vector<int> > instances(new std::vector<int>);
   std::vector<boost::shared_ptr<CHardware> > databasePluginInstances = m_database->getHardwares();
   BOOST_FOREACH(boost::shared_ptr<CHardware> databasePluginInstance, databasePluginInstances)
   {
      if (!databasePluginInstance->getDeleted())
         instances->push_back(databasePluginInstance->getId());
   }

   return instances;
}

boost::shared_ptr<CHardwarePluginManager::PluginDetailedInstanceMap> CHardwarePluginManager::getInstanceListDetails () const
{
   boost::shared_ptr<PluginDetailedInstanceMap> instances(new std::map<int, boost::shared_ptr <const CHardware> >);
   std::vector<boost::shared_ptr<CHardware> > databasePluginInstances = m_database->getHardwares(true);
   BOOST_FOREACH(boost::shared_ptr<CHardware> databasePluginInstance, databasePluginInstances)
   {
      (*instances)[databasePluginInstance->getId()] = databasePluginInstance;
   }

   return instances;
}

std::string CHardwarePluginManager::getInstanceConfiguration(int id) const
{
   // First get database instance data
   BOOST_ASSERT(m_database->getHardware(id));
   boost::shared_ptr<CHardware> instanceData (m_database->getHardware(id));

   // Check if a schema is avalaible
   std::string pluginConfigurationSchema(getPluginConfigurationSchema(instanceData->getPluginName()));
   if (pluginConfigurationSchema.empty())
      return std::string(); // Plugin has no configuration

   // Returns configuration from database
   return instanceData->getConfiguration();
}

void CHardwarePluginManager::setInstanceConfiguration(int id, const std::string& newConfiguration)
{
   // First update configuration in database
   m_database->updateHardwareConfiguration(id, newConfiguration);

   // Next notify the instance, if running
   if (m_runningInstances.find(id) == m_runningInstances.end())
      return;  // Instance is not running, nothing to do more

   m_runningInstances[id]->updateConfiguration(newConfiguration);
}

void CHardwarePluginManager::onPluginDirectoryChanges(const boost::asio::dir_monitor_event& ev)
{
   YADOMS_LOG(debug) << "CHardwarePluginManager::onPluginDirectoryChanges" << ev.type;
   updatePluginList();
}

boost::filesystem::path CHardwarePluginManager::toPath(const std::string& pluginName) const
{
   boost::filesystem::path path(m_pluginPath);
   path /= CDynamicLibrary::ToFileName(pluginName);
   return path;
}

void CHardwarePluginManager::doStartInstance(int id)
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
      YADOMS_LOG(error) << "doStartInstance : " << e.what();   	
   }
}

void CHardwarePluginManager::doStopInstance(int id)
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
