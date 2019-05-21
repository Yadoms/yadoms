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
                          boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
                          communication::ISendMessageAsync& messageSender,
                          bool developerMode)
            : m_dataProvider(dataProvider),
              m_pluginManager(pluginManager),
              m_deviceManager(deviceManager),
              m_restKeyword("plugin"),
              m_messageSender(messageSender),
              m_developerMode(developerMode)
         {
         }

         const std::string& CPlugin::getRestKeyword() const
         {
            return m_restKeyword;
         }

         void CPlugin::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CPlugin::getAllAvailablePlugins);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword), CPlugin::getAllAvailablePluginsParameterized);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("withPackage"), CPlugin::getAllAvailablePluginsWithPackage);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("instance"), CPlugin::getAllPluginsInstance);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("instanceWithState"), CPlugin::getAllPluginsInstanceWithState);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("instance")("handleManuallyDeviceCreation"), CPlugin::
               getAllPluginsInstanceForManualDeviceCreation);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CPlugin::getOnePlugin);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("state"), CPlugin::getInstanceState);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("devices"), CPlugin::getPluginDevices);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("log"), CPlugin::getInstanceLog);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("binding")("*"), CPlugin::getBinding);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("start"), CPlugin::startInstance);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("stop"), CPlugin::stopInstance);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("instanceRunning"), CPlugin::getInstanceRunning);

            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CPlugin::createPlugin, CPlugin::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("createDevice"), CPlugin::createDevice, CPlugin::
               transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CPlugin::updatePlugin, CPlugin::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("extraQuery")("*"), CPlugin::sendExtraQuery, CPlugin
               ::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("deviceExtraQuery")("*")("*"), CPlugin::
               sendDeviceExtraQuery, CPlugin::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CPlugin::deleteAllPlugins, CPlugin::transactionalMethod
            );
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CPlugin::deletePlugin, CPlugin::
               transactionalMethod);
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
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);
                  return CResult::GenerateSuccess(m_pluginManager->getInstance(instanceId));
               }

               return CResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving one plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllPluginsInstance(const std::vector<std::string>& parameters,
                                                                                                    const std::string& requestContent) const
         {
            shared::CDataContainer t;
            t.set(getRestKeyword(), m_pluginManager->getInstanceList());
            return CResult::GenerateSuccess(t);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllPluginsInstanceWithState(const std::vector<std::string>& parameters,
                                                                                                             const std::string& requestContent) const
         {
            try
            {
               std::vector<shared::CDataContainer> list;
               for (const auto& instance : m_pluginManager->getInstanceList())
               {
                  shared::CDataContainer item;
                  item.set("instance", instance);
                  item.set("state", m_pluginManager->getInstanceFullState(instance->Id()));
                  list.push_back(item);
               }

               shared::CDataContainer result;
               result.set(getRestKeyword(), list);
               return CResult::GenerateSuccess(result);
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllPluginsInstanceForManualDeviceCreation(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            std::vector<boost::shared_ptr<database::entities::CPlugin>> result;

            // List all instances
            auto hwList = m_pluginManager->getInstanceList();

            // Search for manuallyCreatedDevice
            auto pluginList = m_pluginManager->getPluginList();

            for (auto& currentInstance : hwList)
            {
               if (m_pluginManager->isInstanceRunning(currentInstance.get()->Id))
               {
                  const auto matchingInfo = pluginList.find(currentInstance->Type);
                  if (matchingInfo != pluginList.end())
                  {
                     if (matchingInfo->second->getSupportManuallyCreatedDevice())
                        result.push_back(currentInstance);
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllAvailablePluginsParameterized(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               // Get input parameters
               shared::CDataContainer content(requestContent);
               std::vector<std::string> fields;
               if (content.containsChild("fields"))
                  fields = content.get<std::vector<std::string>>("fields");

               // Get all available plugins
               auto pluginList = m_pluginManager->getPluginList();

               // Filter plugins
               std::vector<shared::CDataContainer> pluginCollection;
               for (auto& plugin : pluginList)
               {
                  if (!plugin.second->isSupportedOnThisPlatform())
                     continue;

                  if (!m_developerMode && boost::starts_with(plugin.first, "dev-"))
                     continue;

                  // Filter returned data (fields)
                  if (fields.empty())
                     pluginCollection.push_back(*plugin.second->getPackage());
                  else
                  {
                     auto pluginData = boost::make_shared<shared::CDataContainer>();
                     for (auto& field : fields)
                        pluginData->set(field, plugin.second->getPackage()->get<std::string>(field));

                     pluginCollection.push_back(*pluginData);
                  }
               }

               shared::CDataContainer result;
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllAvailablePluginsWithPackage(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               auto pluginList = m_pluginManager->getPluginList();

               shared::CDataContainer result;
               std::vector<shared::CDataContainer> pluginCollection;
               for (auto& plugin : pluginList)
               {
                  shared::CDataContainer pluginInfo;
                  pluginInfo.set("type", plugin.first);
                  pluginInfo.set("package", *(plugin.second->getPackage()));
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
               const auto idCreated = m_pluginManager->createInstance(plugin);
               return CResult::GenerateSuccess(m_pluginManager->getInstance(idCreated));
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

               return CResult::GenerateSuccess(m_pluginManager->getInstance(instanceToUpdate.Id()));
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
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);
                  const auto query = parameters[3];

                  const auto data = boost::make_shared<pluginSystem::CExtraQueryData>(query,
                                                                                      requestContent.empty() ? shared::CDataContainer() : shared::CDataContainer(requestContent),
                                                                                      "");
                  const auto taskId = m_messageSender.sendExtraQueryAsync(instanceId, data);

                  if (!taskId.empty())
                  {
                     shared::CDataContainer result;
                     result.set("taskId", taskId);
                     return CResult::GenerateSuccess(result);
                  }
                  
                  return CResult::GenerateError("Fail to get extra query task");
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::sendDeviceExtraQuery(
            const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 4)
               {
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);
                  const auto deviceId = boost::lexical_cast<int>(parameters[3]);
                  const auto device = m_dataProvider->getDeviceRequester()->getDevice(deviceId);

                  const auto query = parameters[4];

                  const auto data = boost::make_shared<pluginSystem::CExtraQueryData>(query,
                                                                                requestContent.empty() ? shared::CDataContainer() : shared::CDataContainer(requestContent),
                                                                                device->Name());
                  const auto taskId = m_messageSender.sendExtraQueryAsync(instanceId, data);

                  if (!taskId.empty())
                  {
                     shared::CDataContainer result;
                     result.set("taskId", taskId);
                     return CResult::GenerateSuccess(result);
                  }
                  
                  return CResult::GenerateError("Fail to get extra query task");
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
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);
                  m_pluginManager->deleteInstance(instanceId);
                  return CResult::GenerateSuccess();
               }

               return CResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getInstanceState(
            const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  if (m_pluginManager->getInstance(instanceId))
                     return CResult::GenerateSuccess(m_pluginManager->getInstanceFullState(instanceId));

                  return CResult::GenerateError("invalid parameter. Can not retrieve instance id");
               }

               return CResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getInstanceRunning(
            const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  shared::CDataContainer result;
                  result.set("isRunning", m_pluginManager->isInstanceRunning(instanceId));
                  return CResult::GenerateSuccess(result);
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
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
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  shared::CDataContainer t;
                  t.set("devices", m_dataProvider->getDeviceRequester()->getDevices(instanceId,
                                                                                    true));
                  return CResult::GenerateSuccess(t);
               }

               return CResult::GenerateError("invalid parameter. Can not retrieve plugin instance id in url");
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
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  if (m_pluginManager->getInstance(instanceId))
                  {
                     //start instance
                     m_pluginManager->startInstance(instanceId);
                     //check for instance status
                     if (m_pluginManager->isInstanceRunning(instanceId))
                        return CResult::GenerateSuccess();
                     return CResult::GenerateError("Fail to start the plugin instance");
                  }

                  return CResult::GenerateError("invalid parameter. Can not retrieve instance id");
               }

               return CResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
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
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  const auto pluginInstanceFound = m_pluginManager->getInstance(instanceId);
                  if (!pluginInstanceFound)
                     return CResult::GenerateError("invalid parameter. Can not retrieve instance id");

                  m_pluginManager->stopInstance(instanceId);
                  return CResult::GenerateSuccess();
               }

               return CResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
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
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  shared::CDataContainer result;
                  result.set("log", m_pluginManager->getInstanceLog(instanceId));
                  return CResult::GenerateSuccess(result);
               }

               return CResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
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
                  const auto number = std::stoul(std::string(result[1].first, result[1].second));
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
                  const auto pluginId = boost::lexical_cast<int>(parameters[1]);

				  if(requestContent.empty())
					  return CResult::GenerateError("invalid request content. Must not be empty");

                  shared::CDataContainer content(requestContent);
                  if (!content.exists("name") || !content.exists("type") || !content.exists("configuration"))
                     return CResult::GenerateError("invalid request content. There must be a name, a type and a configuration field");

                  YADOMS_LOG(information) << "Manually device creation request received";

                  const auto& deviceName = generateUniqueDeviceName(pluginId);

                  try
                  {
                     // Declare device
                     const auto& device = m_deviceManager->createDevice(pluginId,
                                                                        deviceName,
                                                                        content.get<std::string>("name"),
                                                                        content.get<std::string>("type"),
                                                                        content.exists("model") && !content.get<std::string>("model").empty()
                                                                           ? content.get<std::string>("name")
                                                                           : content.get<std::string>("model"),
                                                                        shared::CDataContainer());
                     m_dataProvider->getDeviceRequester()->updateDeviceConfiguration(device->Id(),
                                                                                     content.get<shared::CDataContainer>("configuration"));

                     // Send request to plugin
                     communication::callback::CSynchronousCallback<std::string> cb;
                     const pluginSystem::CManuallyDeviceCreationData data(deviceName,
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
                           const auto res = cb.getCallbackResult();

                           if (res.success)
                              return CResult::GenerateSuccess(m_dataProvider->getDeviceRequester()->getDeviceInPlugin(pluginId,
                                                                                                                      device->Name()));

                           // The plugin failed to process manually creation request, we have to remove the just created device
                           m_dataProvider->getDeviceRequester()->removeDevice(device->Id());
                           return CResult::GenerateError(res.errorMessage);
                        }

                     case shared::event::kTimeout:
                        {
                           m_dataProvider->getDeviceRequester()->removeDevice(device->Id());
                           return CResult::GenerateError("The plugin did not respond");
                        }

                     default:
                        {
                           m_dataProvider->getDeviceRequester()->removeDevice(device->Id());
                           return CResult::GenerateError("Unknown plugin result");
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

               return CResult::GenerateError("invalid parameter. Can not retrieve keyword id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving keyword");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getBinding(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 3)
               {
                  const auto pluginId = boost::lexical_cast<int>(parameters[1]);

                  const auto query = parameters[3];
                  try
                  {
                     //create a callback (allow waiting for result)              
                     communication::callback::CSynchronousCallback<shared::CDataContainer> cb;

                     //create the data container to send to plugin
                     const pluginSystem::CBindingQueryData data(query);

                     //send request to plugin
                     m_messageSender.sendBindingQueryRequest(pluginId, data, cb);

                     //wait for result
                     switch (cb.waitForResult())
                     {
                     case communication::callback::CSynchronousCallback<shared::CDataContainer>::kResult:
                        {
                           const auto res = cb.getCallbackResult();

                           if (res.success)
                              return CResult::GenerateSuccess(res.result);

                           return CResult::GenerateError(res.errorMessage);
                        }

                     case shared::event::kTimeout:
                        return CResult::GenerateError("The plugin did not respond");

                     default:
                        return CResult::GenerateError("Unknown plugin result");
                     }
                  }
                  catch (shared::exception::CException& ex)
                  {
                     return CResult::GenerateError(ex);
                  }
               }

               return CResult::GenerateError("invalid parameter. Can not retrieve plugin id in url");
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
