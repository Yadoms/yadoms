#include "stdafx.h"
#include "Supervisor.h"
#include "pluginSystem/Manager.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/DatabaseException.hpp"
#include <shared/Log.h>
#include <shared/exception/NotSupported.hpp>
#include "web/poco/WebServer.h"
#include "web/rest/service/Acquisition.h"
#include "web/rest/service/AutomationRule.h"
#include "web/rest/service/Plugin.h"
#include "web/rest/service/Device.h"
#include "web/rest/service/Page.h"
#include "web/rest/service/Widget.h"
#include "web/rest/service/Configuration.h"
#include "web/rest/service/PluginEventLogger.h"
#include "web/rest/service/EventLogger.h"
#include "web/rest/service/System.h"
#include "web/rest/service/Task.h"
#include <shared/ThreadBase.h>
#include <shared/Peripherals.h>
#include "tools/web/FileDownloader.h"
#include "task/Scheduler.h"
#include "task/update/Plugin.h"
#include "task/backup/Database.h"
#include "communication/PluginGateway.h"
#include "SystemInformation.h"
#include "dataAccessLayer/DataAccessLayer.h"
#include <shared/notification/NotificationCenter.h>
#include "automation/RuleManager.h"

CSupervisor::CSupervisor(const startupOptions::IStartupOptions& startupOptions)
   :m_EventHandler(new shared::event::CEventHandler), m_stopHandler(m_EventHandler, kStopRequested), m_startupOptions(startupOptions)
{
}

CSupervisor::~CSupervisor()
{
}

void CSupervisor::doWork()
{
   YADOMS_LOG_CONFIGURE("Supervisor");
   YADOMS_LOG(information) << "Supervisor is starting";   

   boost::shared_ptr<database::IDataProvider> pDataProvider;
   try
   {
      //create the system information
      boost::shared_ptr<CSystemInformation> systemInformation(new CSystemInformation());

      //create the notification center
      boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter(new shared::notification::CNotificationCenter);

      //start database system
      pDataProvider.reset(new database::sqlite::CSQLiteDataProvider(m_startupOptions.getDatabaseFile()));
      if (!pDataProvider->load())
      {
         throw shared::exception::CException("Fail to load database");
      }

      //create the data access layer
      boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dal(new dataAccessLayer::CDataAccessLayer(pDataProvider, notificationCenter));

      // Start Task manager
      boost::shared_ptr<task::CScheduler> taskManager(new task::CScheduler(m_EventHandler, kSystemEvent));
      taskManager->start();

      // Create the Plugin manager
      boost::shared_ptr<pluginSystem::CManager> pluginManager(new pluginSystem::CManager(
         m_startupOptions.getPluginsPath(), pDataProvider, dal, m_EventHandler, kPluginManagerEvent, m_stopHandler));

      // Start the plugin gateway
      boost::shared_ptr<communication::CPluginGateway> pluginGateway(new communication::CPluginGateway(pDataProvider, dal->getAcquisitionHistorizer(), pluginManager));

      // Start the plugin manager (start all plugin instances)
      pluginManager->start();

      // Start automation rules manager
      boost::shared_ptr<automation::IRuleManager> automationRulesManager(new automation::CRuleManager(pDataProvider->getRuleRequester(), pluginGateway, notificationCenter, pDataProvider->getAcquisitionRequester()));
      automationRulesManager->start();

      // Start Web server
      const std::string & webServerIp = m_startupOptions.getWebServerIPAddress();
      const std::string webServerPort = boost::lexical_cast<std::string>(m_startupOptions.getWebServerPortNumber());
      const std::string & webServerPath = m_startupOptions.getWebServerInitialPath();

      web::poco::CWebServer webServer(webServerIp, webServerPort, webServerPath, "/rest/", "/ws", notificationCenter);

      //TODO delete unused alias when widget directory will be no more movable
      //const std::string & webServerWidgetPath = m_startupOptions.getWidgetsPath();
      //webServer.configureAlias("widgets", webServerWidgetPath);
      webServer.getConfigurator()->websiteHandlerAddAlias("plugins", m_startupOptions.getPluginsPath());
      webServer.getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPlugin(pDataProvider, pluginManager, *pluginGateway)));
      webServer.getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CDevice(pDataProvider, *pluginGateway)));
      webServer.getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPage(pDataProvider)));
      webServer.getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CWidget(pDataProvider, webServerPath)));
      webServer.getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CConfiguration(pDataProvider)));
      webServer.getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPluginEventLogger(pDataProvider)));
      webServer.getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CEventLogger(pDataProvider)));
      webServer.getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CSystem(systemInformation)));
      webServer.getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CAcquisition(pDataProvider)));
      webServer.getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CAutomationRule(pDataProvider, automationRulesManager)));
      webServer.getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CTask(taskManager)));

      webServer.start();

      // Register to event logger started event
      pDataProvider->getEventLoggerRequester()->addEvent(database::entities::ESystemEventCode::kStarted, "yadoms", shared::CStringExtension::EmptyString);

      // Main loop
      YADOMS_LOG(information) << "Supervisor is running...";
      bool running = true;
      while(running)
      {
         switch(m_EventHandler->waitForEvents())
         {
         case kStopRequested:
            running = false;
            break;

         case kPluginManagerEvent:
            pluginManager->signalEvent(m_EventHandler->getEventData<pluginSystem::CManagerEvent>());
            break;

         case kSystemEvent:
            pDataProvider->getEventLoggerRequester()->addEvent(m_EventHandler->getEventData<database::entities::CEventLogger>());
            break;

         default:
            YADOMS_LOG(error) << "Unknown message id";
            BOOST_ASSERT(false);
            break;
         }
      }

      YADOMS_LOG(information) << "Supervisor is stopping...";

      //stop the automation rules
      automationRulesManager->stop();

      //stop all plugins
      if(pluginManager.get() != NULL)
         pluginManager->stop();

      //stop web server
      webServer.stop();

      YADOMS_LOG(information) << "Supervisor is stopped";

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
   m_EventHandler->postEvent(kStopRequested);
}

IApplicationStopHandler::EStopMode CSupervisor::stopMode() const
{
   return m_stopHandler.stopMode();
}
