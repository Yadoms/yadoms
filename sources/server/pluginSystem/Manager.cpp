#include "stdafx.h"

#include "Manager.h"
#ifdef _DEBUG
#include "BasicQualifier.h"
#else
#include "IndicatorQualifier.h"
#endif
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/EmptyResult.hpp>
#include <shared/exception/JSONParse.hpp>
#include <utility>
#include <shared/plugin/yPluginApi/historization/PluginState.h>

#include "Factory.h"

#include <shared/Log.h>
#include "PluginException.hpp"
#include "InvalidPluginException.hpp"
#include "DeviceConfigurationSchemaRequest.h"
#include "SetDeviceConfiguration.h"
#include "DeviceRemoved.h"
#include "task/plugins/ExtraQuery.h"

namespace pluginSystem
{
   CManager::CManager(boost::shared_ptr<const IPathProvider> pathProvider,
                      const shared::versioning::CSemVer& yadomsVersion,
                      const boost::shared_ptr<database::IDataProvider>& dataProvider,
                      const boost::shared_ptr<dataAccessLayer::IDataAccessLayer>& dataAccessLayer,
                      boost::shared_ptr<shared::ILocation> location,
                      boost::shared_ptr<task::CScheduler> taskScheduler)
      : m_factory(boost::make_shared<CFactory>(pathProvider,
                                               yadomsVersion,
                                               location)),
        m_dataProvider(dataProvider),
        m_pluginDBTable(dataProvider->getPluginRequester()),
#ifdef _DEBUG
        m_qualifier(boost::make_shared<CBasicQualifier>(dataProvider->getPluginEventLoggerRequester(),
                                                        dataAccessLayer->getEventLogger())),
#else
      m_qualifier(boost::make_shared<CIndicatorQualifier>(dataProvider->getPluginEventLoggerRequester(), dataAccessLayer->getEventLogger())),
#endif
        m_dataAccessLayer(dataAccessLayer),
        m_taskScheduler(std::move(taskScheduler))
   {
   }

   CManager::~CManager()
   {
      try
      {
         stop();
      }
      catch (const std::exception& e)
      {
         YADOMS_LOG(error) << "Fail to stop plugin manager, " << e.what();
      }
      catch (...)
      {
         YADOMS_LOG(error) << "Fail to stop plugin manager, unknown error";
      }
   }

   void CManager::start(const boost::posix_time::time_duration& timeout)
   {
      std::set<int> startedInstanceIds;

      // Start all plugin instances
      YADOMS_LOG(information) << "Start all plugin instances...";
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

         // Initialize the plugin list (detect available plugins)
         updatePluginList();

         startAllInstances(startedInstanceIds);
      }

