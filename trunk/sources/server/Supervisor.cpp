#include "stdafx.h"
#include "Supervisor.h"
#include "pluginSystem/HardwarePluginInstance.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include "Log.h"

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
      
      BOOST_LOG_SCOPED_THREAD_TAG("ThreadName", "Supervisor");
      BOOST_LOG_SEV(my_logger::get(), boost::log::trivial::info) << "Supervisor is starting";

      CHardwarePluginManager hardwarePluginManager("../lib/Debug"/* TODO m_startupOptions.getHarwarePluginsPath() */);

      // R�cup�rer la liste des plugins � instancier depuis la base
      //TODO : v�rifier que le plugin � instancier est dans la liste des plugins trouv�s (cas de suppression d'un plugin)
      boost::shared_ptr<CHardwarePluginFactory> pFactory(hardwarePluginManager.getFactory("fakePlugin.dll"));
      CHardwarePluginInstance fakePlugin(pFactory->construct());
      fakePlugin.start();

      CHardwarePluginInstance fakePlugin2(pFactory->construct());
      fakePlugin2.start();
      fakePlugin2.updateConfiguration();



      //BOOST_LOG_TRIVIAL(info) << "Testing database" << std::endl;
      boost::shared_ptr<IDataProvider> pDataProvider (new CSQLiteDataProvider(m_startupOptions.getDatabaseFile()));
      if(pDataProvider->load())
      {

         std::vector<CHardware> hardwares = pDataProvider->getHardwareRequester()->getHardwares();
         std::vector<CHardware>::iterator i;
         BOOST_LOG_TRIVIAL(info) << "List of all hardwares"<< std::endl;
         for(i=hardwares.begin(); i!=hardwares.end(); i++)
         {
            //BOOST_LOG_TRIVIAL(info) << "Hardware Id=" << i->getId() << " Name=" << i->getName() << " PluginName=" << i->getPluginName();
         }
         //BOOST_LOG_TRIVIAL(info) << "[END] List of all hardwares"<< std::endl;
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
         //BOOST_LOG_TRIVIAL(info) << "Supervisor is stopping...";
         fakePlugin.stop();
         fakePlugin2.stop();
      }

      pDataProvider->unload();//TODO : mettre un appel � unload dans le destructeur de IDataProvider (si pas d�j� unloaded �videmment).

      //BOOST_LOG_TRIVIAL(info) << "Supervisor is stopped";
   }
   catch(std::exception& e)
   {
      BOOST_LOG_TRIVIAL(error) << "Supervisor : unhandled exception " << e.what();
   }
   catch(...)
   {
      //BOOST_LOG_TRIVIAL(error) << "Supervisor : unhandled exception.";
   }
}
