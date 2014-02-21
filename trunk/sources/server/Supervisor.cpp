#include "stdafx.h"
#include "Supervisor.h"
#include "pluginSystem/HardwarePluginManager.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/DatabaseException.hpp"
#include <shared/Log.h>
#include <shared/Exceptions/NotSupportedException.hpp>
#include "web/webem/WebServer.h"
#include "web/WebServerManager.h"
#include <shared/Xpl/XplHub.h>
#include "XplLogger.h"
#include "web/rest/HardwareRestService.h"
#include "web/rest/DeviceRestService.h"
#include "web/rest/PageRestService.h"
#include "web/rest/WidgetRestService.h"
#include "web/rest/AcquisitionRestService.h"
#include "web/rest/ConfigurationRestService.h"
#include "web/rest/HardwareEventLoggerRestService.h"
#include "web/rest/EventLoggerRestService.h"
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
      boost::shared_ptr<server::database::IDataProvider> pDataProvider(new server::database::sqlite::CSQLiteDataProvider(m_startupOptions.getDatabaseFile()));
      if (!pDataProvider->load())
      {
         throw CException("Fail to load database");
      }

      // Start the hardware plugin manager
      boost::shared_ptr<CHardwarePluginManager> hardwarePluginManager = CHardwarePluginManager::newHardwarePluginManager(
         m_startupOptions.getHarwarePluginsPath(), pDataProvider->getHardwareRequester(), pDataProvider->getHardwareEventLoggerRequester(),
         *this, kHardwarePluginManagerEvent);

      //TODO ######################### test interface hardwarePluginManager #########################
#if DEV_ACTIVATE_HARDWARE_PLUGIN_MANAGER_TESTS
      // 1) List all available plugins (even if not loaded) and associated informations
      CHardwarePluginManager::AvalaiblePluginMap plugins = hardwarePluginManager->getPluginList();
      YADOMS_LOG(debug) << "Available plugins :";
      BOOST_FOREACH(CHardwarePluginManager::AvalaiblePluginMap::value_type plugin, plugins)
      {
         YADOMS_LOG(debug) << "   - " << plugin.first << " : " << plugin.second->toString() <<
            ", Quality indicator = " << hardwarePluginManager->getPluginQualityIndicator(plugin.first);
      }

      // 2) User want to create new plugin instance
      // 2.1) Get configuration schema (= default configuration) from a specific plugin
      const std::string& pluginName="fakePlugin";
      std::string pluginConfigurationSchema(hardwarePluginManager->getPluginConfigurationSchema(pluginName));
      if (!pluginConfigurationSchema.empty())
      {
         YADOMS_LOG(debug) << pluginName << " configuration schema is : \"" << pluginConfigurationSchema << "\"";
      }
      else
      {
         YADOMS_LOG(debug) << pluginName << " has no configuration";
      }
      // 2.2) User can update default values, GUI returns configuration values
      // Here, were modified :
      // - EnumParameter from "EnumValue2" to "EnumValue1"
      // - DoubleParameter from "25.3" to "18.4"
      // - BitsFieldParameter : value "second one" from true to false
      std::string newConf("{ \"BitsFieldParameter\": { \"value\": { \"and a third\": \"true\", \"first checkbox\": \"false\", \"second one\": \"false\" } }, \"BoolParameter\": { \"value\": \"false\" }, \"DoubleParameter\": { \"value\": \"18.4\" }, \"EnumParameter\": { \"value\": \"EnumValue1\" }, \"IntParameter\": { \"value\": \"42\" }, \"Serial port\": { \"value\": \"tty0\" }, \"StringParameter\": { \"value\": \"Yadoms is so powerful !\" } }");
      // 2.3) User press OK to valid configuration and create the new instance
      int createdInstanceId;
      try
      {
         createdInstanceId = hardwarePluginManager->createInstance("theInstanceName", pluginName, newConf);
      }
      catch (CDatabaseException& e)
      {
      	YADOMS_LOG(error) << pluginName << " unable to create \"theInstanceName\", check that it doesn't already exist : " << e.what();
         createdInstanceId = 0;
      }

      // 3) List of IDs of existing plugin instances (all known instances, EXCEPT deleted)
      {
         boost::shared_ptr<std::vector<int> > instances = hardwarePluginManager->getInstanceList();
         std::ostringstream os;
         os << "Existing instances : ";
         BOOST_FOREACH(int value, *instances)
            os << value << " | ";
         YADOMS_LOG(debug) << os.str();
      }

      // 4) List of all plugin instances, with details (all instances, EVEN deleted)
      {
         boost::shared_ptr<CHardwarePluginManager::PluginDetailedInstanceMap> instances = hardwarePluginManager->getInstanceListDetails();
         YADOMS_LOG(debug) << "Existing instances, with details : ";
         BOOST_FOREACH(CHardwarePluginManager::PluginDetailedInstanceMap::value_type instance, *instances)
            YADOMS_LOG(debug) << "Id#" << instance.second->getId() <<
            ", name=" << instance.second->getName() <<
            ", plugin=" << instance.second->getPluginName() <<
            ", enabled=" << (instance.second->getEnabled() ? "true":"false") <<
            ", deleted=" << (instance.second->getDeleted() ? "true":"false") <<
            ", configuration=" << instance.second->getConfiguration();
      }

      // 5) Update instance configuration
      {
         // 5.1) First, get the configuration schema
         std::string pluginConfigurationSchema(hardwarePluginManager->getPluginConfigurationSchema(createdInstanceId));
         if (!pluginConfigurationSchema.empty())
         {
            // 5.2) Next, get the actual configuration
            std::string instanceConfiguration(hardwarePluginManager->getInstanceConfiguration(createdInstanceId));
            if (instanceConfiguration.empty())
            {
               YADOMS_LOG(debug) << "Instance created at step #2 has no configuration";
            }
            else
            {
               // 5.3) Now, change some values (Serial port from tty0 to tty1, and BoolParameter from false to true)
               instanceConfiguration.replace(instanceConfiguration.find("\"Serial port\": { \"value\": \"tty0\" }"), 34, "\"Serial port\": { \"value\": \"tty1\" }");
               instanceConfiguration.replace(instanceConfiguration.find("\"BoolParameter\": { \"value\": \"false\" }"), 37, "\"BoolParameter\": { \"value\": \"true\" }");

               // 5.3) Valid the new configuration
               hardwarePluginManager->setInstanceConfiguration(createdInstanceId, instanceConfiguration);
            }
         }
      }

      // 6) Disable (and stop) registered plugin instance (to be able to remove/replace plugin for example)
      hardwarePluginManager->disableInstance(createdInstanceId);

      // 7) Enable registered plugin instance (and start it)
      hardwarePluginManager->enableInstance(createdInstanceId);

      // 8) Remove an instance
      hardwarePluginManager->deleteInstance(createdInstanceId);
