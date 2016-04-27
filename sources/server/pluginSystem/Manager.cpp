#include "stdafx.h"//TODO faire ménage dans les include

#include "Manager.h"
#ifdef _DEBUG
#include "BasicQualifier.h"
#else
#include "IndicatorQualifier.h"
#endif
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/EmptyResult.hpp>
#include <shared/plugin/yPluginApi/historization/PluginState.h>

#include "Factory.h"

#include <shared/Log.h>
#include "PluginException.hpp"
#include "InvalidPluginException.hpp"
#include "InstanceRemover.h"

namespace pluginSystem
{
   CManager::CManager(const std::string& initialDir,
                      boost::shared_ptr<database::IDataProvider> dataProvider,
                      boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
                      boost::shared_ptr<shared::event::CEventHandler> supervisor,
                      boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger/*TODO utile ?*/)
      :
      m_factory(boost::make_shared<CFactory>(initialDir)),
      m_dataProvider(dataProvider),
      m_pluginDBTable(dataProvider->getPluginRequester()),
#ifdef _DEBUG //TODO faut-il conserver les qualifiers ?
      m_qualifier(boost::make_shared<CBasicQualifier>(dataProvider->getPluginEventLoggerRequester(), dataAccessLayer->getEventLogger())),//TODO passer par la factory
#else
      m_qualifier(boost::make_shared<CIndicatorQualifier>(dataProvider->getPluginEventLoggerRequester(), dataAccessLayer->getEventLogger())),//TODO passer par la factory
#endif
      m_supervisor(supervisor), m_dataAccessLayer(dataAccessLayer),
      m_instanceRemover(boost::make_shared<InstanceRemover>(m_runningInstancesMutex, m_runningInstances))//TODO passer par la factory ?
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

   bool CManager::startInstances(const std::vector<boost::shared_ptr<database::entities::CPlugin>>& instances)
   {
      auto allInstancesStarted = true;
      for (auto it = instances.begin(); it != instances.end(); ++it)
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
      auto instanceId = m_pluginDBTable->addInstance(data);

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

      auto instance = m_runningInstances.find(id);
      if (instance == m_runningInstances.end())
         throw CPluginException((boost::format("Instance #%1% is not running") % id).str());

      return instance->second;
   }

   void CManager::updateInstance(const database::entities::CPlugin& newData)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      // Check for supported modifications
      if (!newData.Id.isDefined())
         throw shared::exception::CException("Update instance : instance ID was not provided");

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
      std::vector<boost::shared_ptr<database::entities::CPlugin>> allInstances = getInstanceList();
      for (std::vector<boost::shared_ptr<database::entities::CPlugin>>::const_iterator instance = allInstances.begin(); instance != allInstances.end(); ++instance)
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
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      // Find instances to stop
      std::vector<int> instancesToStop;
      for (PluginInstanceMap::const_iterator instance = m_runningInstances.begin(); instance != m_runningInstances.end(); ++instance)
      {
         if (instance->second && boost::iequals(instance->second->aboutPlugin()->getType(), pluginName))
            instancesToStop.push_back(instance->first);
      }

