#include "stdafx.h"//TODO faire ménage dans les include
#include "Manager.h"
#include "Instance.h"
#include "PluginStateHandler.h"
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
#include "PluginException.hpp"

namespace pluginSystem
{

CManager::CManager(
   const std::string& initialDir,
   boost::shared_ptr<database::IDataProvider> dataProvider,
   boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
   boost::shared_ptr<shared::event::CEventHandler> supervisor,
   boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger)
   :m_dataProvider(dataProvider), m_pluginDBTable(dataProvider->getPluginRequester()), m_pluginPath(initialDir),
#ifdef _DEBUG //TODO faut-il conserver les qualifiers ?
   m_qualifier(new CBasicQualifier(dataProvider->getPluginEventLoggerRequester(), dataAccessLayer->getEventLogger())),
#else
   m_qualifier(new CIndicatorQualifier(dataProvider->getPluginEventLoggerRequester(), dataAccessLayer->getEventLogger())),
#endif
   m_supervisor(supervisor), m_dataAccessLayer(dataAccessLayer),
   m_pluginEventHandler(boost::make_shared<shared::event::CEventHandler>()),
   m_pluginStateHandler(boost::make_shared<CPluginStateHandler>(m_pluginDBTable, eventLogger, m_pluginEventHandler)),
   m_pluginEventsThread(boost::make_shared<boost::thread>(boost::bind(&CManager::pluginEventsThreadDoWork, this)))
{
}

CManager::~CManager()
{
   stop();
}

void CManager::start()
{
   boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

   // Initialize the plugin list (detect available plugins)
   updatePluginList();

   startAllInstances();
}

void CManager::stop()
{
   stopInstances();

   m_pluginEventsThread->interrupt();
   m_pluginEventsThread->join();
}

void CManager::startAllInstances()
{
   boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
   if (!m_runningInstances.empty())
      throw shared::exception::CException("Some plugins are already started, are you sure that manager was successfuly stopped ?");

   if (!startInstances(getInstanceList()))
      YADOMS_LOG(error) << "One or more plugins failed to start, check plugins page for details";

   //start the internal plugin
   startInternalPlugin();
}

bool CManager::startInstances(const std::vector<boost::shared_ptr<database::entities::CPlugin> >& instances)
{
   bool allInstancesStarted = true;
   for (std::vector<boost::shared_ptr<database::entities::CPlugin> >::const_iterator it = instances.begin(); it != instances.end(); ++it)
   {
      try
      {
         if ((*it)->Category() != database::entities::EPluginCategory::kSystem)
            if ((*it)->AutoStart())
               startInstance((*it)->Id());
      }
      catch (CPluginException& ex)
      {
         YADOMS_LOG(error) << "Unable to start plugin " << (*it)->DisplayName() << "(" << ex.what() << "), skipped";
         allInstancesStarted = false;
      }
   }

   return allInstancesStarted;
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
   if (m_loadedPlugins.find(pluginName) == m_loadedPlugins.end())
      throw shared::exception::CException("pluginName is not loaded");
   m_qualifier->signalUnload(m_loadedPlugins.find(pluginName)->second->getInformation());

   // Effectively unload plugin
   m_loadedPlugins.erase(pluginName);

   return true;
}

void CManager::buildAvailablePluginList()
{
   boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

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
   boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

   // First step, record instance in database, to get its ID
   int instanceId = m_pluginDBTable->addInstance(data);

   // Next create instance
   startInstance(instanceId);

   return instanceId;
}

void CManager::deleteInstance(int id)
{
   try
   {
      boost::shared_ptr<database::entities::CPlugin> instanceData = m_pluginDBTable->getInstance(id);
      if (instanceData->Category() == database::entities::EPluginCategory::kSystem)
         return;

      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      // Stop the rule
      stopInstanceAndWaitForStopped(id);

      // Remove in database
      m_pluginDBTable->removeInstance(id);
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(error) << "Unable to delete plugin (" << id << ") : " << e.what();
      throw shared::exception::CInvalidParameter(boost::lexical_cast<std::string>(id));
   }
}

std::vector<boost::shared_ptr<database::entities::CPlugin> > CManager::getInstanceList() const
{
   boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
   return m_pluginDBTable->getInstances();
}

boost::shared_ptr<database::entities::CPlugin> CManager::getInstance(int id) const
{
   boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
   return m_pluginDBTable->getInstance(id);
}

void CManager::updateInstance(const database::entities::CPlugin& newData)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

   // Check for supported modifications
   if (!newData.Id.isDefined())
      throw new shared::exception::CException("Update instance : instance ID was not provided");

   // First get old configuration from database
   boost::shared_ptr<const database::entities::CPlugin> previousData = m_pluginDBTable->getInstance(newData.Id());

   // Next, update configuration in database
   m_pluginDBTable->updateInstance(newData);

