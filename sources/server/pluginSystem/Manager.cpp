#include "stdafx.h"
#include "Manager.h"
#include "Instance.h"
#ifdef _DEBUG
#include "BasicQualifier.h"
#else
#include "IndicatorQualifier.h"
#endif
#include <shared/DynamicLibrary.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/NotSupported.hpp>
#include <shared/exception/EmptyResult.hpp>
#include <shared/ServiceLocator.h>
#include <shared/plugin/yPluginApi/historization/PluginState.h>

#include "ExternalPluginLibrary.h"
#include "InternalPluginLibrary.h"

#include <shared/Log.h>

namespace pluginSystem
{

CManager::CManager(
   const std::string& initialDir,
   boost::shared_ptr<database::IDataProvider> dataProvider,
   boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
   boost::shared_ptr<shared::event::CEventHandler> supervisor,
   int pluginManagerEventId)
   :m_dataProvider(dataProvider), m_pluginDBTable(dataProvider->getPluginRequester()), m_pluginPath(initialDir),
#ifdef _DEBUG
   m_qualifier(new CBasicQualifier(dataProvider->getPluginEventLoggerRequester(), dataAccessLayer->getEventLogger())),
#else
   m_qualifier(new CIndicatorQualifier(dataProvider->getPluginEventLoggerRequester(), dataAccessLayer->getEventLogger())),
#endif
   m_supervisor(supervisor), m_pluginManagerEventId(pluginManagerEventId), m_dataAccessLayer(dataAccessLayer)
{
}

CManager::~CManager()
{
   stop();
}

void CManager::start()
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   // Initialize the plugin list (detect available plugins)
   updatePluginList();

   // Create and start plugin instances from database
   std::vector<boost::shared_ptr<database::entities::CPlugin> > databasePluginInstances = m_pluginDBTable->getInstances();
   
   for (std::vector<boost::shared_ptr<database::entities::CPlugin> >::iterator databasePluginInstanceIterator = databasePluginInstances.begin(); databasePluginInstanceIterator != databasePluginInstances.end(); ++databasePluginInstanceIterator)
   {
      if ((*databasePluginInstanceIterator)->Category() != database::entities::EPluginCategory::kSystem)
         if ((*databasePluginInstanceIterator)->AutoStart())
            startInstance((*databasePluginInstanceIterator)->Id());
   }

   //start the internal plugin
   startInternalPlugin();
}

void CManager::stop()
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   YADOMS_LOG(information) << "pluginSystem::CManager stop plugins...";
   stopInternalPlugin();

   while (!m_runningInstances.empty())
      stopInstance(m_runningInstances.begin()->first);

   YADOMS_LOG(information) << "pluginSystem::CManager all plugins are stopped";

}

std::vector<boost::filesystem::path> CManager::findPluginDirectories()
{
   // Look for all subdirectories in m_pluginPath directory, where it contains library with same name,
   // for example a subdirectory "fakePlugin" containing a "fakePlugin.dll|so" file
   std::vector<boost::filesystem::path> plugins;

   if (boost::filesystem::exists(m_pluginPath) && boost::filesystem::is_directory(m_pluginPath))
   {
      // Check all subdirectories in m_pluginPath
      for(boost::filesystem::directory_iterator subDirIterator(m_pluginPath) ; subDirIterator != boost::filesystem::directory_iterator() ; ++subDirIterator)
      {
         if (boost::filesystem::is_directory(subDirIterator->status()))
         {
            // Subdirectory, check if it is a plugin (= contains a dynamic library with same name)
            const std::string expectedLibName(shared::CDynamicLibrary::ToFileName(subDirIterator->path().filename().string()));
            for (boost::filesystem::directory_iterator fileIterator(subDirIterator->path()); fileIterator != boost::filesystem::directory_iterator() ; ++fileIterator)
            {
               if (boost::filesystem::is_regular_file(fileIterator->status()) &&                // It's a file...
                  boost::iequals(fileIterator->path().filename().string(), expectedLibName))                           // ...with the same name as sub-directory...
               {
                  plugins.push_back(subDirIterator->path());
               }
            }
         }
      }
   }

   return plugins;
}

