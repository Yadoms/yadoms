#include "stdafx.h"
#include "Manager.h"
#include "Instance.h"
#include "Qualifier.h"


namespace pluginSystem
{

boost::shared_ptr<CManager> CManager::newManager(
   const std::string & initialDir,
   boost::shared_ptr<database::IPluginRequester> database,
   boost::shared_ptr<database::IPluginEventLoggerRequester> eventLoggerDatabase,
   shared::event::CEventHandler& supervisor,
   int pluginManagerEventId)
{
   boost::shared_ptr<CManager> manager (new CManager(initialDir, database, eventLoggerDatabase, supervisor, pluginManagerEventId));
   manager->init();
   return manager;
}

CManager::CManager(
   const std::string& initialDir,
   boost::shared_ptr<database::IPluginRequester> database,
   boost::shared_ptr<database::IPluginEventLoggerRequester> eventLoggerDatabase,
   shared::event::CEventHandler& supervisor,
   int pluginManagerEventId)
   :m_database(database), m_pluginPath(initialDir), m_qualifier(new CQualifier(eventLoggerDatabase)),
   m_supervisor(supervisor), m_pluginManagerEventId(pluginManagerEventId)
{
   BOOST_ASSERT(m_database);
}

CManager::~CManager()
{
   stop();
}

void CManager::stop()
{
   YADOMS_LOG(info) << "pluginSystem::CManager stop io service...";
   if(!m_pluginIOService.stopped())
   {
      m_pluginIOService.stop();
      while(!m_pluginIOService.stopped());
   }

   if(m_ioServiceThread.get())
      m_ioServiceThread->join();

   YADOMS_LOG(info) << "pluginSystem::CManager stop plugins...";

   while (!m_runningInstances.empty())
      stopInstance(m_runningInstances.begin()->first);

   YADOMS_LOG(info) << "pluginSystem::CManager all plugins are stopped";
}


void CManager::init()
{
   // Initialize the plugin list (detect available plugins)
   updatePluginList();

   //create ioservice for all plugin instances
   m_ioServiceThread.reset(new boost::thread(boost::bind(&CManager::runPluginIOService, this)));

   // Create and start plugin instances from database
   std::vector<boost::shared_ptr<database::entities::CPlugin> > databasePluginInstances = m_database->getPlugins();
   BOOST_FOREACH(boost::shared_ptr<database::entities::CPlugin> databasePluginInstance, databasePluginInstances)
   {
      if (databasePluginInstance->getEnabled())
         startInstance(databasePluginInstance->getId());
   }

   // Start the directory changes monitor
   m_pluginsDirectoryMonitor.reset(new tools::CDirectoryChangeListener(m_pluginPath, boost::bind(&CManager::onPluginDirectoryChanges, this, _1)));

}

void CManager::runPluginIOService()
{
   try
   {
      boost::asio::io_service::work work(m_pluginIOService); // 3
      m_pluginIOService.run();
   }
   catch (std::exception& e)
   {
      // Deal with exception as appropriate.
      YADOMS_LOG(error) << "pluginSystem::CManager io_service exception : " << e.what();
   }
}

void CManager::enableInstance(int id)
{
   // Start instance (throw if fails)
   startInstance(id);

   // Update instance state in database
   m_database->enableInstance(id, true);
}

void CManager::disableInstance(int id)
{
   // Start instance (throw if fails)
   stopInstance(id);

   // Update instance state in database
   m_database->enableInstance(id, false);
}

std::vector<boost::filesystem::path> CManager::findPluginFilenames()
{
   std::vector<boost::filesystem::path> pluginFilenames;

   if (boost::filesystem::exists(m_pluginPath) && boost::filesystem::is_directory(m_pluginPath))
   {
      boost::filesystem::directory_iterator endFileIterator;

      static const std::string pluginEndWithString = shared::CDynamicLibrary::DotExtension();

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

boost::shared_ptr<CFactory> CManager::loadPlugin(const std::string& pluginName)
{
   // Check if already loaded
   if (m_loadedPlugins.find(pluginName) != m_loadedPlugins.end())
      return m_loadedPlugins[pluginName];  // Plugin already loaded

   // Check if plugin is available
   if (m_availablePlugins.find(pluginName) == m_availablePlugins.end())
      throw CInvalidPluginException(pluginName);   // Invalid plugin

   // Load the plugin
   boost::shared_ptr<CFactory> pNewFactory (new CFactory(toPath(pluginName)));
   m_loadedPlugins[pluginName] = pNewFactory;

   // Signal qualifier that a plugin was loaded
   m_qualifier->signalLoad(pNewFactory->getInformation());

   return pNewFactory;
}

bool CManager::unloadPlugin(const std::string& pluginName)
{
   PluginInstanceMap::const_iterator instance;
   for (instance = m_runningInstances.begin() ; instance != m_runningInstances.end() ; ++instance)
   {
      if ((*instance).second->getPluginName() == pluginName)
         break;
   }
   if (instance != m_runningInstances.end())
      return false;  // No unload : plugin is still used by another instance

   // Signal qualifier that a plugin is about to be unloaded
   m_qualifier->signalUnload(m_loadedPlugins[pluginName]->getInformation());

   // Effectively unload plugin
   m_loadedPlugins.erase(pluginName);

   return true;
}

void CManager::buildAvailablePluginList()
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
         const std::string& pluginName = shared::CDynamicLibrary::ToLibName((*libPathIt).string());

         // If plugin is already loaded, use its information
         if (m_loadedPlugins.find(pluginName) != m_loadedPlugins.end())
            m_availablePlugins[pluginName] = m_loadedPlugins[pluginName]->getInformation();
         else
            m_availablePlugins[pluginName] = CFactory::getInformation(toPath(pluginName));

         YADOMS_LOG(info) << "Plugin " << pluginName << " successfully loaded";
      }
      catch (CInvalidPluginException& e)
      {
         // Invalid plugin
         YADOMS_LOG(warning) << e.what() << " found in plugin path but is not a valid plugin";
      }
   }
}

