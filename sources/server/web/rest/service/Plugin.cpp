#include "stdafx.h"
#include "Plugin.h"
#include "web/poco/RestResult.h"
#include "pluginSystem/ManuallyDeviceCreationData.h"
#include "pluginSystem/BindingQueryData.h"
#include "pluginSystem/ExtraQueryData.h"
#include "communication/callback/SynchronousCallback.h"
#include <shared/Log.h>

#include <utility>

#include "web/poco/RestDispatcherHelpers.hpp"

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
            : m_dataProvider(std::move(dataProvider)),
              m_pluginManager(std::move(pluginManager)),
              m_deviceManager(std::move(deviceManager)),
              m_restKeyword("plugin"),
              m_messageSender(messageSender),
              m_developerMode(developerMode)
         {
         }

         const std::string& CPlugin::getRestKeyword() const
         {
            return m_restKeyword;
         }

         void CPlugin::configurePocoDispatcher(poco::CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CPlugin::getAllAvailablePluginsV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword), CPlugin::getAllAvailablePluginsParameterizedV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("withPackage"), CPlugin::getAllAvailablePluginsWithPackageV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("instance"), CPlugin::getAllPluginsInstanceV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("instanceWithState"), CPlugin::getAllPluginsInstanceWithStateV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("instance")("handleManuallyDeviceCreation"), CPlugin::
                                        getAllPluginsInstanceForManualDeviceCreationV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CPlugin::getOnePluginV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("state"), CPlugin::getInstanceStateV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("devices"), CPlugin::getPluginDevicesV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("log"), CPlugin::getInstanceLogV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("*")("binding")("*"), CPlugin::getBindingV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("start"), CPlugin::startInstanceV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("stop"), CPlugin::stopInstanceV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("instanceRunning"), CPlugin::getInstanceRunningV1)

            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CPlugin::createPluginV1, CPlugin::transactionalMethodV1)
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("createDevice"), CPlugin::createDeviceV1, CPlugin::
                                                        transactionalMethodV1)
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CPlugin::updatePluginV1,
                                                        CPlugin::transactionalMethodV1)
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("extraQuery")("*"), CPlugin::sendExtraQuery, CPlugin
                                                        ::transactionalMethodV1)
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("deviceExtraQuery")("*")("*"), CPlugin::
                                                        sendDeviceExtraQueryV1, CPlugin::transactionalMethodV1)
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CPlugin::deleteAllPluginsV1,
                                                        CPlugin::transactionalMethodV1
            )
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CPlugin::deletePluginV1, CPlugin::
                                                        transactionalMethodV1)
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::transactionalMethodV1(
            poco::CRestDispatcher::CRestMethodHandler realMethod,
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            const auto pTransactionalEngine = m_dataProvider->getTransactionalEngine();
            boost::shared_ptr<shared::serialization::IDataSerializable> result;
            try
            {
               if (pTransactionalEngine)
                  pTransactionalEngine->transactionBegin();
               result = realMethod(parameters, requestContent);
            }
            catch (std::exception& ex)
            {
               result = poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               result = poco::CRestResult::GenerateError("unknown exception plugin rest method");
            }

            if (pTransactionalEngine)
            {
               if (poco::CRestResult::isSuccess(*boost::dynamic_pointer_cast<shared::CDataContainer>(result)))
                  pTransactionalEngine->transactionCommit();
               else
                  pTransactionalEngine->transactionRollback();
            }
            return result;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getOnePluginV1(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);
                  return poco::CRestResult::GenerateSuccess(m_pluginManager->getInstance(instanceId));
               }

               return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving one plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllPluginsInstanceV1(const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            shared::CDataContainer t;
            t.set(getRestKeyword(), m_pluginManager->getInstanceList());
            return poco::CRestResult::GenerateSuccess(t);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllPluginsInstanceWithStateV1(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               shared::CDataContainer result;
               result.createArray(getRestKeyword());

               for (const auto& instance : m_pluginManager->getInstanceList())
               {
                  shared::CDataContainer item;
                  item.set("instance", instance);
                  item.set("state", m_pluginManager->getInstanceFullState(instance->Id()));
                  result.appendArray(getRestKeyword(), item);
               }

               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in reading plugin instance status");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllPluginsInstanceForManualDeviceCreationV1(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            std::vector<boost::shared_ptr<database::entities::CPlugin>> result;

            // List all instances
            const auto hwList = m_pluginManager->getInstanceList();

            // Search for manuallyCreatedDevice
            auto pluginList = m_pluginManager->getPluginList();

            for (auto& currentInstance : hwList)
            {
               if (m_pluginManager->isInstanceRunning(currentInstance->Id))
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
            return poco::CRestResult::GenerateSuccess(t);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllAvailablePluginsV1(const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               const auto pluginList = m_pluginManager->getPluginList();

               shared::CDataContainer result;
               std::vector<std::string> pluginCollection;

               for (auto& plugin : pluginList)
                  pluginCollection.push_back(plugin.first);

               result.set("plugins", pluginCollection);

               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in creating a new plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllAvailablePluginsParameterizedV1(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               // Get input parameters
               shared::CDataContainer content(requestContent);
               std::vector<std::string> fields;
               if (content.containsChildArray("fields"))
                  fields = content.get<std::vector<std::string>>("fields");

               // Get all available plugins
               auto pluginList = m_pluginManager->getPluginList();

               // Filter plugins

               shared::CDataContainer result;
               result.createArray("plugins");

               for (auto& plugin : pluginList)
               {
                  if (!plugin.second->isSupportedOnThisPlatform())
                     continue;

                  if (!m_developerMode && boost::starts_with(plugin.first, "dev-"))
                     continue;

                  // Filter returned data (fields)
                  if (fields.empty())
                     result.appendArray("plugins", *plugin.second->getPackage());
                  else
                  {
                     auto pluginData = shared::CDataContainer::make();
                     for (auto& field : fields)
                     {
                        const auto package = plugin.second->getPackage();
                        if (!package->exists(field))
                           continue;
                        if (package->containsChild(field))
                           pluginData->set(field, package->getChild(field));
                        else if (package->containsValue(field))
                           pluginData->set(field, package->get<std::string>(field));
                     }

                     result.appendArray("plugins", *pluginData);
                  }
               }

               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in creating a new plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getAllAvailablePluginsWithPackageV1(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               auto pluginList = m_pluginManager->getPluginList();

               shared::CDataContainer result;
               result.createArray("plugins");
               for (auto& plugin : pluginList)
               {
                  shared::CDataContainer pluginInfo;
                  pluginInfo.set("type", plugin.first);
                  pluginInfo.set("package", *(plugin.second->getPackage()));
                  result.appendArray("plugins", pluginInfo);
               }

               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in creating a new plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::createPluginV1(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            try
            {
               database::entities::CPlugin plugin;
               plugin.fillFromSerializedString(requestContent);
               const auto idCreated = m_pluginManager->createInstance(plugin);
               return poco::CRestResult::GenerateSuccess(m_pluginManager->getInstance(idCreated));
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in creating a new plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::updatePluginV1(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            try
            {
               database::entities::CPlugin instanceToUpdate;
               instanceToUpdate.fillFromSerializedString(requestContent);

               m_pluginManager->updateInstance(instanceToUpdate);

               return poco::CRestResult::GenerateSuccess(m_pluginManager->getInstance(instanceToUpdate.Id()));
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in updating a plugin instance");
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
                                                                                      requestContent.empty()
                                                                                         ? shared::CDataContainer::make()
                                                                                         : shared::CDataContainer::make(requestContent),
                                                                                      "");
                  const auto taskId = m_messageSender.sendExtraQueryAsync(instanceId, data);

                  if (!taskId.empty())
                  {
                     shared::CDataContainer result;
                     result.set("taskId", taskId);
                     return poco::CRestResult::GenerateSuccess(result);
                  }

                  return poco::CRestResult::GenerateError("Fail to get extra query task");
               }
               return poco::CRestResult::GenerateError("invalid parameter. Not enough parameters in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in sending extra query to plugin");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::sendDeviceExtraQueryV1(
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
                                                                                      requestContent.empty()
                                                                                         ? shared::CDataContainer::make()
                                                                                         : shared::CDataContainer::make(requestContent),
                                                                                      device->Name());
                  const auto taskId = m_messageSender.sendExtraQueryAsync(instanceId, data);

                  if (!taskId.empty())
                  {
                     shared::CDataContainer result;
                     result.set("taskId", taskId);
                     return poco::CRestResult::GenerateSuccess(result);
                  }

                  return poco::CRestResult::GenerateError("Fail to get extra query task");
               }
               return poco::CRestResult::GenerateError("invalid parameter. Not enough parameters in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in sending extra query to plugin");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::deletePluginV1(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);
                  m_pluginManager->deleteInstance(instanceId);
                  return poco::CRestResult::GenerateSuccess();
               }

               return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in deleting one plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::deleteAllPluginsV1(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const
         {
            auto hwList = m_pluginManager->getInstanceList();

            while (!m_pluginManager->getInstanceList().empty())
               m_pluginManager->deleteInstance(m_pluginManager->getInstanceList().front()->Id());

            return poco::CRestResult::GenerateSuccess();
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getInstanceStateV1(
            const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  if (m_pluginManager->getInstance(instanceId))
                     return poco::CRestResult::GenerateSuccess(m_pluginManager->getInstanceFullState(instanceId));

                  return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve instance id");
               }

               return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in reading plugin instance status");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getInstanceRunningV1(
            const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  shared::CDataContainer result;
                  result.set("isRunning", m_pluginManager->isInstanceRunning(instanceId));
                  return poco::CRestResult::GenerateSuccess(result);
               }
               return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in reading plugin instance running");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getPluginDevicesV1(const std::vector<std::string>& parameters,
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
                  return poco::CRestResult::GenerateSuccess(t);
               }

               return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve plugin instance id in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in reading plugin instance devices");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::startInstanceV1(const std::vector<std::string>& parameters,
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
                        return poco::CRestResult::GenerateSuccess();
                     return poco::CRestResult::GenerateError("Fail to start the plugin instance");
                  }

                  return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve instance id");
               }

               return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in starting plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::stopInstanceV1(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  const auto pluginInstanceFound = m_pluginManager->getInstance(instanceId);
                  if (!pluginInstanceFound)
                     return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve instance id");

                  m_pluginManager->stopInstance(instanceId);
                  return poco::CRestResult::GenerateSuccess();
               }

               return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in stopping plugin instance");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getInstanceLogV1(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto instanceId = boost::lexical_cast<int>(parameters[1]);

                  shared::CDataContainer result;
                  result.set("log", m_pluginManager->getInstanceLog(instanceId));
                  return poco::CRestResult::GenerateSuccess(result);
               }

               return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve instance id in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in get plugin instance log");
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::createDeviceV1(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto pluginId = boost::lexical_cast<int>(parameters[1]);

                  if (requestContent.empty())
                     return poco::CRestResult::GenerateError("invalid request content. Must not be empty");

                  shared::CDataContainer content(requestContent);
                  if (!content.exists("name") || !content.exists("type") || !content.exists("configuration"))
                     return poco::CRestResult::GenerateError("invalid request content. There must be a name, a type and a configuration field");

                  YADOMS_LOG(information) << "Manually device creation request received";

                  const auto deviceName = generateUniqueDeviceName(pluginId);

                  try
                  {
                     // Declare device
                     const auto& device = m_deviceManager->createDevice(pluginId,
                                                                        deviceName,
                                                                        content.get<std::string>("name"),
                                                                        content.get<std::string>("type"),
                                                                        content.exists("model") ? content.get<std::string>("model") : "",
                                                                        shared::CDataContainer::make());
                     m_dataProvider->getDeviceRequester()->updateDeviceConfiguration(device->Id(),
                                                                                     content.get<boost::shared_ptr<shared::CDataContainer>>(
                                                                                        "configuration"));

                     // Send request to plugin
                     communication::callback::CSynchronousCallback<std::string> cb;
                     const pluginSystem::CManuallyDeviceCreationData data(deviceName,
                                                                          content.get<std::string>("type"),
                                                                          content.get<boost::shared_ptr<shared::CDataContainer>>("configuration"));
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
                              return poco::CRestResult::GenerateSuccess(m_dataProvider->getDeviceRequester()->getDeviceInPlugin(pluginId,
                                 device->Name()));

                           // The plugin failed to process manually creation request, we have to remove the just created device
                           m_dataProvider->getDeviceRequester()->removeDevice(device->Id());
                           return poco::CRestResult::GenerateError(res.errorMessage);
                        }

                     case shared::event::kTimeout:
                        {
                           m_dataProvider->getDeviceRequester()->removeDevice(device->Id());
                           return poco::CRestResult::GenerateError("The plugin did not respond");
                        }

                     default:
                        {
                           m_dataProvider->getDeviceRequester()->removeDevice(device->Id());
                           return poco::CRestResult::GenerateError("Unknown plugin result");
                        }
                     }
                  }
                  catch (shared::exception::CException& ex)
                  {
                     if (m_dataProvider->getDeviceRequester()->deviceExists(pluginId,
                                                                            deviceName))
                        m_dataProvider->getDeviceRequester()->removeDevice(pluginId,
                                                                           deviceName);
                     return poco::CRestResult::GenerateError(ex);
                  }
               }

               return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve keyword id in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving keyword");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPlugin::getBindingV1(const std::vector<std::string>& parameters,
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
                     communication::callback::CSynchronousCallback<boost::shared_ptr<shared::CDataContainer>> cb;

                     //create the data container to send to plugin
                     const pluginSystem::CBindingQueryData data(query);

                     //send request to plugin
                     m_messageSender.sendBindingQueryRequest(pluginId, data, cb);

                     //wait for result
                     switch (cb.waitForResult())
                     {
                     case communication::callback::CSynchronousCallback<boost::shared_ptr<shared::CDataContainer>>::kResult:
                        {
                           const auto res = cb.getCallbackResult();

                           if (res.success)
                              return poco::CRestResult::GenerateSuccess(res.result());

                           return poco::CRestResult::GenerateError(res.errorMessage);
                        }

                     case shared::event::kTimeout:
                        return poco::CRestResult::GenerateError("The plugin did not respond");

                     default:
                        return poco::CRestResult::GenerateError("Unknown plugin result");
                     }
                  }
                  catch (shared::exception::CException& ex)
                  {
                     return poco::CRestResult::GenerateError(ex);
                  }
               }

               return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve plugin id in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in executing binding query");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
