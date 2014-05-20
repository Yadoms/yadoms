#include "stdafx.h"
#include "Supervisor.h"
#include "pluginSystem/Manager.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/DatabaseException.hpp"
#include <shared/Log.h>
#include <shared/exception/NotSupported.hpp>
#include "web/webem/WebServer.h"
#include "web/WebServerManager.h"
#include <shared/xpl/XplHub.h>
#include "communication/XplGateway.h"
#include "web/rest/service/PluginRestService.h"
#include "web/rest/service/DeviceRestService.h"
#include "web/rest/service/PageRestService.h"
#include "web/rest/service/WidgetRestService.h"
#include "web/rest/service/ConfigurationRestService.h"
#include "web/rest/service/PluginEventLoggerRestService.h"
#include "web/rest/service/EventLoggerRestService.h"
#include "web/rest/service/GeneralRestService.h"
#include <shared/ThreadBase.h>
#include <shared/Peripherals.h>
#include "tools/web/FileDownloader.h"
#include "task/Scheduler.h"
#include "task/update/Plugin.h"
#include "task/backup/Database.h"

CSupervisor::CSupervisor(const startupOptions::IStartupOptions& startupOptions)
   :CThreadBase("Supervisor"), m_startupOptions(startupOptions)
{
}


CSupervisor::~CSupervisor(void)
{
   BOOST_ASSERT(getStatus() == kStopped);   // CSupervisor must be stopped before destroy
}

void CSupervisor::doWork()
{
   YADOMS_LOG_CONFIGURE("Supervisor");
   YADOMS_LOG(info) << "Supervisor is starting";   

   boost::shared_ptr<database::IDataProvider> pDataProvider;
   try
   {
      pDataProvider.reset(new database::sqlite::CSQLiteDataProvider(m_startupOptions.getDatabaseFile()));
      if (!pDataProvider->load())
      {
         throw shared::exception::CException("Fail to load database");
      }

      // Start Task manager
      boost::shared_ptr<task::CScheduler> taskManager = boost::shared_ptr<task::CScheduler>(new task::CScheduler(*this, kSystemEvent));

      // Start Xpl Hub (only if necessary)
      boost::shared_ptr<shared::xpl::CXplHub> hub;
      if (m_startupOptions.getStartXplHubFlag())
      {
         hub.reset(new shared::xpl::CXplHub(m_startupOptions.getXplNetworkIpAddress()));
         hub->start();
      }

      // Start Xpl Logger
      communication::CXplGateway xplGateway(pDataProvider);
      xplGateway.start();

      // Start Plugin manager
      boost::shared_ptr<pluginSystem::CManager> pluginManager = pluginSystem::CManager::newManager(
         m_startupOptions.getPluginsPath(), pDataProvider->getPluginRequester(), pDataProvider->getPluginEventLoggerRequester(),
         pDataProvider->getEventLoggerRequester(), *this, kPluginManagerEvent);

      // Start Web server
      const std::string webServerIp = m_startupOptions.getWebServerIPAddress();
      const std::string webServerPort = boost::lexical_cast<std::string>(m_startupOptions.getWebServerPortNumber());
      const std::string webServerPath = m_startupOptions.getWebServerInitialPath();
      const std::string webServerWidgetPath = m_startupOptions.getWidgetsPath();

      boost::shared_ptr<web::IWebServer> webServer(new web::webem::CWebServer(webServerIp, webServerPort, webServerPath, "/rest/"));
      webServer->configureAlias("widget", webServerWidgetPath);
      webServer->configureAlias("plugin", m_startupOptions.getPluginsPath());
      boost::shared_ptr<web::IRestHandler> restHanlder = webServer->getRestHandler();
      if(restHanlder.get() != NULL)
      {
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPluginRestService(pDataProvider, pluginManager)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CDeviceRestService(pDataProvider, xplGateway)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPageRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CWidgetRestService(pDataProvider, webServerPath)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CConfigurationRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPluginEventLoggerRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CEventLoggerRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CGeneralRestService()));
      }

      boost::shared_ptr<web::CWebServerManager> webServerManager(new web::CWebServerManager(webServer));
      webServerManager->start();


      // Register to event logger started evebt
      pDataProvider->getEventLoggerRequester()->addEvent(database::entities::kStarted, "yadoms", shared::CStringExtension::EmptyString);

      // Main loop
      YADOMS_LOG(info) << "Supervisor is running...";
      try
      {
         while(true)
         {
            switch(waitForEvents())
            {
            case kPluginManagerEvent:
               pluginManager->signalEvent(getEventData<pluginSystem::CManagerEvent>());
               break;

            case kSystemEvent:
               pDataProvider->getEventLoggerRequester()->addEvent(getEventData<database::entities::CEventLogger>());
               break;

            default:
               YADOMS_LOG(error) << "Unknown message id";
               BOOST_ASSERT(false);
               break;
            }
         }
      }
      catch (boost::thread_interrupted&)
      {
         YADOMS_LOG(info) << "Supervisor is interrupted...";
      }

      //stop all plugins
      if(pluginManager.get() != NULL)
         pluginManager->stop();

      //stop xpl logger
      xplGateway.stop();

      //stop xpl hub
      if(hub.get() != NULL)
         hub->stop();

      //stop web server
      if(webServerManager.get() != NULL)
         webServerManager->stop();

      YADOMS_LOG(info) << "Supervisor is stopped";

      pDataProvider->getEventLoggerRequester()->addEvent(database::entities::kStopped, "yadoms", shared::CStringExtension::EmptyString);

   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception " << e.what();
      if(pDataProvider)
         pDataProvider->getEventLoggerRequester()->addEvent(database::entities::kYadomsCrash, "yadoms",  e.what());
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception.";
      if(pDataProvider)
         pDataProvider->getEventLoggerRequester()->addEvent(database::entities::kYadomsCrash, "yadoms",  "unknwon error");
   }
}
