#include "stdafx.h"
#include "Supervisor.h"
#include "automation/script/ObjectFactory.h"
#include "pluginSystem/Manager.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include <shared/Log.h>
#include "web/poco/WebServer.h"
#include "authentication/BasicAuthentication.h"
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
#include "web/rest/service/Recipient.h"
#include "web/rest/service/Update.h"
#include <shared/ThreadBase.h>
#include "task/Scheduler.h"
#include "communication/PluginGateway.h"
#include "dataAccessLayer/DataAccessLayer.h"
#include "notification/NotificationCenter.h"
#include "automation/RuleManager.h"
#include <shared/ServiceLocator.h>
#include "startupOptions/IStartupOptions.h"
#include "Version.h"

CSupervisor::CSupervisor(boost::shared_ptr<shared::event::CEventHandler> applicationEventHandler, const int applicationStopCode)
   :m_EventHandler(new shared::event::CEventHandler), m_applicationEventHandler(applicationEventHandler), m_applicationStopCode(applicationStopCode)
{
}

CSupervisor::~CSupervisor()
{
}


void CSupervisor::run()
{
   YADOMS_LOG_CONFIGURE("Supervisor");
   YADOMS_LOG(information) << "Supervisor is starting";

   boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dal;
   try
   {
      shared::CServiceLocator::instance().push<automation::script::IObjectFactory>(boost::make_shared<automation::script::ObjectFactory>());

      //create the notification center
      boost::shared_ptr<notification::CNotificationCenter> notificationCenter(new notification::CNotificationCenter);
      shared::CServiceLocator::instance().push<notification::CNotificationCenter>(notificationCenter);

      //retreive startup options
      boost::shared_ptr<startupOptions::IStartupOptions> startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();

      //start database system
      boost::shared_ptr<database::IDataProvider> pDataProvider(new database::sqlite::CSQLiteDataProvider(startupOptions->getDatabaseFile()));
      if (!pDataProvider->load())
         throw shared::exception::CException("Fail to load database");

      //create the data access layer
      dal.reset(new dataAccessLayer::CDataAccessLayer(pDataProvider));
      shared::CServiceLocator::instance().push<dataAccessLayer::IDataAccessLayer>(dal);

      // Start Task manager
      boost::shared_ptr<task::CScheduler> taskManager(new task::CScheduler(m_EventHandler, kSystemEvent));
      taskManager->start();

      // Create the update manager
      boost::shared_ptr<update::CUpdateManager> updateManager(new update::CUpdateManager(taskManager));

      // Create the Plugin manager
      const std::string pluginsPath = startupOptions->getPluginsPath();
      boost::shared_ptr<pluginSystem::CManager> pluginManager(new pluginSystem::CManager(
         pluginsPath, pDataProvider, dal, m_EventHandler, kPluginManagerEvent));
      shared::CServiceLocator::instance().push<pluginSystem::CManager>(pluginManager);

      // Start the plugin gateway
      boost::shared_ptr<communication::CPluginGateway> pluginGateway(new communication::CPluginGateway(pDataProvider, dal->getAcquisitionHistorizer(), pluginManager));

      // Start the plugin manager (start all plugin instances)
      pluginManager->start();

      // Start automation rules manager
      boost::shared_ptr<automation::IRuleManager> automationRulesManager(new automation::CRuleManager(
         pDataProvider->getRuleRequester(), pluginGateway,
         pDataProvider->getAcquisitionRequester(), pDataProvider->getDeviceRequester(), pDataProvider->getKeywordRequester(), pDataProvider->getRecipientRequester(),
         dal->getConfigurationManager(), dal->getEventLogger(), m_EventHandler, kRuleManagerEvent));
      shared::CServiceLocator::instance().push<automation::IRuleManager>(automationRulesManager);

      // Start Web server
      const std::string & webServerIp = startupOptions->getWebServerIPAddress();
      const std::string webServerPort = boost::lexical_cast<std::string>(startupOptions->getWebServerPortNumber());
      const std::string & webServerPath = startupOptions->getWebServerInitialPath();
      const std::string scriptInterpretersPath = startupOptions->getScriptInterpretersPath();

      boost::shared_ptr<web::poco::CWebServer> webServer(new web::poco::CWebServer(webServerIp, webServerPort, webServerPath, "/rest/", "/ws"));
      webServer->getConfigurator()->websiteHandlerAddAlias("plugins", pluginsPath);
      webServer->getConfigurator()->websiteHandlerAddAlias("scriptInterpreters", scriptInterpretersPath);
      webServer->getConfigurator()->configureAuthentication(boost::shared_ptr<authentication::IAuthentication>(new authentication::CBasicAuthentication(dal->getConfigurationManager(), startupOptions->getNoPasswordFlag())));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPlugin(pDataProvider, pluginManager, *pluginGateway)));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CDevice(pDataProvider, *pluginGateway)));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPage(pDataProvider)));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CWidget(pDataProvider, webServerPath)));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CConfiguration(dal->getConfigurationManager())));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPluginEventLogger(pDataProvider)));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CEventLogger(dal->getEventLogger())));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CSystem()));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CAcquisition(pDataProvider)));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CAutomationRule(pDataProvider, automationRulesManager)));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CTask(taskManager)));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CRecipient(pDataProvider)));
      webServer->getConfigurator()->restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CUpdate(updateManager)));

      webServer->start();

      // Register to event logger started event
      dal->getEventLogger()->addEvent(database::entities::ESystemEventCode::kStarted, "yadoms", shared::CStringExtension::EmptyString);

      // Main loop
      YADOMS_LOG(information) << "Supervisor is running...";
      bool stopIsRequested = false;
      while (!stopIsRequested)
      {
         switch (m_EventHandler->waitForEvents())
         {
         case kStopRequested:
            stopIsRequested = true;
            break;

         case kPluginManagerEvent:
            pluginManager->signalEvent(m_EventHandler->getEventData<pluginSystem::CManagerEvent>());
            break;

         case kRuleManagerEvent:
            automationRulesManager->signalEvent(m_EventHandler->getEventData<automation::CManagerEvent>());
            break;

         case kSystemEvent:
            dal->getEventLogger()->addEvent(m_EventHandler->getEventData<database::entities::CEventLogger>());
            break;

         default:
            YADOMS_LOG(error) << "Unknown message id";
            BOOST_ASSERT(false);
            break;
         }
      }

      YADOMS_LOG(information) << "Supervisor is stopping...";

      //stop web server
      webServer.reset();

      //stop the automation rules
      shared::CServiceLocator::instance().remove<automation::IRuleManager>(automationRulesManager);
      automationRulesManager.reset();

      //stop task manager
      taskManager.reset();

      //stop all plugins
      shared::CServiceLocator::instance().remove<pluginSystem::CManager>(pluginManager);
      pluginManager.reset();

      YADOMS_LOG(information) << "Supervisor is stopped";

      dal->getEventLogger()->addEvent(database::entities::ESystemEventCode::kStopped, "yadoms", shared::CStringExtension::EmptyString);
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception " << e.what();
      if (dal && dal->getEventLogger())
         dal->getEventLogger()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms", e.what());
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception.";
      if (dal && dal->getEventLogger())
         dal->getEventLogger()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms", "unknwon error");
   }

   //notify application that supervisor ends
   if (m_applicationEventHandler)
      m_applicationEventHandler->postEvent(m_applicationStopCode);
}

void CSupervisor::requestToStop()
{
   if (m_EventHandler)
      m_EventHandler->postEvent(kStopRequested);
}
