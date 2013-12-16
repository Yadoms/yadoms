#include "stdafx.h"
#include "Supervisor.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include "tools/Log.h"

CSupervisor::CSupervisor(const IStartupOptions& startupOptions)
   :CThreadBase("Supervisor"), m_startupOptions(startupOptions)
{
}


CSupervisor::~CSupervisor(void)
{
   BOOST_ASSERT(getStatus()==kStopped);   // CSupervisor must be stopped before destroy
}

void CSupervisor::doWork()
{
   try
   {
      YADOMS_LOG_CONFIGURE("Supervisor");
      YADOMS_LOG(info) << "Supervisor is starting";


      YADOMS_LOG(info) << "Testing database";
      boost::shared_ptr<IDataProvider> pDataProvider (new CSQLiteDataProvider(m_startupOptions.getDatabaseFile()));
      if(pDataProvider->load())
      {

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

         YADOMS_LOG(info) << "Retreive HW";
         boost::shared_ptr<CHardware> addedHw = pDataProvider->getHardwareRequester()->getHardware(addedId);
         YADOMS_LOG(info) << "Name=" << addedHw->getName() << " PluginName=" << addedHw->getPluginName() << " Config = " << addedHw->getConfiguration();

         YADOMS_LOG(info) << "Update config HW";
         pDataProvider->getHardwareRequester()->updateHardwareConfiguration(addedHw->getId(), "new config");

         YADOMS_LOG(info) << "Retreive updated HW";
         boost::shared_ptr<CHardware> addedHw2 = pDataProvider->getHardwareRequester()->getHardware(addedId);
         YADOMS_LOG(info) << "Name=" << addedHw2->getName() << " PluginName=" << addedHw2->getPluginName() << " Config = " << addedHw2->getConfiguration();

         pDataProvider->getHardwareRequester()->removeHardware(addedHw2->getId());

         YADOMS_LOG(info) << "List of all hardwares";
         BOOST_FOREACH(boost::shared_ptr<CHardware> hardware, hardwares)
         {
            YADOMS_LOG(info) << "Name=" << hardware->getName() << " PluginName=" << hardware->getPluginName();
         }
         YADOMS_LOG(info) << "[END] List of all hardwares";
      }
      YADOMS_LOG(info) << "[END] Testing database";


      // Start the hardware plugin manager
      boost::shared_ptr<CHardwarePluginManager> hardwarePluginManager = CHardwarePluginManager::newHardwarePluginManager(
         "../builds/lib/Debug"/* TODO m_startupOptions.getHarwarePluginsPath() */,
         pDataProvider->getHardwareRequester());

      //TODO ######################### test interface hardwarePluginManager #########################
      // 1) List all available plugins (even if not loaded) and associated informations
      CHardwarePluginManager::AvalaiblePluginMap plugins = hardwarePluginManager->getPluginList();
      YADOMS_LOG(debug) << "Available plugins :";
      BOOST_FOREACH(CHardwarePluginManager::AvalaiblePluginMap::value_type plugin, plugins)
      {
         YADOMS_LOG(debug) << "   - " << plugin.first << " : " << plugin.second->toString();
      }

      // 2) User want to create new plugin instance
      // 2.1) Get default configuration from a specific plugin (User want to create new plugin instance)
      const std::string& pluginName="fakePlugin";
      boost::optional<const CHardwarePluginConfiguration&> pluginDefaultConfiguration(hardwarePluginManager->getPluginDefaultConfiguration(pluginName));
      if (pluginDefaultConfiguration)
      {
         YADOMS_LOG(debug) << pluginName << " configuration is :";
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
      // 2.2) User can modify value (first, copy the configuration)
      CHardwarePluginConfiguration newConf = *pluginDefaultConfiguration;
      newConf.set("BoolParameter","true");
      newConf.set("EnumParameter","EnumValue3");
      // 2.3) User press OK to valid configuration and create the new instance
//TODO pas encore disponible (attente de CSQLiteHardwareRequester::addHardware)      hardwarePluginManager->createInstance("theInstanceName", pluginName, newConf);

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
         boost::optional<CHardwarePluginConfiguration> instanceConfiguration(hardwarePluginManager->getInstanceConfiguration(1));
         if (instanceConfiguration)
         {
            YADOMS_LOG(debug) << "Instance 1 has no configuration";
         }
         else
         {
            // 5.2) Next, change some values
            instanceConfiguration.get().set("EnumParameter","EnumValue1");
            instanceConfiguration.get().set("DoubleParameter","56.78");

            // 5.3) Valid the new configuration
//TODO pas encore disponible (attente de CSQLiteHardwareRequester::addHardware)            hardwarePluginManager->setInstanceConfiguration(1, instanceConfiguration.get());
         }
      }

      // 6) Remove an instance
      {
//TODO pas encore disponible (attente de CSQLiteHardwareRequester::addHardware)         hardwarePluginManager->deleteInstance(1);
      }

      // 7) Stop registered plugin instance (to be able to remove/replace plugin for example)
//TODO pas encore disponible      hardwarePluginManager->stopInstance(1);

      // 8) Start registered plugin instance
//TODO pas encore disponible      hardwarePluginManager->startInstance(1);
      //\TODO ######################### [FIN] test interface hardwarePluginManager #########################

      try
      {
         while(1)
         {
            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
         }
      }
      catch (boost::thread_interrupted&)
      {
         YADOMS_LOG(info) << "Supervisor is stopping...";
      }

      pDataProvider->unload();//TODO : mettre un appel à unload dans le destructeur de IDataProvider (si pas déjà unloaded évidemment).

      YADOMS_LOG(info) << "Supervisor is stopped";
   }
   catch(std::exception& e)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception " << e.what();
   }
   catch(...)
   {
      YADOMS_LOG(error) << "Supervisor : unhandled exception.";
   }
}
