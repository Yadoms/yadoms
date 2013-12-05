#include "stdafx.h"
#include "HardwarePluginManager.h"
#include "HardwarePluginInstance.h"

CHardwarePluginManager::CHardwarePluginManager(const std::string& initialDir, boost::shared_ptr<IHardwareRequester> database)
   :m_database(database), m_pluginPath(initialDir)
{
   //TODO : remettre   BOOST_ASSERT(m_database);
   Start();
}

CHardwarePluginManager::~CHardwarePluginManager()
{
   Stop();
}

void CHardwarePluginManager::Start()
{
   loadPlugins();

   /*TODO : ce code est prêt pour quand la base pourra nous donner à manger : pour l'instant, m_databaseest NULL
   // Get instances from database
   std::vector<CHardware> databasePluginInstances = m_database->getHardwares();
   BOOST_FOREACH(CHardware databasePluginInstance, databasePluginInstances)
   {
      if (m_plugins.find(databasePluginInstance.getPluginName()) == m_plugins.end())
      {
         // The plugin in database doesn't exist anymore
         YADOMS_LOG(error) << "Plugin " << databasePluginInstance.getPluginName() <<
            " for instance " << databasePluginInstance.getName() << " in database was not found";

         continue;      
      }
      
      // Create the instance
      boost::shared_ptr<IHardwarePlugin> pluginObject(m_plugins[databasePluginInstance.getPluginName()]->construct());

      boost::shared_ptr<CHardwarePluginInstance> pluginInstance(new CHardwarePluginInstance(databasePluginInstance.getName(), pluginObject));
      m_pluginInstances[databasePluginInstance.getName()] = pluginInstance;
   }

   // Start all plugin instances
   BOOST_FOREACH(PluginInstanceMap::value_type instanceIt, m_pluginInstances)
   {
      instanceIt.second->start();
   }
   */

   // TODO ne serait-il pas plus logique de créer un thread pour le plugin manager ? Ou de créer un thread
   // qui surveille l'état du répertoire de plugin et averti si détection de nouveau plugin

   // Récupérer la liste des plugins à instancier depuis la base
   //TODO : vérifier que le plugin à instancier est dans la liste des plugins trouvés (cas de suppression d'un plugin)
   boost::shared_ptr<IHardwarePlugin> pluginInstance (m_plugins["fakePlugin.dll"]->construct());
   CHardwarePluginInstance fakePlugin("fakePlugin.dll", pluginInstance);
   fakePlugin.start();

   //CHardwarePluginInstance fakePlugin2(pFactory->construct());
   //fakePlugin2.start();
   //fakePlugin2.updateConfiguration();
}

void CHardwarePluginManager::Stop()
{
   //TODO stop all plugins instance
   //fakePlugin.stop();
   //fakePlugin2.stop();

   //TODO unload plugins
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
         m_plugins[(*libPathIt).filename().string()] = pNewFactory;
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


