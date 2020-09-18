#include "stdafx.h"
#include "Supervisor.h"
#include "pluginSystem/Manager.h"
#include "database/Factory.h"
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
#include "web/rest/service/Maintenance.h"
#include <shared/ThreadBase.h>
#include "task/Scheduler.h"
#include "communication/PluginGateway.h"
#include "dataAccessLayer/DataAccessLayer.h"
#include "notification/NotificationCenter.h"
#include "automation/RuleManager.h"
#include <shared/ServiceLocator.h>
#include "startupOptions/IStartupOptions.h"
#include "dateTime/DateTimeNotifier.h"
#include <Poco/Net/NetException.h>
#include "location/Location.h"
#include "location/IpApiAutoLocation.h"
#include "dateTime/TimeZoneProvider.h"
#include "dateTime/TimeZoneDatabase.h"
#include "automation/interpreter/Manager.h"
#include "hardware/usb/DevicesListerFactory.h"
#include "hardware/serial/SerialPortsManagerFactory.h"

CSupervisor::CSupervisor(boost::shared_ptr<const IPathProvider> pathProvider,
                         const shared::versioning::CSemVer& yadomsVersion)
   : m_pathProvider(pathProvider),
     m_yadomsVersion(yadomsVersion)
{
}


void CSupervisor::run()
{
   YADOMS_LOG_CONFIGURE("Supervisor");
   YADOMS_LOG(debug) << "Supervisor is starting";

   boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dal;
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
      auto pDataProvider = databaseFactory->createDataProvider();
      if (!pDataProvider->load())
         throw shared::exception::CException("Fail to load database");

      //create the data access layer
      dal = boost::make_shared<dataAccessLayer::CDataAccessLayer>(pDataProvider);
      shared::CServiceLocator::instance().push<dataAccessLayer::IDataAccessLayer>(dal);

      // Create the location provider
      auto location = boost::make_shared<location::CLocation>(dal->getConfigurationManager(),
                                                              boost::make_shared<location::CIpApiAutoLocation>());
      const auto timezoneDatabase = boost::make_shared<dateTime::CTimeZoneDatabase>();
      const auto timezoneProvider = boost::make_shared<dateTime::CTimeZoneProvider>(dal->getConfigurationManager(),
                                                                                    timezoneDatabase,
                                                                                    "Europe/Paris");

      // Create Task manager
      auto taskManager(boost::make_shared<task::CScheduler>(dal->getEventLogger()));

      // Create the Plugin manager
      auto pluginManager(boost::make_shared<pluginSystem::CManager>(m_pathProvider,
                                                                    m_yadomsVersion,
                                                                    pDataProvider,
                                                                    dal,
                                                                    location,
                                                                    taskManager));

      // Start Task manager
      taskManager->start();

      // Start the plugin gateway
      auto pluginGateway(
         boost::make_shared<communication::CPluginGateway>(pDataProvider, dal->getAcquisitionHistorizer(),
                                                           pluginManager));


      // Start script interpreter manager
      auto scriptInterpreterManager(boost::make_shared<automation::interpreter::CManager>(m_pathProvider));

      // Start automation rules manager
      boost::shared_ptr<automation::IRuleManager> automationRulesManager(boost::make_shared<automation::CRuleManager>(
         scriptInterpreterManager,
         pDataProvider,
         pluginGateway,
         dal->getKeywordManager(),
         dal->getEventLogger(),
         location,
         timezoneProvider));
      shared::CServiceLocator::instance().push<automation::IRuleManager>(automationRulesManager);


      // Create the update manager
      auto updateManager(boost::make_shared<update::CUpdateManager>(taskManager,
                                                                    pluginManager,
                                                                    scriptInterpreterManager,
                                                                    dal->getEventLogger(),
                                                                    startupOptions->getDeveloperMode(),
                                                                    m_pathProvider));


      // Start Web server
      const auto webServerIp = startupOptions->getWebServerIPAddress();
      const auto webServerUseSSL = startupOptions->getIsWebServerUseSSL();
      const auto webServerPort = startupOptions->getWebServerPortNumber();
      const auto securedWebServerPort = startupOptions->getSSLWebServerPortNumber();
      const auto webServerPath = m_pathProvider->webServerPath().string();
      const auto scriptInterpretersPath = m_pathProvider->scriptInterpretersPath().string();
      const auto allowExternalAccess = startupOptions->getWebServerAllowExternalAccess();

      auto usbDeviceLister = hardware::usb::CDevicesListerFactory::createDeviceLister();
      auto serialPortsManager = hardware::serial::CSerialPortsManagerFactory::createSerialPortsManager(usbDeviceLister);

      auto webServer(boost::make_shared<web::poco::CWebServer>(webServerIp, webServerUseSSL, webServerPort,
                                                               securedWebServerPort, webServerPath,
                                                               "/rest/", "/ws", allowExternalAccess));

      webServer->getConfigurator()->websiteHandlerAddAlias("plugins", m_pathProvider->pluginsPath().string());
      webServer->getConfigurator()->websiteHandlerAddAlias("scriptInterpreters", scriptInterpretersPath);
      webServer->getConfigurator()->websiteHandlerAddAlias("backups", m_pathProvider->backupPath().string());

      webServer->getConfigurator()->configureAuthentication(
         boost::make_shared<authentication::CBasicAuthentication>(
            dal->getConfigurationManager(), startupOptions->getNoPasswordFlag()));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CPlugin>(pDataProvider, pluginManager,
                                                         dal->getDeviceManager(), *pluginGateway,
                                                         startupOptions->getDeveloperMode()));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CDevice>(pDataProvider, pluginManager,
                                                         dal->getDeviceManager(), dal->getKeywordManager(),
                                                         *pluginGateway));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CPage>(pDataProvider));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CWidget>(pDataProvider, webServerPath));
      webServer->getConfigurator()->
                 restHandlerRegisterService(
                    boost::make_shared<web::rest::service::CConfiguration>(dal->getConfigurationManager()));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CPluginEventLogger>(pDataProvider));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CEventLogger>(dal->getEventLogger()));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CSystem>(timezoneDatabase,
                                                         usbDeviceLister,
                                                         serialPortsManager));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CAcquisition>(pDataProvider));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CAutomationRule>(pDataProvider, automationRulesManager));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CTask>(taskManager));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CRecipient>(pDataProvider));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CUpdate>(updateManager));
      webServer->getConfigurator()->restHandlerRegisterService(
         boost::make_shared<web::rest::service::CMaintenance>(m_pathProvider,
                                                              pDataProvider->getDatabaseRequester(),
                                                              pDataProvider->getKeywordRequester(),
                                                              pDataProvider->getAcquisitionRequester(),
                                                              taskManager));

      webServer->start();

      // Start the plugin manager (start all plugin instances)
      pluginManager->start(boost::posix_time::minutes(2));

      //start the rule manager
      automationRulesManager->start();

      //create and start the dateTime notification scheduler
      dateTime::CDateTimeNotifier dateTimeNotificationService;
      dateTimeNotificationService.start();

      // Register to event logger started event
      dal->getEventLogger()->addEvent(database::entities::ESystemEventCode::kStarted, "yadoms", std::string());

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
      webServer->stop();
      webServer.reset();

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
      pDataProvider->stopMaintenanceTasks();

      dal->getEventLogger()->addEvent(database::entities::ESystemEventCode::kStopped, "yadoms", std::string());
   }
   catch (Poco::Net::NetException& pe)
   {
      YADOMS_LOG(error) << "Supervisor : net exception " << pe.displayText();
      if (dal && dal->getEventLogger())
         dal->getEventLogger()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms",
                                         pe.displayText());
   }
   catch (Poco::Exception& e)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception " << e.displayText();
      if (dal && dal->getEventLogger())
         dal->getEventLogger()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms", e.displayText());
   }
   catch (shared::exception::CException& ex)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled shared::exception::CException " << ex.what();
      if (dal && dal->getEventLogger())
         dal->getEventLogger()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms", ex.what());
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
         dal->getEventLogger()->addEvent(database::entities::ESystemEventCode::kYadomsCrash, "yadoms", "unknown error");
   }

   //notify application that supervisor ends
   YADOMS_LOG(information) << "Supervisor is stopped";
}

void CSupervisor::requestToStop()
{
   m_eventHandler.postEvent(kStopRequested);
}