      // Stop all instances of this plugin
      for (std::vector<int>::const_iterator instanceToStop = instancesToStop.begin(); instanceToStop != instancesToStop.end(); ++instanceToStop)
         requestStopInstance(*instanceToStop);
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
                                                                            m_instanceRemover);
      }
      catch (shared::exception::CEmptyResult& e)
      {
         const auto& error((boost::format("Invalid plugin instance %1%, element not found in database : %2%") % id % e.what()).str());
         //TODO utile ?         m_pluginStateHandler->signalError(id, error);
         throw CPluginException(error);
      }
      catch (CInvalidPluginException& e)
      {
         const auto& error((boost::format("Invalid plugin instance %1% configuration, invalid parameter : %2%") % id % e.what()).str());
         //TODO utile ?         m_pluginStateHandler->signalError(id, error);
         throw CPluginException(error);
      }
      catch (shared::exception::COutOfRange& e)
      {
         const auto& error((boost::format("Invalid plugin instance %1% configuration, out of range : %2%") % id % e.what()).str());
         //TODO utile ?         m_pluginStateHandler->signalError(id, error);
         throw CPluginException(error);
      }
      catch (std::exception& e)
      {
         const auto& error((boost::format("Unable to start instance %1% %2%") % id % e.what()).str());
         //TODO utile ?         m_pluginStateHandler->signalError(id, error);
         throw CPluginException(error);
      }
      catch (...)
      {
         const auto& error((boost::format("Unable to start instance %1%, unknown error") % id).str());
         //TODO utile ?         m_pluginStateHandler->signalError(id, error);
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

   void CManager::stopInstanceAndWaitForStopped(int id)
   {
      if (isInstanceRunning(id))
      {
         auto waitForStoppedInstanceHandler(boost::make_shared<shared::event::CEventHandler>());
         m_instanceRemover->addWaiterOn(id, waitForStoppedInstanceHandler);

         requestStopInstance(id);
         if (waitForStoppedInstanceHandler->waitForEvents(boost::posix_time::seconds(10)) == shared::event::kTimeout)
            YADOMS_LOG(warning) << "pluginSystem::CManager, instance #" << id << " didn't stop when requested. Will be killed.";

         killInstance(id);
         if (waitForStoppedInstanceHandler->waitForEvents(boost::posix_time::seconds(10)) == shared::event::kTimeout)
         {
            m_instanceRemover->removeWaiterOn(id, waitForStoppedInstanceHandler);
            throw CPluginException((boost::format("pluginSystem::CManager, unable to stop instance #%1%") % id).str());
         }
         m_instanceRemover->removeWaiterOn(id, waitForStoppedInstanceHandler);
      }
   }

   //TODO à virer ?
   //void CManager::signalEvent(const CManagerEvent& event)
   //{
   //   switch (event.getSubEventId())
   //   {
   //   case CManagerEvent::kPluginInstanceAbnormalStopped:
   //   {
   //      // The thread of an instance has stopped in a non-conventional way (probably crashed)
   //
   //      // First perform the full stop
   //      requestStopInstance(event.getInstanceId());
   //
   //      // Now, evaluate if it is still safe
   //      if (m_qualifier->isSafe(event.aboutPlugin()))
   //      {
   //         // Don't restart if event occurs when instance was stopping
   //         if (!event.getStopping())
   //         {
   //            // Still safe, try to restart it (only if it was a plugin crash. If it's a plugin error, user have to restart plugin manually)
   //            if (getInstanceState(event.getInstanceId()) != shared::plugin::yPluginApi::historization::EPluginState::kError)
   //               startInstance(event.getInstanceId());
   //         }
   //      }
   //      else
   //      {
   //         // Not safe anymore. Disable plugin autostart mode (user will just be able to start it manually)
   //         // Not that this won't stop other instances of this plugin
   //         YADOMS_LOG(warning) << " plugin " << event.aboutPlugin()->getType() << " was evaluated as not safe and will not start automatically anymore.";
   //         m_pluginDBTable->disableAutoStartForAllPluginInstances(event.aboutPlugin()->getType());
   //
   //         // Log this event in the main event logger
   //         m_dataAccessLayer->getEventLogger()->addEvent(database::entities::ESystemEventCode::kPluginDisabled,
   //            event.aboutPlugin()->getIdentity(),
   //            "Plugin " + event.aboutPlugin()->getIdentity() + " was evaluated as not safe and will not start automatically anymore.");
   //      }
   //
   //      break;
   //   }
   //   default:
   //   {
   //      YADOMS_LOG(error) << "Unknown message id";
   //      BOOST_ASSERT(false);
   //      break;
   //   }
   //   }
   //}


   void CManager::startInternalPlugin()
   {
      //TODO revoir la gestion du plugin interne
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      try
      {
         auto databasePluginInstance(m_pluginDBTable->getSystemInstance());

         if (m_runningInstances.find(databasePluginInstance->Id()) != m_runningInstances.end())
            return; // Already started ==> nothing more to do


         // Load the plugin
         //TODO boost::shared_ptr<ILibrary> plugin(boost::make_shared<CInternalPluginLibrary>());//TODO à virer probablement

         // Create instance
         //TODO BOOST_ASSERT(plugin); // Plugin not loaded
         //TODO
         //boost::shared_ptr<IInstance> pluginInstance(m_factory->createInstance(
         //   plugin,
         //   databasePluginInstance, m_dataProvider, m_dataAccessLayer->getDeviceManager(), m_dataAccessLayer->getAcquisitionHistorizer(),
         //   m_qualifier, m_supervisor, 0));
         //m_runningInstances[databasePluginInstance->Id()] = pluginInstance;
      }
      catch (shared::exception::CEmptyResult& e)
      {
         YADOMS_LOG(error) << "startInternalPlugin : unable to find internal plugin : " << e.what();
      }
      catch (CInvalidPluginException& e)
      {
         YADOMS_LOG(error) << "startInternalPlugin : " << e.what();
      }
   }

   void CManager::stopInternalPlugin()
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      //get the plugin info from db
      auto databasePluginInstance(m_pluginDBTable->getSystemInstance());

      // Already stopped ==> nothing more to do
      if (m_runningInstances.find(databasePluginInstance->Id()) == m_runningInstances.end())
         return;

      // Remove (=stop) instance
      m_runningInstances.erase(databasePluginInstance->Id());
   }

   bool CManager::isInstanceRunning(int id) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
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
            auto stateKw = m_dataProvider->getKeywordRequester()->getKeyword(device->Id, "state");
            shared::plugin::yPluginApi::historization::EPluginState state(m_dataProvider->getAcquisitionRequester()->getKeywordLastData(stateKw->Id)->Value());
            if (state == shared::plugin::yPluginApi::historization::EPluginState::kError)
            {
               // In error state
               auto customMessageIdKw = m_dataProvider->getKeywordRequester()->getKeyword(device->Id, "customMessageId");
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
         auto stateKw = m_dataProvider->getKeywordRequester()->getKeyword(device->Id, "state");
         auto customMessageIdKw = m_dataProvider->getKeywordRequester()->getKeyword(device->Id, "customMessageId");
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
      auto fullState = getInstanceFullState(id);
      return fullState.get<shared::plugin::yPluginApi::historization::EPluginState>("state");
   }

   void CManager::postCommand(int id, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
      auto instance(getRunningInstance(id));

      YADOMS_LOG(debug) << "Send command " << command->toString() << " to plugin " << instance->about()->DisplayName();

      //TODO
      //instance->postCommand(command);
   }

   void CManager::postExtraCommand(int id, boost::shared_ptr<const shared::plugin::yPluginApi::IExtraCommand> command)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);
      auto instance(getRunningInstance(id));

      YADOMS_LOG(debug) << "Send extra command " << command->getCommand() << " to plugin " << instance->about()->DisplayName();

      //TODO
      //instance->postExtraCommand(command);
   }

   void CManager::postManuallyDeviceCreationRequest(int id, boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest>& request)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      if (!isInstanceRunning(id))
         return; // Instance is stopped, nothing to do

      //TODO
      //boost::shared_ptr<CInstance> instance(m_runningInstances.find(id)->second);
      //instance->postManuallyDeviceCreationRequest(request);
   }

   void CManager::postBindingQueryRequest(int id, boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest>& request)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

      if (!isInstanceRunning(id))
      {
         request->sendError("Plugin instance is not running");
         return;
      }

      //TODO
      //boost::shared_ptr<CInstance> instance(m_runningInstances.find(id)->second);
      //instance->postBindingQueryRequest(request);


      //TODO pour test
      shared::CDataContainer ev;
      ev.set("HOUR", "1 hour");
      ev.set("DAY", "1 day");
      ev.set("WEEK", "1 week");
      ev.set("MONTH", "1 month");
      ev.set("HALF_YEAR", "6 months");
      ev.set("YEAR", "1 year");

      shared::CDataContainer en;
      en.set("name", "Interval of the chart");
      en.set("description", "Permit to change the interval of all the chart");
      en.set("type", "enum");
      en.set("values", ev);
      en.set("defaultValue", "DAY");

      shared::CDataContainer result;
      result.set("interval", en);

      request->sendSuccess(result);
   }
} // namespace pluginSystem
