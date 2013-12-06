#include "stdafx.h"
#include "Supervisor.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include "tools/Log.h"

CSupervisor::CSupervisor(const IStartupOptions& startupOptions)
   :CThreadBase("Supervisor"), m_startupOptions(startupOptions)
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


      YADOMS_LOG(info) << "Testing database";
      boost::shared_ptr<IDataProvider> pDataProvider (new CSQLiteDataProvider(m_startupOptions.getDatabaseFile()));
      if(pDataProvider->load())
      {

         std::vector<boost::shared_ptr<CHardware> > hardwares = pDataProvider->getHardwareRequester()->getHardwares();
         YADOMS_LOG(info) << "List of all hardwares";
         BOOST_FOREACH(boost::shared_ptr<CHardware> hardware, hardwares)
         {
            YADOMS_LOG(info) << "Name=" << hardware->getName() << " PluginName=" << hardware->getPluginName();
         }
         YADOMS_LOG(info) << "[END] List of all hardwares";
      }

      // Start the harware plugin manager
      CHardwarePluginManager hardwarePluginManager(
         "../builds/lib/Debug"/* TODO m_startupOptions.getHarwarePluginsPath() */,
         pDataProvider->getHardwareRequester());

      try
      {
         while(1)
         {
            // Scan for new/deleted plugins
            //TODO if (toutes les 10 secondes)
            {
               hardwarePluginManager.updatePluginList();
            }

            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
         }
      }
      catch (boost::thread_interrupted&)
      {
         YADOMS_LOG(info) << "Supervisor is stopping...";
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
