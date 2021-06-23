#include "stdafx.h"
#include "Supervisor.h"
#include <Poco/Net/NetException.h>
#include <shared/Log.h>
#include <shared/ServiceLocator.h>
#include <shared/ThreadBase.h>
#include <utility>
#include "authentication/BasicAuthentication.h"
#include "automation/interpreter/Manager.h"
#include "automation/RuleManager.h"
#include "communication/PluginGateway.h"
#include "dataAccessLayer/DataAccessLayer.h"
#include "database/Factory.h"
#include "dateTime/DateTimeNotifier.h"
#include "dateTime/TimeZoneDatabase.h"
#include "dateTime/TimeZoneProvider.h"
#include "hardware/usb/DevicesListerFactory.h"
#include "location/IpApiAutoLocation.h"
#include "location/Location.h"
#include "notification/NotificationCenter.h"
#include "pluginSystem/Manager.h"
#include "task/Scheduler.h"
#include "update/UpdateManager.h"
#include "web/poco/WebServer.h"
#include "web/rest/service/Acquisition.h"
#include "web/rest/service/AutomationRule.h"
#include "web/rest/service/Configuration.h"
#include "web/rest/service/Device.h"
#include "web/rest/service/EventLogger.h"
#include "web/rest/service/Maintenance.h"
#include "web/rest/service/Page.h"
#include "web/rest/service/Plugin.h"
#include "web/rest/service/PluginEventLogger.h"
#include "web/rest/service/Recipient.h"
#include "web/rest/service/System.h"
#include "web/rest/service/Task.h"
#include "web/rest/service/Update.h"
#include "web/rest/service/UploadFileManager.h"
#include "web/rest/service/Widget.h"
#include "web/oatppServer/WebServer.h"

CSupervisor::CSupervisor(boost::shared_ptr<const IPathProvider> pathProvider,
                         const shared::versioning::CSemVer& yadomsVersion)
   : m_pathProvider(std::move(pathProvider)),
     m_yadomsVersion(yadomsVersion)
{
}


