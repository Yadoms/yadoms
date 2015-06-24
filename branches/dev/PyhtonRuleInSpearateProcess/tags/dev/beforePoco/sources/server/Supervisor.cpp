﻿#include "stdafx.h"
#include "Supervisor.h"
#include "pluginSystem/Manager.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/DatabaseException.hpp"
#include <shared/Log.h>
#include <shared/exception/NotSupported.hpp>
#include "web/webem/WebServer.h"
#include "web/WebServerManager.h"
#include "web/rest/service/Acquisition.h"
#include "web/rest/service/Plugin.h"
#include "web/rest/service/Device.h"
#include "web/rest/service/Page.h"
#include "web/rest/service/Widget.h"
#include "web/rest/service/Configuration.h"
#include "web/rest/service/PluginEventLogger.h"
#include "web/rest/service/EventLogger.h"
#include "web/rest/service/General.h"
#include "web/rest/service/Task.h"
#include <shared/ThreadBase.h>
#include <shared/Peripherals.h>
#include "tools/web/FileDownloader.h"
#include "task/Scheduler.h"
#include "task/update/Plugin.h"
#include "task/backup/Database.h"
#include "communication/PluginGateway.h"
#include "System.h"

CSupervisor::CSupervisor(const startupOptions::IStartupOptions& startupOptions)
   :m_stopHandler(m_EventHandler, kStopRequested), m_startupOptions(startupOptions)
{
}


CSupervisor::~CSupervisor()
{
}

void CSupervisor::doWork()
{
   YADOMS_LOG_CONFIGURE("Supervisor");
   YADOMS_LOG(info) << "Supervisor is starting";   

   boost::shared_ptr<database::IDataProvider> pDataProvider;
   try
   {
      //create the system information
      boost::shared_ptr<CSystem> systemInformation(new CSystem());

      //start database system
      pDataProvider.reset(new database::sqlite::CSQLiteDataProvider(m_startupOptions.getDatabaseFile()));
      if (!pDataProvider->load())
      {
         throw shared::exception::CException("Fail to load database");
      }

      // Start Task manager
      boost::shared_ptr<task::CScheduler> taskManager = boost::shared_ptr<task::CScheduler>(new task::CScheduler(m_EventHandler, kSystemEvent));
      taskManager->start();
      // Create the Plugin manager
      boost::shared_ptr<pluginSystem::CManager> pluginManager(new pluginSystem::CManager(
         m_startupOptions.getPluginsPath(), pDataProvider, m_EventHandler, kPluginManagerEvent));

      // Start the plugin gateway
      communication::CPluginGateway pluginGateway(pDataProvider, pluginManager);
      pluginGateway.start();

      // Start the plugin manager (start all plugin instances)
      pluginManager->start();

      // Start Web server
      const std::string & webServerIp = m_startupOptions.getWebServerIPAddress();
      const std::string webServerPort = boost::lexical_cast<std::string>(m_startupOptions.getWebServerPortNumber());
      const std::string & webServerPath = m_startupOptions.getWebServerInitialPath();
      const std::string & webServerWidgetPath = m_startupOptions.getWidgetsPath();

      boost::shared_ptr<web::IWebServer> webServer(new web::webem::CWebServer(webServerIp, webServerPort, webServerPath, "/rest/"));
      //TODO delete unused alias when widget directory will be no more movable
      //webServer->configureAlias("widgets", webServerWidgetPath);
      webServer->configureAlias("plugins", m_startupOptions.getPluginsPath());
      boost::shared_ptr<web::IRestHandler> restHanlder = webServer->getRestHandler();
      if(restHanlder.get() != NULL)
      {
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPlugin(pDataProvider, pluginManager)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CDevice(pDataProvider, pluginGateway)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPage(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CWidget(pDataProvider, webServerPath)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CConfiguration(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPluginEventLogger(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CEventLogger(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CGeneral(systemInformation)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CAcquisition(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CAcquisition(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CAcquisition(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CTask(taskManager)));
      }

      boost::shared_ptr<web::CWebServerManager> webServerManager(new web::CWebServerManager(webServer));
      webServerManager->start();


      // Register to event logger started evebt
      pDataProvider->getEventLoggerRequester()->addEvent(database::entities::ESystemEventCode::kStarted, "yadoms", shared::CStringExtension::EmptyString);

      // Main loop
      YADOMS_LOG(info) << "Supervisor is running...";
      bool running = true;
      while(running)
      {
         switch(m_EventHandler.waitForEvents())
         {
         case kStopRequested:
            running = false;
            break;

         case kPluginManagerEvent:
            pluginManager->signalEvent(m_EventHandler.getEventData<pluginSystem::CManagerEvent>());
            break;

         case kSystemEvent:
            pDataProvider->getEventLoggerRequester()->addEvent(m_EventHandler.getEventData<database::entities::CEventLogger>());
            break;

         default:
            YADOMS_LOG(error) << "Unknown message id";
            BOOST_ASSERT(false);
            break;
         }
      }

      YADOMS_LOG(info) << "Supervisor is stopping...";

      //stop all plugins
      if(pluginManager.get() != NULL)
         pluginManager->stop();


      //stop web server
      if(webServerManager.get() != NULL)
         webServerManager->stop();

      YADOMS_LOG(info) << "Supervisor is stopped";

      pDataProvider->getEventLoggerRequester()->addEvent(database::entities::ESystemEventCode::kStopped, "yadoms", shared::CStringExtension::EmptyString);

      //if the application need to notify its stop (mainly in case of service)
      if (m_callbackAfterStopped)
         m_callbackAfterStopped();
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception " << e.what();
      if(pDataProvider)
         pDataProvider->getEventLoggerRequester()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms", e.what());
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception.";
      if(pDataProvider)
         pDataProvider->getEventLoggerRequester()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms", "unknwon error");
   }
}


void CSupervisor::requestToStop(boost::function<void()> & callbackAfterStopped)
{
   m_callbackAfterStopped = callbackAfterStopped;
   m_EventHandler.postEvent(kStopRequested);
}