   // Last, apply modifications
   if (newData.Configuration.isDefined()
      && previousData->Configuration() != newData.Configuration()) // No need to notify configuration if instance was just enabled/disabled
   {
      // Configuration was updated, notify the instance if running
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

void CManager::startInstance(int id)
{
   try
   {
      boost::shared_ptr<database::entities::CPlugin> instanceData(getInstance(id));

      if (isInstanceRunning(instanceData->Id()))
         return;     // Already started ==> nothing more to do

      recordInstanceStarted(id);

      YADOMS_LOG(information) << "Start plugin instance " << instanceData->DisplayName();

      // Load the plugin
      boost::shared_ptr<ILibrary> plugin(loadPlugin(instanceData->Type()));
      if (!plugin)
         throw CPluginException("Fail to load plugin"); // Plugin not loaded

      // Create instance
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
      boost::shared_ptr<CInstance> pluginInstance(boost::make_shared<CInstance>(
         plugin, instanceData, m_dataProvider->getPluginEventLoggerRequester(), m_dataAccessLayer->getDeviceManager(), m_dataProvider->getKeywordRequester(),
         m_dataProvider->getRecipientRequester(), m_dataProvider->getAcquisitionRequester(), m_dataAccessLayer->getAcquisitionHistorizer(),
         m_qualifier, m_supervisor, m_pluginManagerEventId));
      m_runningInstances[instanceData->Id()] = pluginInstance;
   }
   catch (shared::exception::CEmptyResult& e)
   {
      const std::string& error((boost::format("Invalid plugin instance %1%, element not found in database : %2%") % id % e.what()).str());
      m_pluginStateHandler->signalError(id, error); 
      throw CPluginException(error);
   }
   catch (CInvalidPluginException& e)
   {
      const std::string& error((boost::format("Invalid plugin instance %1% configuration, invalid parameter : %2%") % id % e.what()).str());
      m_pluginStateHandler->signalError(id, error);
      throw CPluginException(error);
   }
   catch (shared::exception::COutOfRange& e)
   {
      const std::string& error((boost::format("Invalid plugin instance %1% configuration, out of range : %2%") % id % e.what()).str());
      m_pluginStateHandler->signalError(id, error);
      throw CPluginException(error);
   }
}

void CManager::stopInstances()
{
   YADOMS_LOG(information) << "pluginSystem::CManager stop plugins...";

   stopInternalPlugin();

   boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

   while (!m_runningInstances.empty())
      stopInstanceAndWaitForStopped(m_runningInstances.begin()->first);

   YADOMS_LOG(information) << "pluginSystem::CManager all plugins are stopped";
}

void CManager::stopInstance(int id)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

   std::map<int, boost::shared_ptr<IInstance> >::iterator instance = m_runningInstances.find(id);

   if (instance == m_runningInstances.end())
      return;     // Already stopped ==> nothing more to do

   instance->second->requestStop();
}

void CManager::stopInstanceAndWaitForStopped(int id)
{
   boost::shared_ptr<shared::event::CEventHandler> waitForStoppedInstanceHandler(boost::make_shared<shared::event::CEventHandler>());
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_instanceStopNotifiersMutex);
      m_instanceStopNotifiers[id].insert(waitForStoppedInstanceHandler);
   }

   if (isInstanceRunning(id))
   {
      // Stop the instance
      stopInstance(id);

      // Wait for instance stopped
      if (waitForStoppedInstanceHandler->waitForEvents(boost::posix_time::seconds(10)) == shared::event::kTimeout)
         throw CPluginException("Unable to stop instance");
   }

   {
      boost::lock_guard<boost::recursive_mutex> lock(m_instanceStopNotifiersMutex);
      m_instanceStopNotifiers[id].erase(waitForStoppedInstanceHandler);
   }
}

void CManager::onInstanceStopped(int id, const std::string& error)
{
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      std::map<int, boost::shared_ptr<IInstance> >::iterator instance = m_runningInstances.find(id);

      if (instance == m_runningInstances.end())
         return;     // Already stopped ==> nothing more to do

      std::string pluginName = instance->second->getPluginName();

      m_runningInstances.erase(instance);
      
      unloadPlugin(pluginName);

      if (!m_yadomsShutdown)
         recordInstanceStopped(id, error);
   }

   {
      // Notify all handlers for this instance
      boost::lock_guard<boost::recursive_mutex> lock(m_instanceStopNotifiersMutex);
      std::map<int, std::set<boost::shared_ptr<shared::event::CEventHandler> > >::const_iterator itEventHandlerSetToNotify = m_instanceStopNotifiers.find(id);
      if (itEventHandlerSetToNotify != m_instanceStopNotifiers.end())
         for (std::set<boost::shared_ptr<shared::event::CEventHandler> >::const_iterator itHandler = itEventHandlerSetToNotify->second.begin(); itHandler != itEventHandlerSetToNotify->second.end(); ++itHandler)
            (*itHandler)->postEvent(shared::event::kUserFirstId);
   }
}