      // Wait for instances started or timeout
      YADOMS_LOG(debug) << "All plugin instances are started, wait for running...";
      enum
      {
         kStartInstancesTimeoutId = shared::event::kUserFirstId
      };
      shared::event::CEventHandler startInstancesEventHandler;
      startInstancesEventHandler.createTimePoint(kStartInstancesTimeoutId,
                                                 shared::currentTime::Provider().now() + timeout);
      while (true)
      {
         switch (startInstancesEventHandler.waitForEvents(boost::posix_time::seconds(2)))
         {
         case kStartInstancesTimeoutId:
            YADOMS_LOG(warning) << "Timeout starting plugin instances : some instance(s) are not running";
            return;
         case shared::event::kTimeout: // Every 2 seconds
            for (auto it = startedInstanceIds.begin(); it != startedInstanceIds.end();)
            {
               if (getInstanceState(*it) == shared::plugin::yPluginApi::historization::EPluginState::kRunning)
               {
                  YADOMS_LOG(debug) << "Plugin instance " << *it << " is running";
                  it = startedInstanceIds.erase(it);
               }
               else
                  ++it;
            }
            if (startedInstanceIds.empty())
            {
               YADOMS_LOG(information) << "All started instances are now running";
               return;
            }
            break;
         default: break;
         }
      }
   }

   void CManager::stop()
   {
      stopInstances();
   }

   void CManager::startAllInstances(std::set<int>& startedInstanceIds)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
      if (!m_runningInstances.empty())
         throw shared::exception::CException(
            "Some plugins are already started, are you sure that manager was successfully stopped ?");

      if (!startInstances(getInstanceList(),
                          startedInstanceIds))
         YADOMS_LOG(error) << "One or more plugins failed to start, check plugins page for details";

      //start the internal plugin
      startInternalPlugin();
   }

   bool CManager::startInstances(const std::vector<boost::shared_ptr<database::entities::CPlugin>>& instances,
                                 std::set<int>& startedInstanceIds)
   {
      auto allInstancesStarted = true;
      for (const auto& instance : instances)
      {
         try
         {
            if (instance->Category() != database::entities::EPluginCategory::kSystem)
               if (instance->AutoStart())
               {
                  YADOMS_LOG(debug) << "Start plugin instance " << instance->Id() << "...";
                  startInstance(instance->Id());
                  startedInstanceIds.insert(instance->Id());
               }
         }
         catch (CPluginException& ex)
         {
            YADOMS_LOG(error) << "Unable to start plugin " << instance->DisplayName() << "(" << ex.what() << "), skipped";
            allInstancesStarted = false;
         }
      }

      return allInstancesStarted;
   }

   void CManager::updatePluginList()
   {
      // Plugin directory have changed, so re-build available plugins list

      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
      boost::lock_guard<boost::recursive_mutex> lock2(m_availablePluginsMutex);
      m_availablePlugins = m_factory->findAvailablePlugins();
   }


   IFactory::AvailablePluginMap CManager::getPluginList() const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_availablePluginsMutex);
      return m_availablePlugins;
   }

   int CManager::getPluginQualityIndicator(const std::string& pluginName) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_availablePluginsMutex);

      if (m_availablePlugins.find(pluginName) == m_availablePlugins.end())
         throw CInvalidPluginException(pluginName); // Invalid plugin

      return m_qualifier->getQualityLevel(m_availablePlugins.at(pluginName));
   }

   int CManager::createInstance(const database::entities::CPlugin& data)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      // First step, record instance in database, to get its ID
      const auto instanceId = m_pluginDBTable->addInstance(data);

      // Next create instance
      startInstance(instanceId);

      return instanceId;
   }

   void CManager::deleteInstance(int id)
   {
      try
      {
         auto instanceData = m_pluginDBTable->getInstance(id);
         if (instanceData->Category() == database::entities::EPluginCategory::kSystem)
            return;

         // Stop plugin instance
         stopInstance(id);

         // Remove in database
         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
         m_dataAccessLayer->getDeviceManager()->removeAllDeviceForPlugin(id);
         m_pluginDBTable->removeInstance(id);

         // Remove logs
         try
         {
            remove_all(m_factory->pluginLogFile(id).parent_path());
         }
         catch (boost::filesystem::filesystem_error& ex)
         {
            YADOMS_LOG(warning) << "Unable to remove plugin log file " << ex.path1() << " : " << ex.what();
         }

         // Remove data path
         try
         {
            remove_all(m_factory->pluginDataPath(id));
         }
         catch (boost::filesystem::filesystem_error& ex)
         {
            YADOMS_LOG(warning) << "Unable to remove plugin instance data path " << ex.path1() << " : " << ex.what();
         }
      }
      catch (shared::exception::CException& e)
      {
         YADOMS_LOG(error) << "Unable to delete plugin (" << id << ") : " << e.what();
         throw shared::exception::CInvalidParameter(std::to_string(id));
      }
   }

   std::string CManager::getInstanceLog(int id) const
   {
      try
      {
         //ensure instance exists
         auto instanceData = m_pluginDBTable->getInstance(id);

         const auto logFile(m_factory->pluginLogFile(id));

         if (!exists(logFile))
            throw shared::exception::CInvalidParameter(logFile.string());

         std::ifstream file(logFile.string().c_str());
         if (!file.is_open())
            throw shared::exception::CInvalidParameter(logFile.string());

         const std::istreambuf_iterator<char> eos;
         return std::string(std::istreambuf_iterator<char>(file), eos);
      }
      catch (shared::exception::CException& e)
      {
         YADOMS_LOG(error) << "Unable to get instance log for id=(" << id << ") : " << e.what();
         throw shared::exception::CInvalidParameter(std::to_string(id));
      }
   }

   std::vector<boost::shared_ptr<database::entities::CPlugin>> CManager::getInstanceList() const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
      return m_pluginDBTable->getInstances();
   }

   boost::shared_ptr<database::entities::CPlugin> CManager::getInstance(int id) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
      return m_pluginDBTable->getInstance(id);
   }

   boost::shared_ptr<IInstance> CManager::getRunningInstance(int id) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      const auto instance = m_runningInstances.find(id);
      if (instance == m_runningInstances.end())
         throw CPluginException((boost::format("Plugin instance #%1% is not running") % id).str());

      return instance->second;
   }

   void CManager::updateInstance(const database::entities::CPlugin& newData)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      // Check for supported modifications
      if (!newData.Id.isDefined())
         throw shared::exception::CException("Update instance : instance ID was not provided");

      // First get old configuration from database
      const auto previousData = m_pluginDBTable->getInstance(newData.Id());

      // Next, update configuration in database
      m_pluginDBTable->updateInstance(newData);

      // Last, apply modifications
      if (newData.Configuration.isDefined()
         && previousData->Configuration() != newData.Configuration())
         // No need to notify configuration if instance was just enabled/disabled
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
      auto allInstances = getInstanceList();
      for (const auto& instance : allInstances)
      {
         if (boost::iequals(instance->Type(), pluginName) && instance->AutoStart())
            instancesToStart.push_back(instance->Id());
      }

      // Start all instances of this plugin
      for (const auto instanceToStart : instancesToStart)
         startInstance(instanceToStart);
   }

   std::vector<int> CManager::stopAllInstancesOfPlugin(const std::string& pluginName)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      // Find instances to stop
      std::vector<int> instancesToStop;
      for (const auto& instance : m_runningInstances)
      {
         if (instance.second && boost::iequals(instance.second->aboutPlugin()->getType(), pluginName))
            instancesToStop.push_back(instance.first);
      }

      // Stop all instances of this plugin
      for (const auto& instanceToStop : instancesToStop)
         requestStopInstance(instanceToStop);

      return instancesToStop;
   }

   void CManager::stopAllInstancesOfPluginAndWaitForStopped(const std::string& pluginName,
                                                            const boost::posix_time::time_duration& timeout)
   {
      YADOMS_LOG(debug) << "Stop all instances of " << pluginName << "...";
      auto instancesToStop = stopAllInstancesOfPlugin(pluginName);

      const auto endTime = shared::currentTime::Provider().now() + timeout;
      auto someInstancesStillRunning = true;
      while (someInstancesStillRunning && shared::currentTime::Provider().now() <= endTime)
      {
         //if any instance is still running just wait for another second
         someInstancesStillRunning = std::any_of(instancesToStop.begin(), instancesToStop.end(),
                                                 [this](int instance) { return this->isInstanceRunning(instance); });
         if (someInstancesStillRunning)
            boost::this_thread::sleep(boost::posix_time::seconds(1));
      }

      if (someInstancesStillRunning)
      {
         YADOMS_LOG(error) << "Unable to stop all instances of plugin " << pluginName << ", try to kill...";
         for (const auto instance : instancesToStop)
            if (isInstanceRunning(instance))
               killInstance(instance);

         boost::this_thread::sleep(boost::posix_time::seconds(1));

         // Check
         someInstancesStillRunning = std::any_of(instancesToStop.begin(), instancesToStop.end(),
                                                 [this](int instance) { return this->isInstanceRunning(instance); });
         if (someInstancesStillRunning)
            throw std::runtime_error((boost::format("Unable to stop all instances of plugin %1%") % pluginName).str());
      }

      YADOMS_LOG(debug) << "All instances of " << pluginName << " are stopped";
   }

   void CManager::notifyDeviceRemoved(int deviceId) const
   {
      try
      {
         if (!m_dataProvider->getDeviceRequester()->deviceExists(deviceId))
            return;

         if (m_dataProvider->getDeviceRequester()->isDeviceBlacklisted(deviceId))
            return;

         const auto device = m_dataProvider->getDeviceRequester()->getDevice(deviceId);
         const auto pluginInstanceId = device->PluginId();

         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
         auto instance(getRunningInstance(pluginInstanceId));

         YADOMS_LOG(debug) << "Send removed device notification on device " << device->Name() << " to plugin " <<
            instance->about()->DisplayName();

         if (instance->aboutPlugin()->getSupportDeviceRemovedNotification())
            instance->postDeviceRemoved(boost::make_shared<CDeviceRemoved>(device->Name(),
                                                                           device->Details()));
      }
      catch (CPluginException&)
      {
         // Plugin instance is not running
      }
   }

   void CManager::startInstance(int id)
   {
      try
      {
         auto instanceData(getInstance(id));

         if (isInstanceRunning(instanceData->Id()))
            return; // Already started ==> nothing more to do

         YADOMS_LOG(information) << "Start plugin instance " << instanceData->DisplayName();

         // Create instance
         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
         m_runningInstances[instanceData->Id()] = m_factory->createInstance(instanceData,
                                                                            m_dataProvider,
                                                                            m_dataAccessLayer,
                                                                            m_qualifier,
                                                                            [this](int pluginInstanceId)
                                                                            {
                                                                               onPluginStopped(pluginInstanceId);
                                                                            });
      }
      catch (shared::exception::CEmptyResult& e)
      {
         std::ostringstream oss;
         oss << "Invalid plugin instance " << id << ", element not found in database : " << e.what();
         YADOMS_LOG(error) << oss.str();
         throw CPluginException(oss.str());
      }
      catch (CInvalidPluginException& e)
      {
         std::ostringstream oss;
         oss << "Invalid plugin instance " << id << ", because of an invalid plugin : " << e.what();
         YADOMS_LOG(error) << oss.str();
         throw CPluginException(oss.str());
      }
      catch (shared::exception::COutOfRange& e)
      {
         std::ostringstream oss;
         oss << "Invalid plugin instance " << id << "configuration, out of range : " << e.what();
         YADOMS_LOG(error) << oss.str();
         throw CPluginException(oss.str());
      }
      catch (std::exception& e)
      {
         std::ostringstream oss;
         oss << "Unable to start instance " << id << " : " << e.what();
         YADOMS_LOG(error) << oss.str();
         throw CPluginException(oss.str());
      }
      catch (...)
      {
         std::ostringstream oss;
         oss << "Unable to start instance " << id << ", unknown error";
         YADOMS_LOG(error) << oss.str();
         throw CPluginException(oss.str());
      }
   }

   void CManager::stopInstances()
   {
      YADOMS_LOG(information) << "pluginSystem::CManager stop plugins...";

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
         for (const auto& runningInstance : m_runningInstances)
            requestStopInstance(runningInstance.first);
      }

      const auto timeout = shared::currentTime::Provider().now() + boost::posix_time::seconds(30);
      do
      {
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
            if (m_runningInstances.empty())
               break;
         }
         boost::this_thread::yield();
      }
      while (shared::currentTime::Provider().now() < timeout);

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
         if (!m_runningInstances.empty())
         {
            for (const auto& instance : m_runningInstances)
               instance.second->kill();
         }
      }

      YADOMS_LOG(information) << "pluginSystem::CManager all plugins are stopped";
   }

   void CManager::requestStopInstance(int id)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      auto instance = m_runningInstances.find(id);

      if (instance == m_runningInstances.end())
         return; // Already stopped ==> nothing more to do

      instance->second->requestStop();
   }

   void CManager::killInstance(int id)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      auto instance = m_runningInstances.find(id);

      if (instance == m_runningInstances.end())
         return; // Already stopped ==> nothing more to do

      instance->second->kill();
   }

   void CManager::stopInstance(int id)
   {
      if (!isInstanceRunning(id))
         return;

      requestStopInstance(id);

      auto timeout = shared::currentTime::Provider().now() + boost::posix_time::seconds(10);
      do
      {
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
            if (m_runningInstances.find(id) == m_runningInstances.end())
               break;
         }
         boost::this_thread::yield();
      }
      while (shared::currentTime::Provider().now() < timeout);

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
         if (m_runningInstances.find(id) != m_runningInstances.end())
         {
            YADOMS_LOG(warning) << "pluginSystem::CManager, instance #" << id <<
               " didn't stop when requested. Will be killed.";
            killInstance(id);
         }
      }

      timeout = shared::currentTime::Provider().now() + boost::posix_time::seconds(20);
      do
      {
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
            if (m_runningInstances.find(id) == m_runningInstances.end())
               break;
         }
         boost::this_thread::yield();
      }
      while (shared::currentTime::Provider().now() < timeout);

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
         if (m_runningInstances.find(id) != m_runningInstances.end())
            throw CPluginException((boost::format("pluginSystem::CManager, unable to stop instance #%1%") % id).str());
      }
   }

   void CManager::onPluginStopped(int pluginInstanceId)
   {
      boost::thread([this, pluginInstanceId]()
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

         const auto pluginInstance = m_runningInstances.find(pluginInstanceId);
         if (pluginInstance != m_runningInstances.end())
            m_runningInstances.erase(pluginInstance);
      });
   }

   void CManager::startInternalPlugin()
   {
      startInstance(m_pluginDBTable->getSystemInstance()->Id());
   }

   bool CManager::isInstanceRunning(int id) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
      return m_runningInstances.find(id) != m_runningInstances.end();
   }

   boost::shared_ptr<shared::CDataContainer> CManager::getInstanceFullState(int id) const
   {
      if (!isInstanceRunning(id))
      {
         // Instance is not running, so can be in error or stopped state
         boost::shared_ptr<database::entities::CDevice> device;
         try
         {
            // First find the pluginState device associated with the plugin
            device = m_dataProvider->getDeviceRequester()->getDeviceInPlugin(id, "pluginState");
         }
         catch (shared::exception::CEmptyResult&)
         {
            // Device doesn't exist, probably not supported by plugin. Plugin is then considered as stopped.
            const auto defaultState = shared::CDataContainer::make();
            defaultState->set("state", shared::plugin::yPluginApi::historization::EPluginState::kStopped);
            return defaultState;
         }

         try
         {
            const auto stateKw = m_dataProvider->getKeywordRequester()->getKeyword(device->Id, "state");
            const shared::plugin::yPluginApi::historization::EPluginState state(
               m_dataProvider->getKeywordRequester()->getKeywordLastData(stateKw->Id));
            if (state == shared::plugin::yPluginApi::historization::EPluginState::kError)
            {
               // In error state
               const auto customMessageIdKw = m_dataProvider
                                              ->getKeywordRequester()->getKeyword(device->Id, "customMessageId");
               const auto defaultState = shared::CDataContainer::make();
               defaultState->set("state", state);

               try
               {
                  const auto dc = shared::CDataContainer::make(
                     m_dataProvider->getKeywordRequester()->getKeywordLastData(customMessageIdKw->Id));
                  defaultState->set("messageId", dc->getWithDefault("messageId", std::string()));
                  defaultState->set("messageData", dc->getWithDefault("messageData", std::string()));
               }
               catch (shared::exception::CJSONParse&)
               {
                  // Last plugin state is not JSON-formatted, probably pure text
                  defaultState->set("messageId",
                                    m_dataProvider->getKeywordRequester()->getKeywordLastData(customMessageIdKw->Id));
               }
               return defaultState;
            }

            // Normal stop
            const auto defaultState = shared::CDataContainer::make();
            defaultState->set("state", shared::plugin::yPluginApi::historization::EPluginState::kStopped);
            return defaultState;
         }
         catch (shared::exception::CEmptyResult&)
         {
            // pluginState keyword exist, but was never historized, so considered as stopped.
            const auto defaultState = shared::CDataContainer::make();
            defaultState->set("state", shared::plugin::yPluginApi::historization::EPluginState::kStopped);
            return defaultState;
         }
      }

      // Instance is running
      boost::shared_ptr<database::entities::CDevice> device;
      try
      {
         // First find the pluginState device associated with the plugin
         device = m_dataProvider->getDeviceRequester()->getDeviceInPlugin(id, "pluginState");
      }
      catch (shared::exception::CEmptyResult&)
      {
         // Device doesn't exist, probably not supported by plugin. Plugin is then considered as running.
         const auto defaultState = shared::CDataContainer::make();
         defaultState->set("state", shared::plugin::yPluginApi::historization::EPluginState::kRunning);
         return defaultState;
      }

      try
      {
         const auto stateKw = m_dataProvider->getKeywordRequester()->getKeyword(device->Id, "state");
         const auto customMessageIdKw = m_dataProvider
                                        ->getKeywordRequester()->getKeyword(device->Id, "customMessageId");
         const auto defaultState = shared::CDataContainer::make();
         defaultState->set("state", m_dataProvider->getKeywordRequester()->getKeywordLastData(stateKw->Id));

         try
         {
            const auto dc = shared::CDataContainer::make(
               m_dataProvider->getKeywordRequester()->getKeywordLastData(customMessageIdKw->Id));
            defaultState->set("messageId", dc->getWithDefault("messageId", std::string()));
            defaultState->set("messageData", dc->getWithDefault("messageData", std::string()));
         }
         catch (shared::exception::CJSONParse&)
         {
            // Last plugin state is not JSON-formatted, probably pure text
            defaultState->set("messageId",
                              m_dataProvider->getKeywordRequester()->getKeywordLastData(customMessageIdKw->Id));
         }

         return defaultState;
      }
      catch (shared::exception::CEmptyResult&)
      {
         // pluginState keyword exist, but was never historized, so considered as unknown.
         const auto defaultState = shared::CDataContainer::make();
         defaultState->set("state", shared::plugin::yPluginApi::historization::EPluginState::kUnknown);
         return defaultState;
      }
   }

   shared::plugin::yPluginApi::historization::EPluginState CManager::getInstanceState(int id) const
   {
      const auto fullState = getInstanceFullState(id);
      return fullState->get<shared::plugin::yPluginApi::historization::EPluginState>("state");
   }

   void CManager::postCommand(int id,
                              const boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand>& command) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
      auto instance(getRunningInstance(id));

      YADOMS_LOG(debug) << "Send command " << shared::plugin::yPluginApi::IDeviceCommand::toString(command) <<
         " to plugin " << instance->about()->DisplayName();

      instance->postDeviceCommand(command);
   }

   std::string CManager::postExtraQuery(int id, boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> query) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
      auto instance(getRunningInstance(id));

      YADOMS_LOG(debug) << "Send extra query " << query->getData()->query() << " to plugin " << instance->about()->DisplayName();

      const auto task(boost::make_shared<task::plugins::CExtraQuery>(instance, query));

      std::string taskUid;
      const auto result = m_taskScheduler->runTask(task, taskUid);
      if (!result)
      {
         YADOMS_LOG(error) << "Fail to send extra query " << query->getData()->query() << " to plugin " <<
            instance->about()->DisplayName();
         //ensure taskId is set to ""
         taskUid = "";
      }
      return taskUid;
   }

   void CManager::postManuallyDeviceCreationRequest(int id,
                                                    boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest>& request) const
   {
      try
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
         auto instance = getRunningInstance(id);

         YADOMS_LOG(debug) << "Send manually device (" << request->getData().getDeviceName() << ") creation request " <<
            " to plugin " << instance->about()->DisplayName();

         instance->postManuallyDeviceCreationRequest(request);
      }
      catch (CPluginException& e)
      {
         request->sendError((boost::format("Error when requesting binding query %1%") % e.what()).str());
      }
   }

   void CManager::postBindingQueryRequest(int id,
                                          boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest>& request)
   const
   {
      try
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
         auto instance = getRunningInstance(id);

         YADOMS_LOG(debug) << "Send binding query " << request->getData().getQuery() << " to plugin " << instance
                                                                                                         ->about()->
                                                                                                         DisplayName();

         instance->postBindingQueryRequest(request);
      }
      catch (CPluginException& e)
      {
         request->sendError((boost::format("Error when requesting binding query %1%") % e.what()).str());
      }
   }

   void CManager::postDeviceConfigurationSchemaRequest(int deviceId,
                                                       communication::callback::ISynchronousCallback<boost::shared_ptr<
                                                          shared::CDataContainer>>& callback) const
   {
      auto device = m_dataAccessLayer->getDeviceManager()->getDevice(deviceId);

      boost::shared_ptr<shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest> request(
         boost::make_shared<CDeviceConfigurationSchemaRequest>(device->Name(),
                                                               callback));

      try
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
         auto instance = getRunningInstance(device->PluginId);

         YADOMS_LOG(debug) << "Send DeviceConfigurationSchema request on device \"" << device->Name() << "\" to plugin "
            << instance->about()->DisplayName();

         instance->postDeviceConfigurationSchemaRequest(request);
      }
      catch (CPluginException& e)
      {
         request->sendError(
            (boost::format("Error when requesting DeviceConfigurationSchema on device %1% : %2%") % device->Name() % e.
               what()).str());
      }
   }

   void CManager::postSetDeviceConfiguration(int deviceId,
                                             const boost::shared_ptr<shared::CDataContainer>& configuration) const
   {
      const auto device = m_dataAccessLayer->getDeviceManager()->getDevice(deviceId);

      const auto command(boost::make_shared<CSetDeviceConfiguration>(device->Name(),
                                                                     configuration));

      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
      const auto instance(getRunningInstance(device->PluginId()));

      YADOMS_LOG(debug) << "Set configuration to device \"" << device->Name() << "\" to plugin " <<
         instance->about()->DisplayName();

      instance->postSetDeviceConfiguration(command);
   }
} // namespace pluginSystem
