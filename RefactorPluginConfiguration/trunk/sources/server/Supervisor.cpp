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
#include "XplLogger.h"
#include "web/rest/service/PluginRestService.h"
#include "web/rest/service/DeviceRestService.h"
#include "web/rest/service/PageRestService.h"
#include "web/rest/service/WidgetRestService.h"
#include "web/rest/service/AcquisitionRestService.h"
#include "web/rest/service/ConfigurationRestService.h"
#include "web/rest/service/PluginEventLoggerRestService.h"
#include "web/rest/service/EventLoggerRestService.h"
#include <shared/ThreadBase.h>
#include <shared/Peripherals.h>


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

   try
   {
      boost::shared_ptr<database::IDataProvider> pDataProvider(new database::sqlite::CSQLiteDataProvider(m_startupOptions.getDatabaseFile()));
      if (!pDataProvider->load())
      {
         throw shared::exception::CException("Fail to load database");
      }

      // Start the plugin manager
      boost::shared_ptr<pluginSystem::CManager> pluginManager = pluginSystem::CManager::newManager(
         m_startupOptions.getPluginsPath(), pDataProvider->getPluginRequester(), pDataProvider->getPluginEventLoggerRequester(),
         *this, kPluginManagerEvent);

      //TODO ######################### test interface pluginManager #########################
#if DEV_ACTIVATE_PLUGIN_MANAGER_TESTS
      // 1) List all available plugins (even if not loaded) and associated informations
      pluginSystem::CManager::AvalaiblePluginMap plugins = pluginManager->getPluginList();
      YADOMS_LOG(debug) << "Available plugins :";
      BOOST_FOREACH(pluginSystem::CManager::AvalaiblePluginMap::value_type plugin, plugins)
      {
         YADOMS_LOG(debug) << "   - " << plugin.first << " : " << plugin.second->toString() <<
            ", Quality indicator = " << pluginManager->getPluginQualityIndicator(plugin.first);
      }

      // 2) User want to create new plugin instance
      // 2.1) GUI directly get the configuration schema (= default configuration) from a specific plugin
      const std::string& pluginName="fakePlugin";

      // 2.2) User can update default values, GUI returns configuration values
      // Here, were modified :
      // - EnumParameter from "EnumValue2" to "EnumValue1"
      // - DecimalParameter from "25.3" to "18.4"
      // - BitsFieldParameter : value "second one" from true to false
      std::string newConf("{ \"BitsFieldParameter\": { \"value\": { \"and a third\": \"true\", \"first checkbox\": \"false\", \"second one\": \"false\" } }, \"BoolParameter\": { \"value\": \"false\" }, \"DecimalParameter\": { \"value\": \"18.4\" }, \"EnumParameter\": { \"value\": \"EnumValue1\" }, \"IntParameter\": { \"value\": \"42\" }, \"Serial port\": { \"value\": \"tty0\" }, \"StringParameter\": { \"value\": \"Yadoms is so powerful !\" } }");
      // 2.3) User press OK to valid configuration and create the new instance
      int createdInstanceId;
      try
      {
         createdInstanceId = pluginManager->createInstance("theInstanceName", pluginName, newConf);
      }
      catch (database::CDatabaseException& e)
      {
      	YADOMS_LOG(error) << pluginName << " unable to create \"theInstanceName\", check that it doesn't already exist : " << e.what();
         createdInstanceId = 0;
      }

      // 3) List of IDs of existing plugin instances (all known instances, EXCEPT deleted)
      {
         boost::shared_ptr<std::vector<int> > instances = pluginManager->getInstanceList();
         std::ostringstream os;
         os << "Existing instances : ";
         BOOST_FOREACH(int value, *instances)
            os << value << " | ";
         YADOMS_LOG(debug) << os.str();
      }

      // 4) List of all plugin instances, with details (all instances, EVEN deleted)
      {
         boost::shared_ptr<pluginSystem::CManager::PluginDetailedInstanceMap> instances = pluginManager->getInstanceListDetails();
         YADOMS_LOG(debug) << "Existing instances, with details : ";
         BOOST_FOREACH(pluginSystem::CManager::PluginDetailedInstanceMap::value_type instance, *instances)
            YADOMS_LOG(debug) << "Id#" << instance.second->getId() <<
            ", name=" << instance.second->getName() <<
            ", plugin=" << instance.second->getPluginName() <<
            ", enabled=" << (instance.second->getEnabled() ? "true":"false") <<
            ", deleted=" << (instance.second->getDeleted() ? "true":"false") <<
            ", configuration=" << instance.second->getConfiguration();
      }

      // 5) Update instance configuration
      try
      {
         // 5.1) Next, get the actual configuration
         std::string instanceConfiguration(pluginManager->getInstanceConfiguration(createdInstanceId));
         if (instanceConfiguration.empty())
         {
            YADOMS_LOG(debug) << "Instance created at step #2 has no configuration";
         }
         else
         {
            // 5.2) Now, change some values (Serial port from tty0 to tty1, and BoolParameter from false to true)
            instanceConfiguration.replace(instanceConfiguration.find("\"Serial port\": { \"value\": \"tty0\" }"), 34, "\"Serial port\": { \"value\": \"tty1\" }");
            instanceConfiguration.replace(instanceConfiguration.find("\"BoolParameter\": { \"value\": \"false\" }"), 37, "\"BoolParameter\": { \"value\": \"true\" }");

            // 5.3) Valid the new configuration
            pluginManager->setInstanceConfiguration(createdInstanceId, instanceConfiguration);
         }
      }
      catch(shared::exception::CException& e)
      {
         YADOMS_LOG(debug) << "Unable to update instance configuration : " << e.what();
      }

      // 6) Disable (and stop) registered plugin instance (to be able to remove/replace plugin for example)
      try
      {
         pluginManager->disableInstance(createdInstanceId);
      }
      catch(shared::exception::CException& e)
      {
         YADOMS_LOG(debug) << "Unable to disbale instance : " << e.what();
      }

      // 7) Enable registered plugin instance (and start it)
      try
      {
         pluginManager->enableInstance(createdInstanceId);
      }
      catch(shared::exception::CException& e)
      {
         YADOMS_LOG(debug) << "Unable to enable instance : " << e.what();
      }

      // 8) Remove an instance
      try
      {
         pluginManager->deleteInstance(createdInstanceId);
      }
      catch(shared::exception::CException& e)
      {
         YADOMS_LOG(debug) << "Unable to delete instance : " << e.what();
      }
#endif
      //\TODO ######################### [END] test interface pluginManager #########################

      //TODO ######################### test serial ports getter #########################
#if DEV_ACTIVATE_SERIAL_PORTS_GETTER_TESTS
      try
      {
         const boost::shared_ptr<const shared::CPeripherals::SerialPortsMap> serialPorts(shared::CPeripherals::getSerialPorts());
         YADOMS_LOG(debug) << "Search system serial ports...";
         if (serialPorts->empty())
            YADOMS_LOG(debug) << "No serial port found.";
         else
            for (shared::CPeripherals::SerialPortsMap::const_iterator serialPort = serialPorts->begin() ; serialPort != serialPorts->end() ; ++serialPort)
               YADOMS_LOG(debug) << "Found serial port : " << serialPort->first << " (" << serialPort->second << ")";
      }
      catch (shared::exception::CNotSupported& e)
      {
         YADOMS_LOG(debug) << "Not supported function : " << e.what();
      }
#endif
      //\TODO ######################### [END] test interface pluginManager #########################


      // ######################### Web server #########################
      const std::string webServerIp = m_startupOptions.getWebServerIPAddress();
      const std::string webServerPort = boost::lexical_cast<std::string>(m_startupOptions.getWebServerPortNumber());
      const std::string webServerPath = m_startupOptions.getWebServerInitialPath();
      const std::string webServerWidgetPath = m_startupOptions.getWidgetsPath();

      boost::shared_ptr<web::IWebServer> webServer(new web::webem::CWebServer(webServerIp, webServerPort, webServerPath, "/rest/"));
      webServer->configureAlias("widget", webServerWidgetPath);
      boost::shared_ptr<web::IRestHandler> restHanlder = webServer->getRestHandler();
      if(restHanlder.get() != NULL)
      {
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPluginRestService(pDataProvider, pluginManager)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CDeviceRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPageRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CWidgetRestService(pDataProvider, webServerPath)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CAcquisitionRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CConfigurationRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CPluginEventLoggerRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<web::rest::service::IRestService>(new web::rest::service::CEventLoggerRestService(pDataProvider)));
      }

      boost::shared_ptr<web::CWebServerManager> webServerManager(new web::CWebServerManager(webServer));
      webServerManager->start();
      
      // ######################### [END] Web server #########################


      // ######################### Xpl Hub #########################
      //we start xpl hub only if it's necessary
      boost::shared_ptr<shared::xpl::CXplHub> hub;
      if (m_startupOptions.getStartXplHubFlag())
      {
         hub.reset(new shared::xpl::CXplHub(m_startupOptions.getXplNetworkIpAddress()));
         hub->start();
      }

#if DEV_ACTIVATE_XPL_TESTS
//      if(pluginManager.get() != NULL)
//         int createdInstanceId = pluginManager->createInstance("testOfXpl", "fakePlugin");
#endif
      // ######################### [END] Xpl Hub #########################

      // ######################### Xpl Logger #########################
      CXplLogger xplLogger(pDataProvider);
      xplLogger.start();

      // ######################### [END] Xpl Logger #########################

      YADOMS_LOG(info) << "Supervisor is running...";
      try
      {
         while(true)
         {
            switch(waitForEvents())
            {
            case kPluginManagerEvent:
               pluginManager->signalEvent(popEvent<pluginSystem::CManagerEvent>());
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
      xplLogger.stop();

      //stop xpl hub
      if(hub.get() != NULL)
         hub->stop();

      //stop web server
      if(webServerManager.get() != NULL)
         webServerManager->stop();
      
      pDataProvider->unload();//TODO : mettre un appel à unload dans le destructeur de IDataProvider (si pas déjà unloaded évidemment).


      YADOMS_LOG(info) << "Supervisor is stopped";
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception " << e.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception.";
   }
}