void CManager::signalEvent(const CManagerEvent& event)//TODO à virer ?
{
   switch (event.getSubEventId())
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
            // Still safe, try to restart it (only if it was a plugin crash. If it's a plugin error, user have to restart plugin manually)
            if (getInstanceState(event.getInstanceId()) != shared::plugin::yPluginApi::historization::EPluginState::kError)
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

shared::CDataContainer CManager::getInstanceFullState(int id) const
{
   if (!isInstanceRunning(id))
   {
      // Instance is not running, so can be in error or stopped state
      boost::shared_ptr<database::entities::CDevice> device;
      try
      {
         // First find the pluginState device associated with the plugin
         device = m_dataProvider->getDeviceRequester()->getDevice(id, "pluginState");
      }
      catch (shared::exception::CEmptyResult&)
      {
         // Device doesn't exist, probably not supported by plugin. Plugin is then considered as stopped.
         shared::CDataContainer defaultState;
         defaultState.set("state", shared::plugin::yPluginApi::historization::EPluginState::kStopped);
         return defaultState;
      }

      try
      {
         boost::shared_ptr<database::entities::CKeyword> stateKw = m_dataProvider->getKeywordRequester()->getKeyword(device->Id, "state");
         shared::plugin::yPluginApi::historization::EPluginState state(m_dataProvider->getAcquisitionRequester()->getKeywordLastData(stateKw->Id)->Value());
         if (state == shared::plugin::yPluginApi::historization::EPluginState::kError)
         {
            // In error state
            boost::shared_ptr<database::entities::CKeyword> customMessageIdKw = m_dataProvider->getKeywordRequester()->getKeyword(device->Id, "customMessageId");
            shared::CDataContainer defaultState;
            defaultState.set("state", state);
            defaultState.set("messageId", m_dataProvider->getAcquisitionRequester()->getKeywordLastData(customMessageIdKw->Id)->Value());
            return defaultState;
         }

         // Normaly stopped
         shared::CDataContainer defaultState;
         defaultState.set("state", shared::plugin::yPluginApi::historization::EPluginState::kStopped);
         return defaultState;
      }
      catch (shared::exception::CEmptyResult&)
      {
         // pluginState keyword exist, but was never historized, so considered as stopped.
         shared::CDataContainer defaultState;
         defaultState.set("state", shared::plugin::yPluginApi::historization::EPluginState::kStopped);
         return defaultState;
      }
   }

   // Instance is running
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
      boost::shared_ptr<database::entities::CKeyword> customMessageIdKw = m_dataProvider->getKeywordRequester()->getKeyword(device->Id, "customMessageId");
      shared::CDataContainer defaultState;
      defaultState.set("state", m_dataProvider->getAcquisitionRequester()->getKeywordLastData(stateKw->Id)->Value());
      defaultState.set("messageId", m_dataProvider->getAcquisitionRequester()->getKeywordLastData(customMessageIdKw->Id)->Value());
      return defaultState;
   }
   catch (shared::exception::CEmptyResult&)
   {
      // pluginState keyword exist, but was never historized, so considered as unknown.
      shared::CDataContainer defaultState;
      defaultState.set("state", shared::plugin::yPluginApi::historization::EPluginState::kUnknown);
      return defaultState;
   }
}

shared::plugin::yPluginApi::historization::EPluginState CManager::getInstanceState(int id) const
{
   shared::CDataContainer fullState = getInstanceFullState(id);
   return fullState.get<shared::plugin::yPluginApi::historization::EPluginState>("state");
}

void CManager::pluginEventsThreadDoWork()
{
   try
   {
      while (true)
      {
         switch (m_pluginEventHandler->waitForEvents())
         {
         case CPluginStateHandler::kAbnormalStopped:
         {
            // The rule has stopped in a non-conventional way (probably crashed)
            std::pair<int, std::string> data = m_pluginEventHandler->getEventData<std::pair<int, std::string> >();
            onInstanceStopped(data.first, data.second);
            break;
         }
         case CPluginStateHandler::kRuleStopped:
         {
            onInstanceStopped(m_pluginEventHandler->getEventData<int>());
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
   }
   catch (boost::thread_interrupted&)
   {
   }
}

void CManager::postCommand(int id, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

   if (!isInstanceRunning(id))
      return;     // Instance is stopped, nothing to do

   boost::shared_ptr<CInstance> instance(m_runningInstances.find(id)->second);

   YADOMS_LOG(debug) << "Send command " << command->toString() << " to plugin " << instance->getName();

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

void CManager::recordInstanceStarted(int id)
{
   //TODO positionner le keyword state
   //boost::shared_ptr<database::entities::CPlugin> instanceData(boost::make_shared<database::entities::CPlugin>());
   //instanceData->Id = id;
   //instanceData->State = database::entities::ERuleState::kRunning;
   //instanceData->StartDate = shared::currentTime::Provider::now();
   //instanceData->ErrorMessage = std::string();
   //m_pluginDBTable->updateInstance(instanceData);
}

void CManager::recordInstanceStopped(int id, const std::string& error)
{
   //TODO positionner le keyword state
   //boost::shared_ptr<database::entities::CRule> ruleData(new database::entities::CRule);
   //ruleData->Id = id;
   //ruleData->State = error.empty() ? database::entities::ERuleState::kStopped : database::entities::ERuleState::kError;
   //ruleData->StopDate = shared::currentTime::Provider::now();
   //ruleData->ErrorMessage = error;
   //m_ruleRequester->updateRule(ruleData);
}

} // namespace pluginSystem
