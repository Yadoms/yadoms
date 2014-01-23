#include "stdafx.h"
#include "Supervisor.h"
#include "pluginSystem/HardwarePluginManager.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include <shared/Log.h>
#include <shared/Exceptions/NotSupportedException.hpp>
#include "web/webem/WebServer.h"
#include "web/WebServerManager.h"
#include <shared/Xpl/XplHub.h>
#include "XplLogger.h"
#include "web/rest/HardwareRestService.h"
#include "web/rest/DeviceRestService.h"
#include <shared/ThreadBase.h>
#include <shared/Peripherals.h>

CSupervisor::CSupervisor(const IStartupOptions& startupOptions)
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
      boost::shared_ptr<IDataProvider> pDataProvider(new CSQLiteDataProvider(m_startupOptions.getDatabaseFile()));
      if (pDataProvider->load())
      {
#if DEV_ACTIVATE_DATABASE_TESTS
         //TODO ######################### test database #########################
         YADOMS_LOG(info) << "Testing database";

         std::vector<boost::shared_ptr<CHardware> > hardwares = pDataProvider->getHardwareRequester()->getHardwares();
         YADOMS_LOG(info) << "List of all hardwares";
         BOOST_FOREACH(boost::shared_ptr<CHardware> hardware, hardwares)
         {
            YADOMS_LOG(info) << "Name=" << hardware->getName() << " PluginName=" << hardware->getPluginName();
         }
         YADOMS_LOG(info) << "[END] List of all hardwares";


         YADOMS_LOG(info) << "Insert HW";
         boost::shared_ptr<CHardware> toAdd(new CHardware);
         toAdd->setName("AddHw1").setPluginName("FakePluginHw").setConfiguration("configuration pour le plugin AddHw1").setEnabled(true);
         int addedId = pDataProvider->getHardwareRequester()->addHardware(toAdd);

         YADOMS_LOG(info) << "Retrieve HW";
         boost::shared_ptr<CHardware> addedHw = pDataProvider->getHardwareRequester()->getHardware(addedId);
         YADOMS_LOG(info) << "Name=" << addedHw->getName() << " PluginName=" << addedHw->getPluginName() << " Config = " << addedHw->getConfiguration();

         YADOMS_LOG(info) << "Update config HW";
         pDataProvider->getHardwareRequester()->updateHardwareConfiguration(addedHw->getId(), "{\"BoolParameter\":\"true\",\"DoubleParameter\":\"56.78\",\"EnumParameter\":\"7\",\"IntParameter\":\"7\",\"Serial port\":\"tty0\",\"StringParameter\":\"Yadoms is so powerful !\"}");

         YADOMS_LOG(info) << "Retrieve updated HW";
         boost::shared_ptr<CHardware> addedHw2 = pDataProvider->getHardwareRequester()->getHardware(addedId);
         YADOMS_LOG(info) << "Name=" << addedHw2->getName() << " PluginName=" << addedHw2->getPluginName() << " Config = " << addedHw2->getConfiguration();

         pDataProvider->getHardwareRequester()->removeHardware(addedHw2->getId());

         YADOMS_LOG(info) << "List of all hardwares";
         BOOST_FOREACH(boost::shared_ptr<CHardware> hardware, hardwares)
         {
            YADOMS_LOG(info) << "Name=" << hardware->getName() << " PluginName=" << hardware->getPluginName();
         }
         YADOMS_LOG(info) << "[END] List of all hardwares";
         YADOMS_LOG(info) << "[END] Testing database";
         //\TODO ######################### [END] test database #########################
#endif
      }

      // Start the hardware plugin manager
      boost::shared_ptr<CHardwarePluginManager> hardwarePluginManager = CHardwarePluginManager::newHardwarePluginManager(
         m_startupOptions.getHarwarePluginsPath(), pDataProvider->getHardwareRequester());

      //TODO ######################### test interface hardwarePluginManager #########################
