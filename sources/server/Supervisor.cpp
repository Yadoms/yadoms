#include "Supervisor.h"
#include <boost/log/trivial.hpp>
#include "PluginSystem/HardwarePluginInstance.h"
#include "database/sqlite/SQLiteDataProvider.h"

CSupervisor::CSupervisor(const IStartupOptions& startupOptions)
   :CThreadBase("Supervisor"),m_startupOptions(startupOptions)
{
   //TODO pour test, à virer
   std::cout << "log level = " << m_startupOptions.getLogLevel() << std::endl;
   std::cout << "port number = " << m_startupOptions.getPortNumber() << std::endl;
   std::cout << "db path = " << m_startupOptions.getDatabaseFile() << std::endl;
   std::cout << "hardware plugins = " << m_startupOptions.getHarwarePluginsPath() << std::endl;
   std::cout << "device plugins = " << m_startupOptions.getDevicePluginsPath() << std::endl;
   std::cout << "start Xpl Hub = " << m_startupOptions.getStartXplHubFlag() << std::endl;
   //\TODO pour test, à virer
}


CSupervisor::~CSupervisor(void)
{
   BOOST_ASSERT(getStatus()==kStopped);   // CSupervisor must be stopped before destroy
}

void CSupervisor::doWork()
{
   try
   {
      BOOST_LOG_TRIVIAL(info) << "Supervisor is starting";

      m_hardwarePluginManager.buildPluginFactoryList(m_startupOptions.getHarwarePluginsPath());

      //récupérer la liste des plugins à instancier depuis la base
      CHardwarePluginFactory * pFactory = m_hardwarePluginManager.getFactory("FakePlugin");
      CHardwarePluginInstance fakePlugin(pFactory->construct());
      fakePlugin.start();

      CHardwarePluginInstance fakePlugin2(pFactory->construct());
      fakePlugin2.start();


      BOOST_LOG_TRIVIAL(info) << "Testing database" << std::endl;
      IDataProvider * pDataProvider = new CSQLiteDataProvider(m_startupOptions.getDatabaseFile());
      if(pDataProvider->load())
      {

         std::vector<CHardware> hardwares = pDataProvider->getHardwareRequester()->getHardwares();
         std::vector<CHardware>::iterator i;
         BOOST_LOG_TRIVIAL(info) << "List of all hardwares"<< std::endl;
         for(i=hardwares.begin(); i!=hardwares.end(); i++)
         {
            BOOST_LOG_TRIVIAL(info) << "Hardware Id=" << i->getId() << " Name=" << i->getName() << " PluginName=" << i->getPluginName();
         }
         BOOST_LOG_TRIVIAL(info) << "[END] List of all hardwares"<< std::endl;
      }

      try
      {
         while(1)
         {
            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
         }
      }
      catch (boost::thread_interrupted& e)
      {
         BOOST_LOG_TRIVIAL(info) << "Supervisor is stopping...";
         fakePlugin.stop();
         fakePlugin2.stop();
      }

      pDataProvider->unload();
      delete pDataProvider;

      BOOST_LOG_TRIVIAL(info) << "Supervisor is stopped";
   }
   catch(...)
   {
      BOOST_LOG_TRIVIAL(error) << "Supervisor : unhandled exception.";
   }

   m_hardwarePluginManager.freePluginFactoryList();
}