boost::shared_ptr<ILibrary> CManager::loadPlugin(const std::string& pluginName)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   // Check if already loaded
   PluginMap::const_iterator itLoadedPlugin = m_loadedPlugins.find(pluginName);
   if (itLoadedPlugin != m_loadedPlugins.end())
      return itLoadedPlugin->second;  // Plugin already loaded

   // Check if plugin is available
   if (m_availablePlugins.find(pluginName) == m_availablePlugins.end())
      throw CInvalidPluginException(pluginName);   // Invalid plugin

   // Load the plugin
   boost::shared_ptr<ILibrary> pNewFactory(new CExternalPluginLibrary(toPath(pluginName)));
   m_loadedPlugins[pluginName] = pNewFactory;

   // Signal qualifier that a plugin was loaded
   m_qualifier->signalLoad(pNewFactory->getInformation());

   return pNewFactory;
}

bool CManager::unloadPlugin(const std::string& pluginName)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   PluginInstanceMap::const_iterator instance;
   for (instance = m_runningInstances.begin() ; instance != m_runningInstances.end() ; ++instance)
   {
      if ((*instance).second->getPluginName() == pluginName)
         break;
   }
   if (instance != m_runningInstances.end())
      return false;  // No unload : plugin is still used by another instance

   // Signal qualifier that a plugin is about to be unloaded
   BOOST_ASSERT_MSG(m_loadedPlugins.find(pluginName) != m_loadedPlugins.end(), "pluginName is not loaded");
   m_qualifier->signalUnload(m_loadedPlugins.find(pluginName)->second->getInformation());

   // Effectively unload plugin
   m_loadedPlugins.erase(pluginName);

   return true;
}

void CManager::buildAvailablePluginList()
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   // Empty the list
   m_availablePlugins.clear();

   // Search for library files
   std::vector<boost::filesystem::path> avalaiblePluginDirectories = findPluginDirectories();

   for (std::vector<boost::filesystem::path>::const_iterator libPathIt = avalaiblePluginDirectories.begin() ;
      libPathIt != avalaiblePluginDirectories.end() ; ++libPathIt)
   {
      try
      {
         // Get informations for current found plugin
         std::string pluginName = (*libPathIt).filename().string();

         // If plugin is already loaded, use its information
         if (m_loadedPlugins.find(pluginName) != m_loadedPlugins.end())
            m_availablePlugins[pluginName] = m_loadedPlugins[pluginName]->getInformation();
         else
         {
            boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation = CExternalPluginLibrary::getInformation(toPath(pluginName));
            if (pluginInformation->isSupportedOnThisPlatform())
               m_availablePlugins[pluginName] = pluginInformation;
            else
               YADOMS_LOG(warning) << "Plugin " << pluginName << " found but unsupported on this platform";
         }

         YADOMS_LOG(information) << "Plugin " << pluginName << " successfully loaded";
      }
      catch (CInvalidPluginException& e)
      {
         // Invalid plugin
         YADOMS_LOG(warning) << e.what() << ", found in plugin path but is not a valid plugin";
      }  
      catch (shared::exception::CInvalidParameter& e)
      {
         // Invalid plugin parameter
         YADOMS_LOG(warning) << "Invalid plugin parameter : " << e.what();
      }
      catch (shared::exception::CException & e)
      {
         // Fail to load one plugin
         YADOMS_LOG(warning) << "Invalid plugin : " << e.what();
      }
   }
}

void CManager::updatePluginList()
{
   // Plugin directory have change, so re-build plugin available list
   buildAvailablePluginList();
}


CManager::AvalaiblePluginMap CManager::getPluginList()
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   AvalaiblePluginMap mapCopy = m_availablePlugins;
   return mapCopy;
}