void CSupervisor::run()
{
   YADOMS_LOG_CONFIGURE("Supervisor")
   YADOMS_LOG(debug) << "Supervisor is starting";

   boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer;
   try
   {
      //create the notification center
      const auto notificationCenter(boost::make_shared<notification::CNotificationCenter>());
      shared::CServiceLocator::instance().push<notification::CNotificationCenter>(notificationCenter);

      //retrieve startup options
      auto startupOptions = shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>();

      //start database system
      const auto databaseFactory = boost::make_shared<database::CFactory>(m_pathProvider,
                                                                          startupOptions);
      auto dataProvider = databaseFactory->createDataProvider();
      if (!dataProvider->load())
         throw shared::exception::CException("Fail to load database");

      //create the data access layer
      dataAccessLayer = boost::make_shared<dataAccessLayer::CDataAccessLayer>(dataProvider);
      shared::CServiceLocator::instance().push<dataAccessLayer::IDataAccessLayer>(dataAccessLayer);

      // Create the location provider
      auto location = boost::make_shared<location::CLocation>(dataAccessLayer->getConfigurationManager(),
                                                              boost::make_shared<location::CIpApiAutoLocation>());
      const auto timezoneDatabase = boost::make_shared<dateTime::CTimeZoneDatabase>();
      const auto timezoneProvider = boost::make_shared<dateTime::CTimeZoneProvider>(dataAccessLayer->getConfigurationManager(),
                                                                                    timezoneDatabase,
                                                                                    "Europe/Paris");

      // Create Task manager
      auto taskManager(boost::make_shared<task::CScheduler>(dataAccessLayer->getEventLogger()));

      // Create the Plugin manager
      auto pluginManager(boost::make_shared<pluginSystem::CManager>(m_pathProvider,
                                                                    m_yadomsVersion,
                                                                    dataProvider,
                                                                    dataAccessLayer,
                                                                    location,
                                                                    taskManager));

      // Start Task manager
      taskManager->start();

      // Start the plugin gateway
      auto pluginGateway(boost::make_shared<communication::CPluginGateway>(dataProvider,
                                                                           dataAccessLayer->getAcquisitionHistorizer(),
                                                                           pluginManager));


      // Start script interpreter manager
      auto scriptInterpreterManager(boost::make_shared<automation::interpreter::CManager>(m_pathProvider));

      // Start automation rules manager
      boost::shared_ptr<automation::IRuleManager> automationRulesManager(boost::make_shared<automation::CRuleManager>(
         scriptInterpreterManager,
         dataProvider,
         pluginGateway,
         dataAccessLayer->getKeywordManager(),
         dataAccessLayer->getEventLogger(),
         location,
         timezoneProvider));
      shared::CServiceLocator::instance().push<automation::IRuleManager>(automationRulesManager);


      // Create the update manager
      const auto updateManager(boost::make_shared<update::CUpdateManager>(taskManager,
                                                                          pluginManager,
                                                                          scriptInterpreterManager,
                                                                          dataAccessLayer->getEventLogger(),
                                                                          startupOptions->getDeveloperMode(),
                                                                          m_pathProvider));

      // Start Web servers
      auto pocoBasedWebServer = createPocoBasedWebServer(startupOptions,
                                                         dataAccessLayer,
                                                         dataProvider,
                                                         pluginManager,
                                                         pluginGateway,
                                                         timezoneDatabase,
                                                         automationRulesManager,
                                                         updateManager,
                                                         taskManager);
      auto oatppBasedWebServer = createOatppBasedWebServer(startupOptions,
                                                           dataAccessLayer,
                                                           dataProvider,
                                                           pluginManager,
                                                           pluginGateway,
                                                           timezoneDatabase,
                                                           automationRulesManager,
                                                           updateManager,
                                                           taskManager);

      pocoBasedWebServer->start();
      oatppBasedWebServer->start();

      // Start the plugin manager (start all plugin instances)
      pluginManager->start(boost::posix_time::minutes(2));

      //start the rule manager
      automationRulesManager->start();

      //create and start the dateTime notification scheduler
      dateTime::CDateTimeNotifier dateTimeNotificationService;
      dateTimeNotificationService.start();

      // Register to event logger started event
      dataAccessLayer->getEventLogger()->addEvent(database::entities::ESystemEventCode::kStarted, "yadoms", std::string());

      //update the server state
      shared::CServiceLocator::instance().get<IRunningInformation>()->setServerFullyLoaded();

      // Main loop
      YADOMS_LOG(information) << "Supervisor is running...";
      while (m_eventHandler.waitForEvents() != kStopRequested)
      {
      }

      YADOMS_LOG(debug) << "Supervisor is stopping...";

      //stop datetime notification service
      dateTimeNotificationService.stop();

      //stop web server
      pocoBasedWebServer->stop();
      oatppBasedWebServer->stop();
      pocoBasedWebServer.reset();
      oatppBasedWebServer.reset();

      //stop the automation rules
      shared::CServiceLocator::instance().remove<automation::IRuleManager>(automationRulesManager);
      automationRulesManager->stop();
      automationRulesManager.reset();

      //stop task manager
      taskManager->stop();
      taskManager.reset();

      //stop all plugins
      //force to stop all plugin, the reset() will call stop only at pluginManager deletion, which could happen in the future if it is used elsewhere
      pluginManager->stop();
      pluginManager.reset();

      //stop database tasks
      dataProvider->stopMaintenanceTasks();

      dataAccessLayer->getEventLogger()->addEvent(database::entities::ESystemEventCode::kStopped, "yadoms", std::string());
   }
   catch (Poco::Net::NetException& pe)
   {
      YADOMS_LOG(error) << "Supervisor : net exception " << pe.displayText();
      if (dataAccessLayer && dataAccessLayer->getEventLogger())
         dataAccessLayer->getEventLogger()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms",
                                                     pe.displayText());
   }
   catch (Poco::Exception& e)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception " << e.displayText();
      if (dataAccessLayer && dataAccessLayer->getEventLogger())
         dataAccessLayer->getEventLogger()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms", e.displayText());
   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled shared::exception::CException " << ex.what();
      if (dataAccessLayer && dataAccessLayer->getEventLogger())
         dataAccessLayer->getEventLogger()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms", ex.what());
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception " << e.what();
      if (dataAccessLayer && dataAccessLayer->getEventLogger())
         dataAccessLayer->getEventLogger()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms", e.what());
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception.";
      if (dataAccessLayer && dataAccessLayer->getEventLogger())
         dataAccessLayer->getEventLogger()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms", "unknown error");
   }

   //notify application that supervisor ends
   YADOMS_LOG(information) << "Supervisor is stopped";
}

