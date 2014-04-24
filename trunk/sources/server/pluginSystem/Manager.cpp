#include "stdafx.h"
#include "Manager.h"
#include "Instance.h"
#include "Qualifier.h"
#include <shared/DynamicLibrary.hpp>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/NotSupported.hpp>


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
   YADOMS_LOG(debug) << "Thread Id=" << m_ioServiceThread->get_id() << " Name = IO Service (pluginsystem::CManager)";

   // Create and start plugin instances from database
   std::vector<boost::shared_ptr<database::entities::CPlugin> > databasePluginInstances = m_database->getInstances();
   BOOST_FOREACH(boost::shared_ptr<database::entities::CPlugin> databasePluginInstance, databasePluginInstances)
   {
      if (databasePluginInstance->Enabled())
         startInstance(databasePluginInstance->Id());
   }

   // Start the directory changes monitor
   m_pluginsDirectoryMonitor.reset(new tools::CDirectoryChangeListener(m_pluginPath, boost::bind(&CManager::onPluginDirectoryChanges, this, _1)));

}

void CManager::runPluginIOService()
{
   try
   {
      boost::asio::io_service::work work(m_pluginIOService);
      m_pluginIOService.run();
   }
   catch (std::exception& e)
   {
      // Deal with exception as appropriate.
      YADOMS_LOG(error) << "pluginSystem::CManager io_service exception : " << e.what();
   }
}

std::vector<boost::filesystem::path> CManager::findPluginDirectories()
{
   // Look for all subdirectories in m_pluginPath directory, where it contains library with same name,
   // for example a subdirectory "fakePlugin" containing a "fakePlugin.dll|so" file
   std::vector<boost::filesystem::path> plugins;

   if (boost::filesystem::exists(m_pluginPath) && boost::filesystem::is_directory(m_pluginPath))
   {
      // Check all subdirectories in m_pluginPath
      for(boost::filesystem::directory_iterator subDirIterator(m_pluginPath) ; subDirIterator != boost::filesystem::directory_iterator() ; ++subDirIterator)
      {
         if (boost::filesystem::is_directory(subDirIterator->status()))
         {
            // Subdirectory, check if it is a plugin (= contains a dynamic library with same name)
            const std::string expectedLibName(shared::CDynamicLibrary::ToFileName(subDirIterator->path().filename().string()));
            for (boost::filesystem::directory_iterator fileIterator(subDirIterator->path()); fileIterator != boost::filesystem::directory_iterator() ; ++fileIterator)
            {
               if (boost::filesystem::is_regular_file(fileIterator->status()) &&                // It's a file...
                  fileIterator->path().filename() == expectedLibName)                           // ...with the same name as sub-directory...
               {
                  plugins.push_back(subDirIterator->path());
               }
            }
         }
      }
   }

   return plugins;
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
   std::vector<boost::filesystem::path> avalaiblePluginDirectories = findPluginDirectories();

   for (std::vector<boost::filesystem::path>::const_iterator libPathIt = avalaiblePluginDirectories.begin() ;
      libPathIt != avalaiblePluginDirectories.end() ; ++libPathIt)
   {
      try
      {
	 // Get informations for current found plugin
         std::string pluginName = (*libPathIt).filename().string();

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

int CManager::getPluginQualityIndicator(const std::string& pluginName) const
{
   if (m_availablePlugins.find(pluginName) == m_availablePlugins.end())
      throw CInvalidPluginException(pluginName);   // Invalid plugin

   return m_qualifier->getQualityLevel(m_availablePlugins.at(pluginName));
}

int CManager::createInstance(const database::entities::CPlugin& data)
{
   // First step, record instance in database, to get its ID
   int instanceId = m_database->addInstance(data);

   // Next create instance
   startInstance(instanceId);

   return instanceId;
}

void CManager::deleteInstance(int id)
{
   try
   {
      // First step, disable and stop instance
      stopInstance(id);

      // Next, delete in database
      m_database->removeInstance(id);
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(error) << "Unable to delete plugin instance (" << id << ") : " << e.what();
      throw shared::exception::CInvalidParameter(boost::lexical_cast<std::string>(id));
   }
}

std::vector<boost::shared_ptr<database::entities::CPlugin> > CManager::getInstanceList () const
{
   return m_database->getInstances();
}

boost::shared_ptr<database::entities::CPlugin> CManager::getInstance(int id) const
{
   return m_database->getInstance(id);
}

void CManager::updateInstance(const database::entities::CPlugin& newData)
{
   // Check for supported modifications
   if (!newData.Id.isDefined())
   {
      BOOST_ASSERT(false); // ID must be provided
      throw new shared::exception::CException("Update instance : instance ID was not provided");
   }

   // First get old configuration from database
   boost::shared_ptr<const database::entities::CPlugin> previousData = m_database->getInstance(newData.Id());

   // Next, update configuration in database
   m_database->updateInstance(newData);

   // Last, apply modifications
   if (newData.Enabled.isDefined() && previousData->Enabled() != newData.Enabled())
   {
      // Enable state was updated
      if (newData.Enabled())
         startInstance(newData.Id());
      else
         stopInstance(newData.Id());
   }
   else if (newData.Configuration.isDefined()
      && previousData->Configuration() != newData.Configuration()) // No need to notify configuration if instance was enabled/disabled
   {
      // Configuration was updated, notify the instance, if running
      if (m_runningInstances.find(newData.Id()) != m_runningInstances.end())
         m_runningInstances[newData.Id()]->updateConfiguration(newData.Configuration());
   }
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
            m_database->disableAllPluginInstances(event.getPluginInformation()->getName());
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
   path /= pluginName;
   path /= shared::CDynamicLibrary::ToFileName(pluginName);
   return path;
}

void CManager::startInstance(int id)
{
   if (m_runningInstances.find(id) != m_runningInstances.end())
      return;     // Already started ==> nothing more to do

   // Get instance informations from database
   boost::shared_ptr<database::entities::CPlugin> databasePluginInstance(m_database->getInstance(id));

   // Load the plugin
   try
   {
      boost::shared_ptr<CFactory> plugin(loadPlugin(databasePluginInstance->PluginName()));

      // Create instance
      BOOST_ASSERT(plugin); // Plugin not loaded
      boost::shared_ptr<CInstance> pluginInstance(new CInstance(plugin, databasePluginInstance, m_qualifier, m_supervisor, m_pluginManagerEventId, &m_pluginIOService));
      m_runningInstances[databasePluginInstance->Id()] = pluginInstance;
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