void CManager::updatePluginList()
{
   // Plugin directory have change, so re-build plugin available list
   boost::lock_guard<boost::mutex> lock(m_availablePluginsMutex);
   buildAvailablePluginList();
}


CManager::AvalaiblePluginMap CManager::getPluginList()
{
   boost::lock_guard<boost::mutex> lock(m_availablePluginsMutex);
   AvalaiblePluginMap mapCopy = m_availablePlugins;
   return mapCopy;
}

std::string CManager::getPluginConfigurationSchema(const std::string& pluginName) const
{
   // If plugin is already loaded, use its information
   if (m_loadedPlugins.find(pluginName) != m_loadedPlugins.end())
      return m_loadedPlugins.at(pluginName)->getConfigurationSchema();

   return CFactory::getConfigurationSchema(toPath(pluginName));
}

int CManager::getPluginQualityIndicator(const std::string& pluginName) const
{
   if (m_availablePlugins.find(pluginName) == m_availablePlugins.end())
      throw CInvalidPluginException(pluginName);   // Invalid plugin

   return m_qualifier->getQualityLevel(m_availablePlugins.at(pluginName));
}

std::string CManager::getPluginConfigurationSchema(int id) const
{
   // Get database instance data
   BOOST_ASSERT(m_database->getPlugin(id));
   boost::shared_ptr<database::entities::CPlugin> instanceData(m_database->getPlugin(id));

   return getPluginConfigurationSchema(instanceData->getPluginName());
}

int CManager::createInstance(const std::string& instanceName,
   const std::string& pluginName, const std::string& configuration)
{
   // First step, record instance in database, to get its ID
   boost::shared_ptr<database::entities::CPlugin> dbRecord(new database::entities::CPlugin);
   dbRecord->setName(instanceName).setPluginName(pluginName).setConfiguration(configuration).setEnabled(true).setDeleted(false);
   int instanceId = m_database->addPlugin(dbRecord);

   // Next create instance
   startInstance(instanceId);

   return instanceId;
}

void CManager::deleteInstance(int id)
{
   // First step, disable and stop instance
   disableInstance(id);

   // Next, delete in database (or flag as deleted)
   m_database->removePlugin(id);
}

boost::shared_ptr<std::vector<int> > CManager::getInstanceList () const
{
   boost::shared_ptr<std::vector<int> > instances(new std::vector<int>);
   std::vector<boost::shared_ptr<database::entities::CPlugin> > databasePluginInstances = m_database->getPlugins();
   BOOST_FOREACH(boost::shared_ptr<database::entities::CPlugin> databasePluginInstance, databasePluginInstances)
   {
      if (!databasePluginInstance->getDeleted())
         instances->push_back(databasePluginInstance->getId());
   }

   return instances;
}

