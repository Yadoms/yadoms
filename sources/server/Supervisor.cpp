#include "stdafx.h"
#include "Supervisor.h"
#include "pluginSystem/HardwarePluginInstance.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include "tools/Log.h"

CSupervisor::CSupervisor(const IStartupOptions& startupOptions)
   :CThreadBase("Supervisor"),m_startupOptions(startupOptions)
{
}


CSupervisor::~CSupervisor(void)
{
   BOOST_ASSERT(getStatus()==kStopped);   // CSupervisor must be stopped before destroy
}

void CSupervisor::doWork()
{
   try
   {
      YADOMS_LOG_CONFIGURE("Supervisor");
      YADOMS_LOG(info) << "Supervisor is starting";

      CHardwarePluginManager hardwarePluginManager("../lib/Debug"/* TODO m_startupOptions.getHarwarePluginsPath() */);

      // Récupérer la liste des plugins à instancier depuis la base
      //TODO : vérifier que le plugin à instancier est dans la liste des plugins trouvés (cas de suppression d'un plugin)
      boost::shared_ptr<CHardwarePluginFactory> pFactory(hardwarePluginManager.getFactory("fakePlugin.dll"));
      CHardwarePluginInstance fakePlugin(pFactory->construct());
      fakePlugin.start();

      CHardwarePluginInstance fakePlugin2(pFactory->construct());
      fakePlugin2.start();
      fakePlugin2.updateConfiguration();



      YADOMS_LOG(info) << "Testing database" << std::endl;
      boost::shared_ptr<IDataProvider> pDataProvider (new CSQLiteDataProvider(m_startupOptions.getDatabaseFile()));
      if(pDataProvider->load())
      {

         std::vector<CHardware> hardwares = pDataProvider->getHardwareRequester()->getHardwares();
         std::vector<CHardware>::iterator i;
         // YADOMS_LOG(info) << "List of all hardwares"<< std::endl;
         for(i=hardwares.begin(); i!=hardwares.end(); i++)
         {
            YADOMS_LOG(info) << "Hardware Id=" << i->getId() << " Name=" << i->getName() << " PluginName=" << i->getPluginName();
         }
         YADOMS_LOG(info) << "[END] List of all hardwares"<< std::endl;
      }

      try
      {
         while(1)
         {
            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
         }
      }
      catch (boost::thread_interrupted&)
      {
         YADOMS_LOG(info) << "Supervisor is stopping...";
         fakePlugin.stop();
         fakePlugin2.stop();
      }

      pDataProvider->unload();//TODO : mettre un appel à unload dans le destructeur de IDataProvider (si pas déjà unloaded évidemment).

      YADOMS_LOG(info) << "Supervisor is stopped";
   }
   catch(std::exception& e)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception " << e.what();
   }
   catch(...)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception.";
   }
}