#if DEV_ACTIVATE_HARDWARE_PLUGIN_MANAGER_TESTS
      // 1) List all available plugins (even if not loaded) and associated informations
      CHardwarePluginManager::AvalaiblePluginMap plugins = hardwarePluginManager->getPluginList();
      YADOMS_LOG(debug) << "Available plugins :";
      BOOST_FOREACH(CHardwarePluginManager::AvalaiblePluginMap::value_type plugin, plugins)
      {
         YADOMS_LOG(debug) << "   - " << plugin.first << " : " << plugin.second->toString();
      }

      // 2) User want to create new plugin instance
      // 2.1) Get default configuration from a specific plugin
      const std::string& pluginName="fakePlugin";
      boost::optional<const CHardwarePluginConfiguration&> pluginDefaultConfiguration(hardwarePluginManager->getPluginDefaultConfiguration(pluginName));
      if (pluginDefaultConfiguration)
      {
         YADOMS_LOG(debug) << pluginName << " default configuration is :";
         for (CHardwarePluginConfiguration::CHardwarePluginConfigurationMap::const_iterator it = pluginDefaultConfiguration->getMap().begin() ;
            it != pluginDefaultConfiguration->getMap().end() ; ++it)
         {
            boost::shared_ptr<CHardwarePluginConfigurationParameter> parameter = (*it).second;

            // Get parameter name, description and value (as string)
            YADOMS_LOG(debug) << parameter->getName() << " (" << parameter->getDescription() << ")" << " = " << parameter->valueToString();

            // Process specific parameters types
            if (dynamic_cast<CHardwarePluginConfigurationEnumGeneric*>(parameter.get()))//TODO : voir si on ne peut pas mettre les dynamic_cast dans la conf
            {
               // Enum, get all available values
               std::ostringstream os;
               os << "Available values : ";
               boost::shared_ptr<std::vector<std::string> > values = dynamic_cast<CHardwarePluginConfigurationEnumGeneric*>(parameter.get())->getAvailableValues();
               BOOST_FOREACH(std::string value, *values)
                  os << value << "|";
               YADOMS_LOG(debug) << os.str();
            }
         }
      }
      else
      {
         YADOMS_LOG(debug) << pluginName << " has no configuration";
      }
      // 2.2) User can modify values (first, copy the configuration)
      CHardwarePluginConfiguration newConf = *pluginDefaultConfiguration;
      newConf.set("BoolParameter","true");
      newConf.set("EnumParameter","EnumValue3");
      // 2.3) User press OK to valid configuration and create the new instance
      int createdInstanceId = hardwarePluginManager->createInstance("theInstanceName", pluginName, newConf);

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
         // 5.1) First, get actual configuration
         boost::optional<CHardwarePluginConfiguration> instanceConfiguration(hardwarePluginManager->getInstanceConfiguration(createdInstanceId));
         if (!instanceConfiguration)
         {
            YADOMS_LOG(debug) << "Instance created at step #2 has no configuration";
         }
         else
         {
            // 5.2) Next, change some values
            instanceConfiguration.get().set("EnumParameter","EnumValue1");
            instanceConfiguration.get().set("DoubleParameter","56.78");

            // 5.3) Valid the new configuration
            hardwarePluginManager->setInstanceConfiguration(createdInstanceId, instanceConfiguration.get());
         }
      }

      // 6) Stop registered plugin instance (to be able to remove/replace plugin for example)
      hardwarePluginManager->stopInstance(createdInstanceId);

      // 7) Start registered plugin instance
      hardwarePluginManager->startInstance(createdInstanceId);

      // 8) Remove an instance
      hardwarePluginManager->deleteInstance(createdInstanceId);
#endif
      //\TODO ######################### [END] test interface hardwarePluginManager #########################

      //TODO ######################### test serial ports getter #########################
#if DEV_ACTIVATE_SERIAL_PORTS_GETTER_TESTS
      try
      {
         boost::shared_ptr<std::vector<std::string> > serialPorts(CPeripherals::getSerialPorts());
         YADOMS_LOG(debug) << "System serial ports";
         BOOST_FOREACH(std::string serialPort, *serialPorts)
         {
            YADOMS_LOG(debug) << serialPort;
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
         restHanlder->configureRestService(boost::shared_ptr<IRestService>(new CHardwareRestService(pDataProvider)));
         restHanlder->configureRestService(boost::shared_ptr<IRestService>(new CDeviceRestService(pDataProvider)));
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
      if(hardwarePluginManager.get() != NULL)
         int createdInstanceId = hardwarePluginManager->createInstance("testOfXpl", "fakePlugin");
#endif
      // ######################### [END] Xpl Hub #########################

      // ######################### Xpl Logger #########################
      CXplLogger xplLogger(pDataProvider);
      xplLogger.start();

      // ######################### [END] Xpl Logger #########################

      try
      {
         YADOMS_LOG(info) << "Supervisor is running...";
         while (getStatus() != CThreadBase::kStopping)
         {
            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
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
      
      pDataProvider->unload();//TODO : mettre un appel � unload dans le destructeur de IDataProvider (si pas d�j� unloaded �videmment).


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