int CManager::getPluginQualityIndicator(const std::string& pluginName) const
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   if (m_availablePlugins.find(pluginName) == m_availablePlugins.end())
      throw CInvalidPluginException(pluginName);   // Invalid plugin

   return m_qualifier->getQualityLevel(m_availablePlugins.at(pluginName));
}

int CManager::createInstance(const database::entities::CPlugin& data)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   // First step, record instance in database, to get its ID
   int instanceId = m_pluginDBTable->addInstance(data);

   // Next create instance
   startInstance(instanceId);

   return instanceId;
}

void CManager::deleteInstance(boost::shared_ptr<database::entities::CPlugin> instanceToDelete)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   try
   {
      if (instanceToDelete->Category() != database::entities::EPluginCategory::kSystem)
      {
         // First step, disable and stop instance
         stopInstance(instanceToDelete->Id());

         // Next, delete in database
         m_pluginDBTable->removeInstance(instanceToDelete->Id());
      }
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(error) << "Unable to delete plugin instance (" << instanceToDelete->Id() << ") : " << e.what();
      throw shared::exception::CInvalidParameter(boost::lexical_cast<std::string>(instanceToDelete->Id()));
   }
}

std::vector<boost::shared_ptr<database::entities::CPlugin> > CManager::getInstanceList() const
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   return m_pluginDBTable->getInstances();
}

boost::shared_ptr<database::entities::CPlugin> CManager::getInstance(int id) const
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   return m_pluginDBTable->getInstance(id);
}

void CManager::updateInstance(const database::entities::CPlugin& newData)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   // Check for supported modifications
   if (!newData.Id.isDefined())
   {
      BOOST_ASSERT(false); // ID must be provided
      throw new shared::exception::CException("Update instance : instance ID was not provided");
   }

   // First get old configuration from database
   boost::shared_ptr<const database::entities::CPlugin> previousData = m_pluginDBTable->getInstance(newData.Id());

   // Next, update configuration in database
   m_pluginDBTable->updateInstance(newData);

   // Last, apply modifications
   if (newData.Configuration.isDefined()
      && previousData->Configuration() != newData.Configuration()) // No need to notify configuration if instance was enabled/disabled
   {
      // Configuration was updated, notify the instance, if running
      if (m_runningInstances.find(newData.Id()) != m_runningInstances.end())
         m_runningInstances[newData.Id()]->updateConfiguration(newData.Configuration());
   }
}

void CManager::startAllInstancesOfPlugin(const std::string& pluginName)
{
   // Find instances to start
   std::vector<int> instancesToStart;
   std::vector<boost::shared_ptr<database::entities::CPlugin> > allInstances = getInstanceList();
   for (std::vector<boost::shared_ptr<database::entities::CPlugin> >::const_iterator instance = allInstances.begin(); instance != allInstances.end(); ++instance)
   {
      if (boost::iequals((*instance)->Type(), pluginName) && (*instance)->AutoStart())
         instancesToStart.push_back((*instance)->Id());
   }

   // Start all instances of this plugin
   for (std::vector<int>::const_iterator instanceToStart = instancesToStart.begin(); instanceToStart != instancesToStart.end(); ++instanceToStart)
      startInstance(*instanceToStart);
}

void CManager::stopAllInstancesOfPlugin(const std::string& pluginName)
{
   // Find instances to stop
   std::vector<int> instancesToStop;
   for (PluginInstanceMap::const_iterator instance = m_runningInstances.begin(); instance != m_runningInstances.end(); ++instance)
   {
      if (instance->second && boost::iequals(instance->second->getPluginName(), pluginName))
         instancesToStop.push_back(instance->first);
   }

   // Stop all instances of this plugin
   for (std::vector<int>::const_iterator instanceToStop = instancesToStop.begin(); instanceToStop != instancesToStop.end(); ++instanceToStop)
      stopInstance(*instanceToStop);
}