boost::shared_ptr<CManager::PluginDetailedInstanceMap> CManager::getInstanceListDetails () const
{
   boost::shared_ptr<PluginDetailedInstanceMap> instances(new std::map<int, boost::shared_ptr <const database::entities::CPlugin> >);
   std::vector<boost::shared_ptr<database::entities::CPlugin> > databasePluginInstances = m_database->getPlugins(true);
   BOOST_FOREACH(boost::shared_ptr<database::entities::CPlugin> databasePluginInstance, databasePluginInstances)
   {
      (*instances)[databasePluginInstance->getId()] = databasePluginInstance;
   }

   return instances;
}

std::string CManager::getInstanceConfiguration(int id) const
{
   // First check if a schema is available
   std::string pluginConfigurationSchema(getPluginConfigurationSchema(id));
   if (pluginConfigurationSchema.empty())
      return shared::CStringExtension::EmptyString; // Plugin has no configuration

   // Next get database instance data
   BOOST_ASSERT(m_database->getPlugin(id));
   boost::shared_ptr<database::entities::CPlugin> instanceData(m_database->getPlugin(id));

   // Returns configuration from database
   return instanceData->getConfiguration();
}

void CManager::setInstanceConfiguration(int id, const std::string& newConfiguration)
{
   // First update configuration in database
   m_database->updatePluginConfiguration(id, newConfiguration);

   // Next notify the instance, if running
   if (m_runningInstances.find(id) == m_runningInstances.end())
      return;  // Instance is not running, nothing to do more

   m_runningInstances[id]->updateConfiguration(newConfiguration);
}

void CManager::signalEvent(const CManagerEvent& event)
{
   switch(event.getSubEventId())
   {
   case CManagerEvent::kPluginInstanceAbnormalStopped:
      {
         // The thread of an instance has stopped in a non-conventional way (probably crashed)

         // First perform the full stop
         stopInstance(event.getInstanceId());

         // Now, evaluate if it is still safe
         if (m_qualifier->isSafe(event.getPluginInformation()))
         {
            // Don't restart if event occurs when instance was stopping
            if (!event.getStopping())
            {
               // Still safe, try to restart it
               startInstance(event.getInstanceId());
            }
         }
         else
         {
            // Not safe anymore. Disable it (user will just be able to start it manually)
            // Not that this won't stop other instances of this plugin
            YADOMS_LOG(warning) << " plugin " << event.getPluginInformation()->getName() << " was evaluated as not safe and disabled.";
            m_database->disableAllPluginInstance(event.getPluginInformation()->getName());
         }

         break;
      }
   default:
      {
         YADOMS_LOG(error) << "Unknown message id";
         BOOST_ASSERT(false);
         break;
      }
   }
}

void CManager::onPluginDirectoryChanges(const boost::asio::dir_monitor_event& ev)
{
   YADOMS_LOG(debug) << "pluginSystem::CManager::onPluginDirectoryChanges" << ev.type;
   updatePluginList();
}

boost::filesystem::path CManager::toPath(const std::string& pluginName) const
{
   boost::filesystem::path path(m_pluginPath);
   path /= shared::CDynamicLibrary::ToFileName(pluginName);
   return path;
}

void CManager::startInstance(int id)
{
   if (m_runningInstances.find(id) != m_runningInstances.end())
      return;     // Already started ==> nothing more to do

   // Get instance informations from database
   boost::shared_ptr<database::entities::CPlugin> databasePluginInstance (m_database->getPlugin(id));

   // Load the plugin
   try
   {
      boost::shared_ptr<CFactory> plugin(loadPlugin(databasePluginInstance->getPluginName()));

      // Create instance
      BOOST_ASSERT(plugin); // Plugin not loaded
      boost::shared_ptr<CInstance> pluginInstance(new CInstance(plugin, databasePluginInstance, m_qualifier, m_supervisor, m_pluginManagerEventId, &m_pluginIOService));
      m_runningInstances[databasePluginInstance->getId()] = pluginInstance;
   }
   catch (CInvalidPluginException& e)
   {
      YADOMS_LOG(error) << "startInstance : " << e.what();   	
   }
}

void CManager::stopInstance(int id)
{
   if (m_runningInstances.find(id) == m_runningInstances.end())
      return;     // Already stopped ==> nothing more to do

   // Get the associated plugin name to unload it after instance being deleted
   std::string pluginName = m_runningInstances[id]->getPluginName();

   // Remove (=stop) instance
   m_runningInstances.erase(id);

   // Try to unload associated plugin (if no more used)
   unloadPlugin(pluginName);
}

} // namespace pluginSystem