void CSupervisor::requestToStop()
{
   m_eventHandler.postEvent(kStopRequested);
}

boost::shared_ptr<web::IWebServer> CSupervisor::createPocoBasedWebServer(
   const boost::shared_ptr<const startupOptions::IStartupOptions>& startupOptions,
   const boost::shared_ptr<dataAccessLayer::IDataAccessLayer>& dataAccessLayer,
   boost::shared_ptr<database::IDataProvider> dataProvider,
   boost::shared_ptr<pluginSystem::CManager> pluginManager,
   const boost::shared_ptr<communication::CPluginGateway>& pluginGateway,
   boost::shared_ptr<dateTime::CTimeZoneDatabase> timezoneDatabase,
   boost::shared_ptr<automation::IRuleManager> automationRulesManager,
   boost::shared_ptr<update::IUpdateManager> updateManager,
   boost::shared_ptr<task::CScheduler> taskManager) const
{
   const auto webServerIp = startupOptions->getWebServerIPAddress();
   const auto webServerUseSsl = startupOptions->getIsWebServerUseSSL();
   const auto webServerPort = startupOptions->getWebServerPortNumber();
   const auto securedWebServerPort = startupOptions->getSSLWebServerPortNumber();
   const auto webServerPath = m_pathProvider->webServerPath().string();
   const auto scriptInterpretersPath = m_pathProvider->scriptInterpretersPath().string();
   const auto allowExternalAccess = startupOptions->getWebServerAllowExternalAccess();

   boost::shared_ptr<web::IWebServer> webServer(boost::make_shared<web::poco::CWebServer>(webServerIp,
                                                                                          webServerUseSsl,
                                                                                          webServerPort,
                                                                                          securedWebServerPort,
                                                                                          webServerPath,
                                                                                          "/rest/",
                                                                                          "/ws",
                                                                                          allowExternalAccess));

   webServer->getConfigurator()->websiteHandlerAddAlias("plugins", m_pathProvider->pluginsPath().string());
   webServer->getConfigurator()->websiteHandlerAddAlias("scriptInterpreters", scriptInterpretersPath);
   webServer->getConfigurator()->websiteHandlerAddAlias("backups", m_pathProvider->backupPath().string());

   webServer->getConfigurator()->configureAuthentication(
      boost::make_shared<authentication::CBasicAuthentication>(dataAccessLayer->getConfigurationManager(),
                                                               startupOptions->getNoPasswordFlag()));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CPlugin>(dataProvider,
                                                      pluginManager,
                                                      dataAccessLayer->getDeviceManager(),
                                                      *pluginGateway,
                                                      startupOptions->getDeveloperMode()));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CDevice>(dataProvider,
                                                      pluginManager,
                                                      dataAccessLayer->getDeviceManager(),
                                                      dataAccessLayer->getKeywordManager(),
                                                      *pluginGateway));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CPage>(dataProvider));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CWidget>(dataProvider, webServerPath));
   webServer->getConfigurator()->
              restHandlerRegisterService(
                 boost::make_shared<web::rest::service::CConfiguration>(dataAccessLayer->getConfigurationManager()));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CPluginEventLogger>(dataProvider));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CEventLogger>(dataAccessLayer->getEventLogger()));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CSystem>(timezoneDatabase,
                                                      hardware::usb::CDevicesListerFactory::createDeviceLister()));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CAcquisition>(dataProvider));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CAutomationRule>(dataProvider,
                                                              automationRulesManager));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CTask>(taskManager));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CRecipient>(dataProvider));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CUpdate>(updateManager));
   webServer->getConfigurator()->restHandlerRegisterService(
      boost::make_shared<web::rest::service::CMaintenance>(m_pathProvider,
                                                           dataProvider,
                                                           taskManager,
                                                           boost::make_shared<web::rest::service::CUploadFileManager>()));

   return webServer;
}