void CManager::signalEvent(const CManagerEvent& event)
{
   switch(event.getSubEventId())
   {
   case CManagerEvent::kPluginInstanceAbnormalStopped:
      {
         // The thread of an instance has stopped in a non-conventional way (probably crashed)

         // First perform the full stop
         stopInstance(event.getInstanceId());

         // Now, evaluate if it is still safe
         if (m_qualifier->isSafe(event.getPluginInformation()))
         {
            // Don't restart if event occurs when instance was stopping
            if (!event.getStopping())
            {
               // Still safe, try to restart it
               startInstance(event.getInstanceId());
            }
         }
         else
         {
            // Not safe anymore. Disable plugin autostart mode (user will just be able to start it manually)
            // Not that this won't stop other instances of this plugin
            YADOMS_LOG(warning) << " plugin " << event.getPluginInformation()->getType() << " was evaluated as not safe and will not start automatically anymore.";
            m_pluginDBTable->disableAutoStartForAllPluginInstances(event.getPluginInformation()->getType());

            // Log this event in the main event logger
            m_dataAccessLayer->getEventLogger()->addEvent(database::entities::ESystemEventCode::kPluginDisabled,
               event.getPluginInformation()->getIdentity(),
               "Plugin " + event.getPluginInformation()->getIdentity() + " was evaluated as not safe and will not start automatically anymore.");
         }

         break;
      }
   default:
      {
         YADOMS_LOG(error) << "Unknown message id";
         BOOST_ASSERT(false);
         break;
      }
   }
}

boost::filesystem::path CManager::toPath(const std::string& pluginName) const
{
   boost::filesystem::path path(m_pluginPath);
   path /= pluginName;
   path /= shared::CDynamicLibrary::ToFileName(pluginName);
   return path;
}

void CManager::startInstance(int id)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   if (m_runningInstances.find(id) != m_runningInstances.end())
      return;     // Already started ==> nothing more to do

   try
   {
      // Get instance informations from database
      boost::shared_ptr<database::entities::CPlugin> databasePluginInstance(m_pluginDBTable->getInstance(id));

      // Load the plugin
      boost::shared_ptr<ILibrary> plugin(loadPlugin(databasePluginInstance->Type()));

      // Create instance
      BOOST_ASSERT(plugin); // Plugin not loaded
      boost::shared_ptr<CInstance> pluginInstance(new CInstance(
         plugin, databasePluginInstance, m_dataProvider->getPluginEventLoggerRequester(), m_dataAccessLayer->getDeviceManager(), m_dataProvider->getKeywordRequester(),
         m_dataProvider->getRecipientRequester(), m_dataProvider->getAcquisitionRequester(), m_dataAccessLayer->getAcquisitionHistorizer(),
         m_qualifier, m_supervisor, m_pluginManagerEventId));
      m_runningInstances[databasePluginInstance->Id()] = pluginInstance;
   }
   catch (shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(error) << "startInstance : unable to find plugin instance id " << id << " : " << e.what();
      return;   	
   }
   catch (CInvalidPluginException& e)
   {
      YADOMS_LOG(error) << "startInstance : " << e.what();   	
   }
}

void CManager::stopInstance(int id)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   if (m_runningInstances.find(id) == m_runningInstances.end())
      return;     // Already stopped ==> nothing more to do

   // Get the associated plugin name to unload it after instance being deleted
   std::string pluginName = m_runningInstances[id]->getPluginName();

   // Remove (=stop) instance
   m_runningInstances.erase(id);

   // Try to unload associated plugin (if no more used)
   unloadPlugin(pluginName);
}


