#include "stdafx.h"
#include "HardwarePluginManager.h"
#include "HardwarePluginInstance.h"
#include "HardwarePluginQualifier.h"


boost::shared_ptr<CHardwarePluginManager> CHardwarePluginManager::newHardwarePluginManager(
   const std::string & initialDir,
   boost::shared_ptr<IHardwareRequester> database,
   boost::shared_ptr<IHardwareEventLoggerRequester> eventLoggerDatabase,
   CEventHandler& supervisor,
   int pluginManagerEventId)
{
   boost::shared_ptr<CHardwarePluginManager> manager (new CHardwarePluginManager(initialDir, database, eventLoggerDatabase, supervisor, pluginManagerEventId));
   manager->init();
   return manager;
}

CHardwarePluginManager::CHardwarePluginManager(
   const std::string& initialDir,
   boost::shared_ptr<IHardwareRequester> database,
   boost::shared_ptr<IHardwareEventLoggerRequester> eventLoggerDatabase,
   CEventHandler& supervisor,
   int pluginManagerEventId)
   :m_database(database), m_pluginPath(initialDir), m_qualifier(new CHardwarePluginQualifier(eventLoggerDatabase)),
   m_supervisor(supervisor), m_pluginManagerEventId(pluginManagerEventId), m_terminate(false)
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

   while (!m_runningInstances.empty())
      stopInstance(m_runningInstances.begin()->first);

   YADOMS_LOG(info) << "CHardwarePluginManager all plugins are stopped";

   YADOMS_LOG(info) << "CHardwarePluginManager stop io service...";
   m_terminate = true;
   
   if(m_pluginIOService.get() != NULL)
      m_pluginIOService->stop();
   
   if(m_ioServiceThread.get())
      m_ioServiceThread->join();

   YADOMS_LOG(info) << "CHardwarePluginManager io service is stopped";
}


void CHardwarePluginManager::init()
{
   // Initialize the plugin list (detect available plugins)
   updatePluginList();

   //create ioservice for all plugin instances
   m_pluginIOService.reset(new boost::asio::io_service());
   m_ioServiceThread.reset(new boost::thread(boost::bind(&CHardwarePluginManager::runPluginIOService, this)));

   while (m_pluginIOService->stopped());

   // Create and start plugin instances from database
   std::vector<boost::shared_ptr<CHardware> > databasePluginInstances = m_database->getHardwares();
   BOOST_FOREACH(boost::shared_ptr<CHardware> databasePluginInstance, databasePluginInstances)
   {
      if (databasePluginInstance->getEnabled())
         startInstance(databasePluginInstance->getId());
   }

   // Start the directory changes monitor
   m_pluginsDirectoryMonitor.reset(new CDirectoryChangeListener(m_pluginPath, boost::bind(&CHardwarePluginManager::onPluginDirectoryChanges, this, _1)));

}

void CHardwarePluginManager::runPluginIOService()
{
   try
   {
      //m_pluginIOService->run();
      boost::asio::io_service::work work(*m_pluginIOService.get()); // 3
      m_pluginIOService->run();

      //while(!m_terminate)
      //   m_pluginIOService->poll_one();
   }
   catch (std::exception& e)
   {
      // Deal with exception as appropriate.
      YADOMS_LOG(error) << "CHardwarePluginManager io_service exception : " << e.what();
   }
}

void CHardwarePluginManager::enableInstance(int id)
{
   // Start instance (throw if fails)
   startInstance(id);

   // Update instance state in database
   m_database->enableInstance(id, true);
}

void CHardwarePluginManager::disableInstance(int id)
{
   // Start instance (throw if fails)
   stopInstance(id);

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

   // Signal qualifier that a plugin was loaded
   m_qualifier->signalLoad(pNewFactory->getInformation());

   return pNewFactory;
}

bool CHardwarePluginManager::unloadPlugin(const std::string& pluginName)
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

         YADOMS_LOG(info) << "Plugin " << pluginName << " successfully loaded";
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

int CHardwarePluginManager::getPluginQualityIndicator(const std::string& pluginName) const
{
   if (m_availablePlugins.find(pluginName) == m_availablePlugins.end())
      throw CInvalidPluginException(pluginName);   // Invalid plugin

   return m_qualifier->getQualityLevel(m_availablePlugins.at(pluginName));
}

std::string CHardwarePluginManager::getPluginConfigurationSchema(int id) const
{
   // Get database instance data
   BOOST_ASSERT(m_database->getHardware(id));
   boost::shared_ptr<CHardware> instanceData(m_database->getHardware(id));

   return getPluginConfigurationSchema(instanceData->getPluginName());
}

//TODO : supprimer cette fonction ?  à priori le pluginanager ne devrait pas faire d'insert en base
int CHardwarePluginManager::createInstance(const std::string& instanceName, const std::string& pluginName,
                                           const std::string& configuration)
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
   // First step, disable and stop instance
   disableInstance(id);

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
   // First check if a schema is available
   std::string pluginConfigurationSchema(getPluginConfigurationSchema(id));
   if (pluginConfigurationSchema.empty())
      return CStringExtension::EmptyString; // Plugin has no configuration

   // Next get database instance data
   BOOST_ASSERT(m_database->getHardware(id));
   boost::shared_ptr<CHardware> instanceData(m_database->getHardware(id));

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

void CHardwarePluginManager::signalEvent(const CHardwarePluginManagerEvent& event)
{
   switch(event.getSubEventId())
   {
   case CHardwarePluginManagerEvent::kPluginInstanceAbnormalStopped:
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
      boost::shared_ptr<CHardwarePluginInstance> pluginInstance(new CHardwarePluginInstance(plugin, databasePluginInstance, m_qualifier, m_supervisor, m_pluginManagerEventId, m_pluginIOService));
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
   std::string pluginName = m_runningInstances[id]->getPluginName();

   // Remove (=stop) instance
   m_runningInstances.erase(id);

   // Try to unload associated plugin (if no more used)
   unloadPlugin(pluginName);
}