#endif
      //\TODO ######################### [END] test interface hardwarePluginManager #########################

      //TODO ######################### test serial ports getter #########################
#if DEV_ACTIVATE_SERIAL_PORTS_GETTER_TESTS
      try
      {
         boost::shared_ptr<CPeripherals::SerialPortsMap> serialPorts(CPeripherals::getSerialPorts());
         YADOMS_LOG(debug) << "System serial ports";
         BOOST_FOREACH(CPeripherals::SerialPortsMap::value_type serialPort, *serialPorts)
         {
            YADOMS_LOG(debug) << "Found serial port : " << serialPort.first << " (" << serialPort.second << ")";
         }
      }
      catch (CNotSupportedException& e)
      {
         YADOMS_LOG(debug) << "Not supported function : " << e.what();
      }
#endif
      //\TODO ######################### [END] test interface hardwarePluginManager #########################


      // ######################### Web server #########################
      const std::string webServerIp = m_startupOptions.getWebServerIPAddress();
      const std::string webServerPort = boost::lexical_cast<std::string>(m_startupOptions.getWebServerPortNumber());
      const std::string webServerPath = m_startupOptions.getWebServerInitialPath();
      const std::string webServerWidgetPath = m_startupOptions.getWidgetsPath();

      boost::shared_ptr<IWebServer> webServer(new CWebServer(webServerIp, webServerPort, webServerPath, "/rest/"));
      webServer->configureAlias("widget", webServerWidgetPath);
      boost::shared_ptr<IRestHandler> restHanlder = webServer->getRestHandler();
      if(restHanlder.get() != NULL)
      {
         restHanlder->registerRestService(boost::shared_ptr<IRestService>(new CHardwareRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<IRestService>(new CDeviceRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<IRestService>(new CPageRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<IRestService>(new CWidgetRestService(pDataProvider, webServerPath)));
         restHanlder->registerRestService(boost::shared_ptr<IRestService>(new CAcquisitionRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<IRestService>(new CConfigurationRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<IRestService>(new CHardwareEventLoggerRestService(pDataProvider)));
         restHanlder->registerRestService(boost::shared_ptr<IRestService>(new CEventLoggerRestService(pDataProvider)));
      }

      boost::shared_ptr<CWebServerManager> webServerManager(new CWebServerManager(webServer));
      webServerManager->start();
      
      // ######################### [END] Web server #########################


      // ######################### Xpl Hub #########################
      //we start xpl hub only if it's necessary
      boost::shared_ptr<CXplHub> hub;
      if (m_startupOptions.getStartXplHubFlag())
      {
         hub.reset(new CXplHub(m_startupOptions.getXplNetworkIpAddress()));
         hub->start();
      }

#if DEV_ACTIVATE_XPL_TESTS
//      if(hardwarePluginManager.get() != NULL)
//         int createdInstanceId = hardwarePluginManager->createInstance("testOfXpl", "fakePlugin");
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
            case kHardwarePluginManagerEvent:
               hardwarePluginManager->signalEvent(popEvent<CHardwarePluginManagerEvent>());
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
      if(hardwarePluginManager.get() != NULL)
         hardwarePluginManager->stop();

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
