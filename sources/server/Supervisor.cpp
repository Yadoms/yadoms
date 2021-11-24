#include "stdafx.h"
#include "Supervisor.h"
#include <Poco/Net/NetException.h>
#include <shared/Log.h>
#include <shared/ServiceLocator.h>
#include <shared/ThreadBase.h>
#include <utility>
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
#include "web/oatppServer/Authentication.h"
#include "web/poco/BasicAuthentication.h"
#include "web/rest/service/Keyword.h"

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
      const auto restServices = boost::make_shared<std::vector<boost::shared_ptr<web::rest::service::IRestService>>>();
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
      restServices->push_back(boost::make_shared<web::rest::service::CKeyword>(dataProvider,
                                                                               dataAccessLayer->getKeywordManager(),
                                                                               *pluginGateway));
      restServices->push_back(boost::make_shared<web::rest::service::CPage>(dataProvider));
      restServices->push_back(boost::make_shared<web::rest::service::CWidget>(dataProvider,
                                                                              m_pathProvider->webServerPath().string()));
      restServices->push_back(boost::make_shared<web::rest::service::CConfiguration>(dataAccessLayer->getConfigurationManager()));
      restServices->push_back(boost::make_shared<web::rest::service::CPluginEventLogger>(dataProvider));
      restServices->push_back(boost::make_shared<web::rest::service::CEventLogger>(dataAccessLayer->getEventLogger()));
      restServices->push_back(boost::make_shared<web::rest::service::CSystem>(timezoneDatabase,
                                                                              hardware::usb::CDevicesListerFactory::createDeviceLister(),
                                                                              dataAccessLayer->getConfigurationManager()));
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

      const auto aliases = boost::make_shared<std::map<std::string, boost::filesystem::path>>();
      (*aliases)["plugins"] = m_pathProvider->pluginsPath();
      (*aliases)["scriptInterpreters"] = m_pathProvider->scriptInterpretersPath();
      (*aliases)["backups"] = m_pathProvider->backupPath();

      auto pocoBasedWebServer = createPocoBasedWebServer(startupOptions->getWebServerIPAddress(),
                                                         startupOptions->getWebServerPortNumber(),
                                                         startupOptions->getIsWebServerUseSSL(),
                                                         startupOptions->getSSLWebServerPortNumber(),
                                                         startupOptions->getWebServerAllowExternalAccess(),
                                                         m_pathProvider->webServerPath(),
                                                         dataAccessLayer,
                                                         restServices,
                                                         aliases,
                                                         dataAccessLayer->getConfigurationManager(),
                                                         startupOptions->getNoPasswordFlag());

      auto oatppBasedWebServer = createOatppBasedWebServer(startupOptions->getWebServerIPAddress(),
                                                           startupOptions->getWebServerPortNumber(),
                                                           startupOptions->getIsWebServerUseSSL(),
                                                           startupOptions->getSSLWebServerPortNumber(),
                                                           startupOptions->getWebServerAllowExternalAccess(),
                                                           m_pathProvider->webServerPath(),
                                                           dataAccessLayer,
                                                           restServices,
                                                           aliases,
                                                           dataAccessLayer->getConfigurationManager(),
                                                           startupOptions->getNoPasswordFlag());

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
   const std::string& address,
   unsigned short port,
   bool useSsl,
   unsigned short securedPort,
   bool allowExternalAccess,
   const boost::filesystem::path& webServerPath,
   const boost::shared_ptr<dataAccessLayer::IDataAccessLayer>& dataAccessLayer,
   const boost::shared_ptr<std::vector<boost::shared_ptr<web::rest::service::IRestService>>>& restServices,
   const boost::shared_ptr<std::map<std::string, boost::filesystem::path>>& aliases,
   const boost::shared_ptr<dataAccessLayer::IConfigurationManager>& configurationManager,
   bool skipPasswordCheck) const
{
   auto webServer(boost::make_shared<web::poco::CWebServer>(address,
                                                            port,
                                                            useSsl,
                                                            securedPort,
                                                            webServerPath.string(),
                                                            "/rest/",
                                                            "/ws",
                                                            allowExternalAccess));

   for (const auto& alias : *aliases)
      webServer->websiteHandlerAddAlias(alias.first,
                                        alias.second.string());

   webServer->configureAuthentication(boost::make_shared<web::poco::CBasicAuthentication>(configurationManager,
                                                                                          skipPasswordCheck));

   for (const auto& restService : *restServices)
      webServer->restHandlerRegisterService(restService);

   webServer->start();

   return webServer;
}

boost::shared_ptr<web::IWebServer> CSupervisor::createOatppBasedWebServer(
   const std::string& address,
   unsigned short port,
   bool useSsl,
   unsigned short securedPort,
   bool allowExternalAccess,
   const boost::filesystem::path& webServerPath,
   const boost::shared_ptr<dataAccessLayer::IDataAccessLayer>& dataAccessLayer,
   boost::shared_ptr<std::vector<boost::shared_ptr<web::rest::service::IRestService>>> restServices,
   boost::shared_ptr<std::map<std::string, boost::filesystem::path>> aliases,
   const boost::shared_ptr<dataAccessLayer::IConfigurationManager>& configurationManager,
   bool skipPasswordCheck) const
{
   boost::shared_ptr<web::IWebServer> webServer(
      boost::make_shared<web::oatppServer::CWebServer>(address,
                                                       port + 1, //TODO virer le +1 (pour test...)
                                                       useSsl,
                                                       securedPort + 1, //TODO virer le +1 (pour test...)
                                                       webServerPath,
                                                       "rest",
                                                       restServices,
                                                       "ws",
                                                       allowExternalAccess,
                                                       aliases,
                                                       boost::make_shared<web::oatppServer::CAuthentication>(configurationManager,
                                                          skipPasswordCheck)));

   return webServer;
}
