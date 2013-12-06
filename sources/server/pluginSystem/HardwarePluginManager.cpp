#include "stdafx.h"
#include "HardwarePluginManager.h"
#include "HardwarePluginInstance.h"

CHardwarePluginManager::CHardwarePluginManager(const std::string& initialDir, boost::shared_ptr<IHardwareRequester> database)
   :m_database(database), m_pluginPath(initialDir)
{
   BOOST_ASSERT(m_database);
   Start();
}

CHardwarePluginManager::~CHardwarePluginManager()
{
   Stop();
}

void CHardwarePluginManager::Start()
{
   loadPlugins();

   // Get instances from database
   std::vector<boost::shared_ptr<CHardware> > databasePluginInstances = m_database->getHardwares();
   BOOST_FOREACH(boost::shared_ptr<CHardware> databasePluginInstance, databasePluginInstances)
   {
      if (m_plugins.find(databasePluginInstance->getPluginName()) == m_plugins.end())
      {
         // The plugin in database doesn't exist anymore
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

void CHardwarePluginManager::Stop()
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

      static const std::string pluginEndWithString = CFileSystem::DynamicLibraryDotExtension();

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

void CHardwarePluginManager::updatePluginList()
{
   //TODO
//TODO   throw std::logic_error("The method or operation is not implemented.");
}


