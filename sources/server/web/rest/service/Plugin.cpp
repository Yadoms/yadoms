#include "stdafx.h"
#include "Plugin.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"
#include "pluginSystem/ManuallyDeviceCreationData.h"
#include "pluginSystem/BindingQueryData.h"
#include "pluginSystem/ExtraQueryData.h"
#include "communication/callback/SynchronousCallback.h"
#include <shared/Log.h>

namespace web
{
   namespace rest
   {
      namespace service
      {
         CPlugin::CPlugin(boost::shared_ptr<database::IDataProvider> dataProvider,
                          boost::shared_ptr<pluginSystem::CManager> pluginManager,
                          communication::ISendMessageAsync& messageSender)
            : m_dataProvider(dataProvider),
              m_pluginManager(pluginManager),
              m_restKeyword("plugin"),
              m_messageSender(messageSender)
         {
         }


         CPlugin::~CPlugin()
         {
         }

         const std::string& CPlugin::getRestKeyword() const
         {
            return m_restKeyword;
         }

         void CPlugin::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CPlugin::getAllAvailablePlugins);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("withPackage"), CPlugin::getAllAvailablePluginsWithPackage);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("instance"), CPlugin::getAllPluginsInstance);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("instance")("handleManuallyDeviceCreation"), CPlugin::getAllPluginsInstanceForManualDeviceCreation);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CPlugin::getOnePlugin);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("state"), CPlugin::getInstanceState);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("devices"), CPlugin::getPluginDevices);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("log"), CPlugin::getInstanceLog);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("binding")("*"), CPlugin::getBinding);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("start"), CPlugin::startInstance);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("stop"), CPlugin::stopInstance);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("instanceRunning"), CPlugin::getInstanceRunning);

            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CPlugin::createPlugin, CPlugin::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("createDevice"), CPlugin::createDevice, CPlugin::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CPlugin::updatePlugin, CPlugin::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("extraQuery")("*"), CPlugin::sendExtraQuery, CPlugin::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CPlugin::deleteAllPlugins, CPlugin::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CPlugin::deletePlugin, CPlugin::transactionalMethod);
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod,
                                                                                                  const std::vector<std::string>& parameters,
                                                                                                  const std::string& requestContent) const
         {
            auto pTransactionalEngine = m_dataProvider->getTransactionalEngine();
            boost::shared_ptr<shared::serialization::IDataSerializable> result;
            try
            {
               if (pTransactionalEngine)
                  pTransactionalEngine->transactionBegin();
               result = realMethod(parameters, requestContent);
            }
            catch (std::exception& ex)
            {
               result = CResult::GenerateError(ex);
            }
            catch (...)
            {
               result = CResult::GenerateError("unknown exception plugin rest method");
            }

            if (pTransactionalEngine)
            {
               if (CResult::isSuccess(*boost::dynamic_pointer_cast<shared::CDataContainer>(result)))
                  pTransactionalEngine->transactionCommit();
               else
                  pTransactionalEngine->transactionRollback();
            }
            return result;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getOnePlugin(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  auto pluginFound = m_pluginManager->getInstance(instanceId);

                  return CResult::GenerateSuccess(pluginFound);
               }

               return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving one plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllPluginsInstance(const std::vector<std::string>& parameters,
                                                                                                    const std::string& requestContent) const
         {
            auto hwList = m_pluginManager->getInstanceList();
            shared::CDataContainer t;
            t.set(getRestKeyword(), hwList);
            return CResult::GenerateSuccess(t);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllPluginsInstanceForManualDeviceCreation(const std::vector<std::string>& parameters,
                                                                                                                           const std::string& requestContent) const
         {
            std::vector<boost::shared_ptr<database::entities::CPlugin>> result;

            //liste de toutes les instances
            auto hwList = m_pluginManager->getInstanceList();

            //search for manuallyCreatedDevice
            auto pluginList = m_pluginManager->getPluginList();

            for (auto currentInstance = hwList.begin(); currentInstance != hwList.end(); ++currentInstance)
            {
               if (m_pluginManager->isInstanceRunning(currentInstance->get()->Id))
               {
                  auto matchingInfo = pluginList.find((*currentInstance)->Type);
                  if (matchingInfo != pluginList.end())
                  {
                     if (matchingInfo->second->getSupportManuallyCreatedDevice())
                        result.push_back(*currentInstance);
                  }
               }
            }

            //send result
            shared::CDataContainer t;
            t.set(getRestKeyword(), result);
            return CResult::GenerateSuccess(t);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllAvailablePlugins(const std::vector<std::string>& parameters,
                                                                                                     const std::string& requestContent) const
         {
            try
            {
               auto pluginList = m_pluginManager->getPluginList();

               shared::CDataContainer result;
               std::vector<std::string> pluginCollection;

               for (auto plugin = pluginList.begin(); plugin != pluginList.end(); ++plugin)
                  pluginCollection.push_back(plugin->first);

               result.set("plugins", pluginCollection);
               return CResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in creating a new plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllAvailablePluginsWithPackage(const std::vector<std::string>& parameters,
                                                                                                                const std::string& requestContent) const
         {
            try
            {
               auto pluginList = m_pluginManager->getPluginList();

               shared::CDataContainer result;
               std::vector<shared::CDataContainer> pluginCollection;
               for (auto plugin = pluginList.begin(); plugin != pluginList.end(); ++plugin)
               {
                  shared::CDataContainer pluginInfo;
                  pluginInfo.set("type", plugin->first);
                  pluginInfo.set("package", *(plugin->second->getPackage()));
                  pluginCollection.push_back(pluginInfo);
               }

               result.set("plugins", pluginCollection);
               return CResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in creating a new plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::createPlugin(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            try
            {
               database::entities::CPlugin plugin;
               plugin.fillFromSerializedString(requestContent);
               auto idCreated = m_pluginManager->createInstance(plugin);

               auto pluginFound = m_pluginManager->getInstance(idCreated);
               return CResult::GenerateSuccess(pluginFound);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in creating a new plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::updatePlugin(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            try
            {
               database::entities::CPlugin instanceToUpdate;
               instanceToUpdate.fillFromSerializedString(requestContent);

               m_pluginManager->updateInstance(instanceToUpdate);

               auto pluginFound = m_pluginManager->getInstance(instanceToUpdate.Id());
               return CResult::GenerateSuccess(pluginFound);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in updating a plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::sendExtraQuery(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 4)
               {
                  auto instanceId = boost::lexical_cast<int>(parameters[1]);
                  auto query = parameters[3];
                  shared::CDataContainer queryData(requestContent);

                  auto data = boost::make_shared<pluginSystem::CExtraQueryData>(query, queryData);
                  std::string taskId = m_messageSender.sendExtraQueryAsync(instanceId, data);
                  if (!taskId.empty())
                  {
                     shared::CDataContainer result;
                     result.set("taskId", taskId);
                     return CResult::GenerateSuccess(result);
                  }
                  else
                  {
                     return CResult::GenerateError("Fail to get extra query task");
                  }
               }
               return CResult::GenerateError("invalid parameter. Not enough parameters in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in sending extra query to plugin");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::deletePlugin(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  auto instanceId = boost::lexical_cast<int>(parameters[1]);
                  m_pluginManager->deleteInstance(instanceId);
                  return CResult::GenerateSuccess();
               }

               return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in deleting one plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::deleteAllPlugins(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const
         {
            auto hwList = m_pluginManager->getInstanceList();

            while (!m_pluginManager->getInstanceList().empty())
               m_pluginManager->deleteInstance(m_pluginManager->getInstanceList().front()->Id());

            return CResult::GenerateSuccess();
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getInstanceState(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  auto pluginInstanceFound = m_pluginManager->getInstance(instanceId);
                  if (pluginInstanceFound)
                     return CResult::GenerateSuccess(m_pluginManager->getInstanceFullState(instanceId));

                  return CResult::GenerateError("invalid parameter. Can not retreive instance id");
               }

               return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in reading plugin instance status");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getInstanceRunning(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  bool instanceRunning = m_pluginManager->isInstanceRunning(instanceId);
                  shared::CDataContainer result;
                  result.set("isRunning", instanceRunning);
                  return CResult::GenerateSuccess(result);
               }
               return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in reading plugin instance running");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getPluginDevices(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  auto instanceId = boost::lexical_cast<int>(parameters[1]);


                  auto devicesFound = m_dataProvider->getDeviceRequester()->getDevices(instanceId,
                                                                                       true);
                  //send result
                  shared::CDataContainer t;
                  t.set("devices", devicesFound);
                  return CResult::GenerateSuccess(t);
               }

               return CResult::GenerateError("invalid parameter. Can not retreive pluigin instance id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in reading plugin instance devices");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::startInstance(const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  auto pluginInstanceFound = m_pluginManager->getInstance(instanceId);
                  if (pluginInstanceFound)
                  {
                     //start instance
                     m_pluginManager->startInstance(instanceId);
                     //check for instance status
                     auto state = m_pluginManager->isInstanceRunning(instanceId);
                     if (state)
                        return CResult::GenerateSuccess();
                     return CResult::GenerateError("Fail to start the plugin instance");
                  }

                  return CResult::GenerateError("invalid parameter. Can not retreive instance id");
               }

               return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in starting plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::stopInstance(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  auto pluginInstanceFound = m_pluginManager->getInstance(instanceId);
                  if (!pluginInstanceFound)
                     return CResult::GenerateError("invalid parameter. Can not retreive instance id");

                  m_pluginManager->stopInstance(instanceId);
                  return CResult::GenerateSuccess();
               }

               return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in stopping plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getInstanceLog(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  shared::CDataContainer result;
                  result.set("log", m_pluginManager->getInstanceLog(instanceId));
                  return CResult::GenerateSuccess(result);
               }

               return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in get plugin instance log");
            }
         }

         std::string CPlugin::generateUniqueDeviceName(const int pluginId) const
         {
            static const boost::regex DeviceNamePattern("^manuallyCreatedDevice_([[:digit:]]*)$");
            const auto& devices = m_dataProvider->getDeviceRequester()->getDevices(pluginId,
                                                                                   true);
            unsigned int lastNumber = 0;
            for (const auto& device : devices)
            {
               boost::smatch result;
               if (boost::regex_search(device->Name(), result, DeviceNamePattern))
               {
                  auto number = std::stoul(std::string(result[1].first, result[1].second));
                  if (lastNumber < number)
                     lastNumber = number;
               }
            }

            return std::string("manuallyCreatedDevice_") + std::to_string(lastNumber + 1);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::createDevice(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  auto pluginId = boost::lexical_cast<int>(parameters[1]);

                  shared::CDataContainer content(requestContent);
                  if (!content.exists("name") || !content.exists("type") || !content.exists("configuration"))
                     return CResult::GenerateError("invalid request content. There must be a name, a type and a configuration field");

                  YADOMS_LOG(information) << "Manually device creation request received";
                  content.printToLog(YADOMS_LOG(debug));

                  const auto& deviceName = generateUniqueDeviceName(pluginId);

                  try
                  {
                     // Declare device
                     const auto& device = m_dataProvider->getDeviceRequester()->createDevice(pluginId,
                                                                                             deviceName,
                                                                                             content.get<std::string>("name"),
                                                                                             content.get<std::string>("type"),
                                                                                             content.exists("model") && !content.get<std::string>("model").empty() ?
                                                                                                content.get<std::string>("name") : content.get<std::string>("model"),
                                                                                             shared::CDataContainer());
                     m_dataProvider->getDeviceRequester()->updateDeviceConfiguration(device->Id(),
                                                                                     content.get<shared::CDataContainer>("configuration"));

                     // Send request to plugin
                     communication::callback::CSynchronousCallback<std::string> cb;
                     pluginSystem::CManuallyDeviceCreationData data(deviceName,
                                                                    content.get<std::string>("type"),
                                                                    content.get<shared::CDataContainer>("configuration"));
                     m_messageSender.sendManuallyDeviceCreationRequest(pluginId,
                                                                       data,
                                                                       cb);

                     // Wait for result
                     switch (cb.waitForResult())
                     {
                     case communication::callback::CSynchronousCallback<std::string>::kResult:
                        {
                           auto res = cb.getCallbackResult();

                           if (res.Success)
                              return CResult::GenerateSuccess(m_dataProvider->getDeviceRequester()->getDeviceInPlugin(pluginId,
                                                                                                                      device->Name()));

                           // The plugin failed to process manually creation request, we have to remove the just created device
                           m_dataProvider->getDeviceRequester()->removeDevice(device->Id());
                           return CResult::GenerateError(res.ErrorMessage);
                        }

                     case shared::event::kTimeout:
                        {
                           m_dataProvider->getDeviceRequester()->removeDevice(device->Id());
                           return CResult::GenerateError("The plugin did not respond");
                        }

                     default:
                        {
                           m_dataProvider->getDeviceRequester()->removeDevice(device->Id());
                           return CResult::GenerateError("Unkown plugin result");
                        }
                     }
                  }
                  catch (shared::exception::CException& ex)
                  {
                     if (m_dataProvider->getDeviceRequester()->deviceExists(pluginId,
                                                                            deviceName))
                        m_dataProvider->getDeviceRequester()->removeDevice(pluginId,
                                                                           deviceName);
                     return CResult::GenerateError(ex);
                  }
               }

               return CResult::GenerateError("invalid parameter. Can not retreive keyword id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving keyword");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getBinding(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 3)
               {
                  auto pluginId = boost::lexical_cast<int>(parameters[1]);

                  auto query = parameters[3];
                  try
                  {
                     //create a callback (allow waiting for result)              
                     communication::callback::CSynchronousCallback<shared::CDataContainer> cb;

                     //create the data container to send to plugin
                     pluginSystem::CBindingQueryData data(query);

                     //send request to plugin
                     m_messageSender.sendBindingQueryRequest(pluginId, data, cb);

                     //wait for result
                     switch (cb.waitForResult())
                     {
                     case communication::callback::CSynchronousCallback<shared::CDataContainer>::kResult:
                        {
                           auto res = cb.getCallbackResult();

                           if (res.Success)
                              return CResult::GenerateSuccess(res.Result);

                           return CResult::GenerateError(res.ErrorMessage);
                        }

                     case shared::event::kTimeout:
                        return CResult::GenerateError("The plugin did not respond");

                     default:
                        return CResult::GenerateError("Unkown plugin result");
                     }
                  }
                  catch (shared::exception::CException& ex)
                  {
                     return CResult::GenerateError(ex);
                  }
               }

               return CResult::GenerateError("invalid parameter. Can not retreive plugin id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in executing binding query");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 


