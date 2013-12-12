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
      }

      // Start the hardware plugin manager
      boost::shared_ptr<CHardwarePluginManager> hardwarePluginManager = CHardwarePluginManager::newHardwarePluginManager(
         "../builds/lib/Debug"/* TODO m_startupOptions.getHarwarePluginsPath() */,
         pDataProvider->getHardwareRequester());

      //TODO : test interface hardwarePluginManager
      // 1) List all available plugins (even if not loaded) and associated informations
      CHardwarePluginManager::AvalaiblePluginMap plugins = hardwarePluginManager->getPluginList();
      YADOMS_LOG(debug) << "Available plugins :";
      BOOST_FOREACH(CHardwarePluginManager::AvalaiblePluginMap::value_type plugin, plugins)
      {
         YADOMS_LOG(debug) << "   - " << plugin.first << " : " << plugin.second->toString();
      }
      // 2) Get default configuration from a specific plugin (User want to create new plugin instance)
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
      // 3) User can modify value (first, copy the configuration)
      CHardwarePluginConfiguration newConf = pluginDefaultConfiguration.get();
      newConf.set("BoolParameter","true");
      newConf.set("EnumParameter","EnumValue3");
      // 4) User press OK to valid configuration and create the new instance
//TODO pas encore disponible (attente de CSQLiteHardwareRequester::addHardware)      hardwarePluginManager->createPluginInstance("theInstanceName", pluginName, newConf);
      // 3) Stop registered plugin instance (to be able to remove/replace plugin for example)
      try
      {
         hardwarePluginManager->startInstance(1);
      }
      catch (std::invalid_argument& e)
      {
         YADOMS_LOG(error) << "Unable to start fakePlugin1 : " << e.what();
      }

      //\TODO

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
