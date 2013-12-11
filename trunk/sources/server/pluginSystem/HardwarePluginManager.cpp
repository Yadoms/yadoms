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
   :m_database(database), m_pluginPath(initialDir),
   m_pluginDirectoryChangesFlag(true)  // Initialized to true to force first plugin scan
{
   BOOST_ASSERT(m_database);
}

CHardwarePluginManager::~CHardwarePluginManager()
{
   stop();
}

void CHardwarePluginManager::init()
{
   m_pluginsDirectoryMonitor.reset(new CDirectoryChangeListener(m_pluginPath, boost::bind(&CHardwarePluginManager::onPluginDirectoryChanges, this, _1)));
   start();
}

void CHardwarePluginManager::start()
{
   loadPlugins();

   // Get instances from database
   std::vector<boost::shared_ptr<CHardware> > databasePluginInstances = m_database->getHardwares();
   BOOST_FOREACH(boost::shared_ptr<CHardware> databasePluginInstance, databasePluginInstances)
   {
      if (m_plugins.find(databasePluginInstance->getPluginName()) == m_plugins.end())
      {
         // The plugin doesn't exist anymore
         YADOMS_LOG(error) << "Plugin #" << databasePluginInstance->getId() <<
            " (" << databasePluginInstance->getPluginName() <<
            ") for instance " << databasePluginInstance->getName() << " in database was not found";

         continue;      
      }
      
      // Create the instance
      boost::shared_ptr<CHardwarePluginInstance> pluginInstance(
         new CHardwarePluginInstance(m_plugins[databasePluginInstance->getPluginName()],
            m_database->getHardware(databasePluginInstance->getId())));
      m_pluginInstances[databasePluginInstance->getId()] = pluginInstance;
   }

   // Start all plugin instances
   BOOST_FOREACH(PluginInstanceMap::value_type instanceIt, m_pluginInstances)
   {
      instanceIt.second->start();
   }
}

void CHardwarePluginManager::stop()
{
   // Stop all plugins instance
   BOOST_FOREACH(PluginInstanceMap::value_type instanceIt, m_pluginInstances)
   {
      instanceIt.second->stop();
   }
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

void CHardwarePluginManager::loadPlugins()
{
   // Search for library files
   std::vector<boost::filesystem::path> avalaiblePluginFileNames = findPluginFilenames();

   for (std::vector<boost::filesystem::path>::const_iterator libPathIt = avalaiblePluginFileNames.begin() ;
      libPathIt != avalaiblePluginFileNames.end() ; ++libPathIt)
   {
      // Generate factory for current found plugin
      try
      {
         boost::shared_ptr<CHardwarePluginFactory> pNewFactory (new CHardwarePluginFactory(*libPathIt));
         // m_plugins key is just the library name (without extension)
         m_plugins[(*libPathIt).stem().string()] = pNewFactory;
      }
      catch (CInvalidPluginException& e)
      {
         YADOMS_LOG(warning) << e.what() << " will not be available in Yadoms";
      }
   }
}

void CHardwarePluginManager::buildAvailablePluginList()
{
   // Search for library files
   std::vector<boost::filesystem::path> avalaiblePluginFileNames = findPluginFilenames();

   for (std::vector<boost::filesystem::path>::const_iterator libPathIt = avalaiblePluginFileNames.begin() ;
      libPathIt != avalaiblePluginFileNames.end() ; ++libPathIt)
   {
      try
      {
         // Get informations for current found plugin
         // m_plugins key is just the library name (without extension)
         boost::shared_ptr<CHardwarePluginInformation> pluginInformation (new CHardwarePluginInformation(CHardwarePluginFactory::getInformation(*libPathIt)));
         m_availablePlugins[(*libPathIt).stem().string()] = pluginInformation;
      }
      catch (CInvalidPluginException& e)
      {
         // Invalid plugin
         YADOMS_LOG(warning) << e.what() << " found in plugin path but is not a valid plugin";
      }
   }
}

CHardwarePluginManager::AvalaiblePluginMap CHardwarePluginManager::getPluginList()
{
   bool needToRebuildPluginList = false;

   {
      boost::lock_guard<boost::mutex> lock(m_pluginDirectoryChangesMutex);
      needToRebuildPluginList = m_pluginDirectoryChangesFlag;
      m_pluginDirectoryChangesFlag = false;
   }

   if (needToRebuildPluginList)
   {
      // Plugin directory have change, re-built plugin available list
      buildAvailablePluginList();
   }

   return m_availablePlugins;
}

void CHardwarePluginManager::onPluginDirectoryChanges(const boost::asio::dir_monitor_event& ev)
{
   YADOMS_LOG(debug) << "CHardwarePluginManager::onPluginDirectoryChanges" << ev.type;

   boost::lock_guard<boost::mutex> lock(m_pluginDirectoryChangesMutex);
   m_pluginDirectoryChangesFlag = true;
}