boost::shared_ptr<web::IWebServer> CSupervisor::createOatppBasedWebServer(
   const boost::shared_ptr<const startupOptions::IStartupOptions>& startupOptions,
   const boost::shared_ptr<dataAccessLayer::IDataAccessLayer>& dataAccessLayer,
   boost::shared_ptr<database::IDataProvider> dataProvider,
   boost::shared_ptr<pluginSystem::CManager> pluginManager,
   const boost::shared_ptr<communication::CPluginGateway>& pluginGateway,
   boost::shared_ptr<dateTime::CTimeZoneDatabase> timezoneDatabase,
   boost::shared_ptr<automation::IRuleManager> automationRulesManager,
   boost::shared_ptr<update::IUpdateManager> updateManager,
   boost::shared_ptr<task::CScheduler> taskManager) const
{
   const auto webServerIp = startupOptions->getWebServerIPAddress();
   const auto webServerUseSsl = startupOptions->getIsWebServerUseSSL();
   const auto webServerPort = startupOptions->getWebServerPortNumber();
   const auto securedWebServerPort = startupOptions->getSSLWebServerPortNumber();
   const auto webServerPath = m_pathProvider->webServerPath().string();
   const auto scriptInterpretersPath = m_pathProvider->scriptInterpretersPath().string();
   const auto allowExternalAccess = startupOptions->getWebServerAllowExternalAccess();

   auto restServices = boost::make_shared<std::vector<boost::shared_ptr<web::rest::service::IRestService>>>();
   restServices->push_back(boost::make_shared<web::rest::service::CPlugin>(dataProvider,
                                                                           pluginManager,
                                                                           dataAccessLayer->getDeviceManager(),
                                                                           *pluginGateway,
                                                                           startupOptions->getDeveloperMode()));
   restServices->push_back(boost::make_shared<web::rest::service::CDevice>(dataProvider,
                                                                           pluginManager,
                                                                           dataAccessLayer->getDeviceManager(),
                                                                           dataAccessLayer->getKeywordManager(),
                                                                           *pluginGateway));
   restServices->push_back(boost::make_shared<web::rest::service::CPage>(dataProvider));
   restServices->push_back(boost::make_shared<web::rest::service::CWidget>(dataProvider, webServerPath));
   restServices->push_back(boost::make_shared<web::rest::service::CConfiguration>(dataAccessLayer->getConfigurationManager()));
   restServices->push_back(boost::make_shared<web::rest::service::CPluginEventLogger>(dataProvider));
   restServices->push_back(boost::make_shared<web::rest::service::CEventLogger>(dataAccessLayer->getEventLogger()));
   restServices->push_back(boost::make_shared<web::rest::service::CSystem>(timezoneDatabase,
                                                                           hardware::usb::CDevicesListerFactory::createDeviceLister()));
   restServices->push_back(boost::make_shared<web::rest::service::CAcquisition>(dataProvider));
   restServices->push_back(boost::make_shared<web::rest::service::CAutomationRule>(dataProvider,
                                                                                   automationRulesManager));
   restServices->push_back(boost::make_shared<web::rest::service::CTask>(taskManager));
   restServices->push_back(boost::make_shared<web::rest::service::CRecipient>(dataProvider));
   restServices->push_back(boost::make_shared<web::rest::service::CUpdate>(updateManager));
   restServices->push_back(boost::make_shared<web::rest::service::CMaintenance>(m_pathProvider,
                                                                                dataProvider,
                                                                                taskManager,
                                                                                boost::make_shared<web::rest::service::CUploadFileManager>()));

   boost::shared_ptr<web::IWebServer> webServer(boost::make_shared<web::oatppServer::CWebServer>(webServerIp,
                                                                                                 webServerUseSsl,
                                                                                                 webServerPort + 1, //TODO virer le +1 (pour test...)
                                                                                                 securedWebServerPort + 1,
                                                                                                 //TODO virer le +1 (pour test...)
                                                                                                 webServerPath,
                                                                                                 "/rest/",
                                                                                                 restServices,
                                                                                                 "/ws",
                                                                                                 allowExternalAccess));

   webServer->getConfigurator()->websiteHandlerAddAlias("plugins", m_pathProvider->pluginsPath().string());
   webServer->getConfigurator()->websiteHandlerAddAlias("scriptInterpreters", scriptInterpretersPath);
   webServer->getConfigurator()->websiteHandlerAddAlias("backups", m_pathProvider->backupPath().string());

   webServer->getConfigurator()->configureAuthentication(
      boost::make_shared<authentication::CBasicAuthentication>(dataAccessLayer->getConfigurationManager(),
                                                               startupOptions->getNoPasswordFlag()));

   return webServer;
}