void CManager::startInternalPlugin()
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);


   try
   {
      boost::shared_ptr<database::entities::CPlugin> databasePluginInstance(m_pluginDBTable->getSystemInstance());

      if (m_runningInstances.find(databasePluginInstance->Id()) != m_runningInstances.end())
         return;     // Already started ==> nothing more to do


      // Load the plugin
      boost::shared_ptr<ILibrary> plugin(new CInternalPluginLibrary());

      // Create instance
      BOOST_ASSERT(plugin); // Plugin not loaded
      boost::shared_ptr<CInstance> pluginInstance(new CInstance(
         plugin, databasePluginInstance, m_dataProvider->getPluginEventLoggerRequester(), m_dataAccessLayer->getDeviceManager(), m_dataProvider->getKeywordRequester(),
         m_dataProvider->getRecipientRequester(), m_dataProvider->getAcquisitionRequester(), m_dataAccessLayer->getAcquisitionHistorizer(),
         m_qualifier, m_supervisor, m_pluginManagerEventId));
      m_runningInstances[databasePluginInstance->Id()] = pluginInstance;
   }
   catch (shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(error) << "startInternalPlugin : unable to find internal plugin : " << e.what();
      return;
   }
   catch (CInvalidPluginException& e)
   {
      YADOMS_LOG(error) << "startInternalPlugin : " << e.what();
   }
}

void CManager::stopInternalPlugin()
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   //get the plugin info from db
   boost::shared_ptr<database::entities::CPlugin> databasePluginInstance(m_pluginDBTable->getSystemInstance());

   // Already stopped ==> nothing more to do
   if (m_runningInstances.find(databasePluginInstance->Id()) == m_runningInstances.end())
      return;

   // Remove (=stop) instance
   m_runningInstances.erase(databasePluginInstance->Id());
}

bool CManager::isInstanceRunning(int id) const
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
   return m_runningInstances.find(id) != m_runningInstances.end();
}

shared::CDataContainer CManager::getInstanceState(int id) const
{
   if (!isInstanceRunning(id))
   {
      shared::CDataContainer defaultState;
      defaultState.set("state", "Stopped");
      return defaultState;
   }

   boost::shared_ptr<database::entities::CDevice> device;
   try
   {
      // First find the pluginState device associated with the plugin
      device = m_dataProvider->getDeviceRequester()->getDevice(id, "pluginState");
   }
   catch (shared::exception::CEmptyResult&)
   {
      // Device doesn't exist, probably not supported by plugin. Plugin is then considered as running.
      shared::CDataContainer defaultState;
      defaultState.set("state", shared::plugin::yPluginApi::historization::EPluginState::kRunning);
      return defaultState;
   }

   try
   {
      boost::shared_ptr<database::entities::CKeyword> stateKw = m_dataProvider->getKeywordRequester()->getKeyword(device->Id, "state");
      return m_dataProvider->getAcquisitionRequester()->getKeywordLastData(stateKw->Id)->Value();
   }
   catch (shared::exception::CEmptyResult&)
   {
      // pluginState keyword exist, but was never historized, so considered as unknown.
      shared::CDataContainer defaultState;
      defaultState.set("state", shared::plugin::yPluginApi::historization::EPluginState::kUnknown);
      return defaultState;
   }
}

void CManager::postCommand(int id, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   if (!isInstanceRunning(id))
      return;     // Instance is stopped, nothing to do

   boost::shared_ptr<CInstance> instance(m_runningInstances.find(id)->second);
   instance->postCommand(command);
}


void CManager::postManuallyDeviceCreationRequest(int id, boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> & request)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   if (!isInstanceRunning(id))
      return;     // Instance is stopped, nothing to do

   boost::shared_ptr<CInstance> instance(m_runningInstances.find(id)->second);
   instance->postManuallyDeviceCreationRequest(request);
}

void CManager::postBindingQueryRequest(int id, boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> & request)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   if (!isInstanceRunning(id))
      return;     // Instance is stopped, nothing to do

   boost::shared_ptr<CInstance> instance(m_runningInstances.find(id)->second);
   instance->postBindingQueryRequest(request);
}

} // namespace pluginSystem
