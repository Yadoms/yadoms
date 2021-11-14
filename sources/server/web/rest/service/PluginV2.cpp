#include <utility>

#include "stdafx.h"
#include "Plugin.h"
#include "RestEndPoint.h"
#include "communication/callback/SynchronousCallback.h"
#include "pluginSystem/BindingQueryData.h"
#include "pluginSystem/ExtraQueryData.h"
#include "web/rest/CreatedAnswer.h"
#include "web/rest/ErrorAnswer.h"
#include "web/rest/Helpers.h"
#include "web/rest/NoContentAnswer.h"
#include "web/rest/SuccessAnswer.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CPlugin::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "plugins", getAvailablePlugins));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "plugins-instances", getPluginsInstances));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "plugins-instances/{id}", getPluginsInstances));

            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "plugins-instances/create", createPluginsInstance));
            m_endPoints->push_back(MAKE_ENDPOINT(kPatch, "plugins-instances/{id}", updatePluginsInstance));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "plugins-instances/{id}", deletePluginsInstance));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "plugins-instances/{id}/devices", getInstanceDevices));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "plugins-instances/{id}/log", getPluginsInstancesLog));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "plugins-instances/{id}/binding/{query}", getPluginsInstancesBinding));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "plugins-instances/{id}/start", startPluginsInstance));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "plugins-instances/{id}/stop", stopPluginsInstance));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "plugins-instances/{id}/extra-query/{query}", sendExtraQueryToPluginInstance));

            //TODO RAF
            //TODO : déplacer dans le service Device : REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("createDevice"), CPlugin::createDevice, CPlugin::transactionalMethod)
            //TODO : déplacer dans le service Device : REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("deviceExtraQuery")("*")("*"), CPlugin::sendDeviceExtraQuery, CPlugin::transactionalMethod)

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CPlugin::getAvailablePlugins(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // Filter by type
               const auto types = request->queryParamAsList("by-type");
               std::vector<boost::shared_ptr<const shared::plugin::information::IInformation>> foundPlugins;
               for (const auto& plugin : m_pluginManager->getPluginList())
               {
                  if (!plugin.second->isSupportedOnThisPlatform())
                     continue;

                  if (!m_developerMode && boost::starts_with(plugin.first, "dev-"))
                     continue;

                  if (types->empty() || types->find(plugin.second->getType()) != types->end())
                     foundPlugins.push_back(plugin.second);
               }

               // Get requested props
               const auto props = request->queryParamAsList("prop");
               std::vector<boost::shared_ptr<shared::CDataContainer>> pluginEntries;
               for (const auto& plugin : foundPlugins)
               {
                  auto pluginEntry = boost::make_shared<shared::CDataContainer>();
                  if (props->empty() || props->find("type") != props->end())
                     pluginEntry->set("type", plugin->getType());
                  if (props->empty() || props->find("version") != props->end())
                     pluginEntry->set("version", plugin->getVersion().toString());
                  if (props->empty() || props->find("author") != props->end())
                     pluginEntry->set("author", plugin->getAuthor());
                  if (props->empty() || props->find("url") != props->end())
                     pluginEntry->set("url", plugin->getUrl());
                  if (props->empty() || props->find("support-manually-created-device") != props->end())
                     pluginEntry->set("support-manually-created-device", plugin->getSupportManuallyCreatedDevice());
                  if (props->empty() || props->find("support-device-removed-notification") != props->end())
                     pluginEntry->set("support-device-removed-notification", plugin->getSupportDeviceRemovedNotification());
                  if (props->empty() || props->find("package") != props->end())
                     pluginEntry->set("package", *plugin->getPackage());

                  pluginEntries.push_back(pluginEntry);
               }

               if (pluginEntries.empty())
                  return boost::make_shared<CNoContentAnswer>();

               shared::CDataContainer container;
               container.set("plugins", pluginEntries);
               return boost::make_shared<CSuccessAnswer>(container);
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available plugins");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::getPluginsInstances(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               std::vector<boost::shared_ptr<database::entities::CPlugin>> instances;
               if (id.empty())
                  instances = m_pluginManager->getInstanceList();
               else
                  instances.push_back(m_pluginManager->getInstance(static_cast<int>(std::stol(id))));

               // Filtering
               if (request->queryParamExists("for-manual-device-creation"))
               {
                  auto pluginList = m_pluginManager->getPluginList();
                  instances.erase(std::remove_if(instances.begin(),
                                                 instances.end(),
                                                 [this, &pluginList](const auto& instance)
                                                 {
                                                    if (!m_pluginManager->isInstanceRunning(instance->Id()))
                                                       return true;

                                                    const auto matchingInfo = pluginList.find(instance->Type());
                                                    if (matchingInfo == pluginList.end() || !matchingInfo->second->getSupportManuallyCreatedDevice())
                                                       return true;

                                                    return false;
                                                 }), instances.end());
               }

               // Get requested props
               const auto props = request->queryParamAsList("prop");
               std::vector<boost::shared_ptr<shared::CDataContainer>> instancesEntries;
               for (const auto& instance : instances)
               {
                  auto instanceEntry = boost::make_shared<shared::CDataContainer>();
                  if (props->empty() || props->find("id") != props->end())
                     instanceEntry->set("id", instance->Id());
                  if (props->empty() || props->find("display-name") != props->end())
                     instanceEntry->set("display-name", instance->DisplayName());
                  if (props->empty() || props->find("type") != props->end())
                     instanceEntry->set("type", instance->Type());
                  if (props->empty() || props->find("configuration") != props->end())
                     instanceEntry->set("configuration", instance->Configuration());
                  if (props->empty() || props->find("auto-start") != props->end())
                     instanceEntry->set("auto-start", instance->AutoStart());
                  if (props->empty() || props->find("category") != props->end())
                     instanceEntry->set("category", instance->Category());
                  if (props->empty() || props->find("state") != props->end())
                     instanceEntry->set("state", m_pluginManager->getInstanceState(instance->Id()));
                  if (props->empty() || props->find("full-state") != props->end())
                     instanceEntry->set("full-state", m_pluginManager->getInstanceFullState(instance->Id()));

                  instancesEntries.push_back(instanceEntry);
               }

               if (instancesEntries.empty())
                  return boost::make_shared<CNoContentAnswer>();

               shared::CDataContainer container;
               container.set("instances", instancesEntries);
               return boost::make_shared<CSuccessAnswer>(container);
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available instances");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::createPluginsInstance(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  std::move(request),
                  m_dataProvider,
                  [this](const auto& req) -> boost::shared_ptr<IAnswer>
                  {
                     database::entities::CPlugin plugin;
                     plugin.fillFromSerializedString(req->body());
                     const auto idCreated = m_pluginManager->createInstance(plugin);
                     return boost::make_shared<CCreatedAnswer>("plugins-instances/" + std::to_string(idCreated));
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to create plugin instance");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::updatePluginsInstance(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "plugin-instance id was not provided");
               const auto instanceId = static_cast<int>(std::stol(id));

               database::entities::CPlugin instanceToUpdate;
               instanceToUpdate.fillFromSerializedString(request->body());
               instanceToUpdate.Id = instanceId;

               m_pluginManager->updateInstance(instanceToUpdate);

               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to create plugin instance");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::deletePluginsInstance(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  std::move(request),
                  m_dataProvider,
                  [this](const auto& req) -> boost::shared_ptr<IAnswer>
                  {
                     // ID
                     const auto id = req->pathVariable("id", std::string());
                     if (id.empty())
                        return boost::make_shared<CErrorAnswer>(
                           shared::http::ECodes::kBadRequest,
                           "plugin-instance id was not provided");
                     const auto instanceId = static_cast<int>(std::stol(id));

                     m_pluginManager->deleteInstance(instanceId);

                     return boost::make_shared<CNoContentAnswer>();
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to create plugin instance");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::getInstanceDevices(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "plugin-instance id was not provided");
               const auto instanceId = static_cast<int>(std::stol(id));

               // Filtering
               const auto withBlacklisted = request->queryParamExists("with-blacklisted");

               const auto devices = m_dataProvider->getDeviceRequester()->getDevices(instanceId,
                                                                                     withBlacklisted);

               // Get requested props
               const auto props = request->queryParamAsList("prop");
               std::vector<boost::shared_ptr<shared::CDataContainer>> deviceEntries;
               for (const auto& device : devices)
               {
                  auto instanceEntry = boost::make_shared<shared::CDataContainer>();
                  if (props->empty() || props->find("id") != props->end())
                     instanceEntry->set("id", device->Id());
                  if (props->empty() || props->find("plugin-id") != props->end())
                     instanceEntry->set("plugin-id", device->PluginId());
                  if (props->empty() || props->find("name") != props->end())
                     instanceEntry->set("name", device->Name());
                  if (props->empty() || props->find("friendly-name") != props->end())
                     instanceEntry->set("friendly-name", device->FriendlyName());
                  if (props->empty() || props->find("model") != props->end())
                     instanceEntry->set("model", device->Model());
                  if (props->empty() || props->find("details") != props->end())
                     instanceEntry->set("details", device->Details());
                  if (props->empty() || props->find("configuration") != props->end())
                     instanceEntry->set("configuration", device->Configuration());
                  if (props->empty() || props->find("type") != props->end())
                     instanceEntry->set("type", device->Type());
                  if (props->empty() || props->find("blacklisted") != props->end())
                     instanceEntry->set("blacklisted", device->Blacklist());

                  deviceEntries.push_back(instanceEntry);
               }

               if (deviceEntries.empty())
                  return boost::make_shared<CNoContentAnswer>();

               shared::CDataContainer container;
               container.set("devices", deviceEntries);
               return boost::make_shared<CSuccessAnswer>(container);
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available instances");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::getPluginsInstancesLog(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "plugin-instance id was not provided");
               const auto instanceId = static_cast<int>(std::stol(id));

               return boost::make_shared<CSuccessAnswer>(m_pluginManager->getInstanceLog(instanceId));
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available instances");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::getPluginsInstancesBinding(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "plugin-instance id was not provided");
               const auto instanceId = static_cast<int>(std::stol(id));

               // Query
               const auto query = request->pathVariable("query", std::string());
               if (query.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "binding query was not provided");

               //create a callback (allow waiting for result)              
               communication::callback::CSynchronousCallback<boost::shared_ptr<shared::CDataContainer>> cb;

               //create the data container to send to plugin
               const pluginSystem::CBindingQueryData data(query);

               //send request to plugin
               m_messageSender.sendBindingQueryRequest(instanceId, data, cb);

               //wait for result
               switch (cb.waitForResult())
               {
               case communication::callback::CSynchronousCallback<boost::shared_ptr<shared::CDataContainer>>::kResult:
                  {
                     const auto res = cb.getCallbackResult();

                     if (res.success)
                        return boost::make_shared<CSuccessAnswer>(*res.result());

                     return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                             res.errorMessage);
                  }

               case shared::event::kTimeout:
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                          "The plugin did not respond");

               default:
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                          "Unknown plugin result");
               }
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available instances");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::startPluginsInstance(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "plugin-instance id was not provided");
               const auto instanceId = static_cast<int>(std::stol(id));

               m_pluginManager->startInstance(instanceId);

               shared::CDataContainer container;
               container.set("started", m_pluginManager->isInstanceRunning(instanceId));
               return boost::make_shared<CSuccessAnswer>(container);
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available instances");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::sendExtraQueryToPluginInstance(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "plugin-instance id was not provided");
               const auto instanceId = static_cast<int>(std::stol(id));

               // Query
               const auto query = request->pathVariable("query", std::string());
               if (query.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "query to plugin-instance was not provided");

               return CHelpers::transactionalMethodV2(
                  std::move(request),
                  m_dataProvider,
                  [this, &instanceId, &query](const auto& req) -> boost::shared_ptr<IAnswer>
                  {
                     const auto data = boost::make_shared<pluginSystem::CExtraQueryData>(query,
                                                                                         req->body().empty()
                                                                                            ? shared::CDataContainer::make()
                                                                                            : shared::CDataContainer::make(req->body()),
                                                                                         std::string());
                     const auto taskId = m_messageSender.sendExtraQueryAsync(instanceId, data);

                     if (taskId.empty())
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                                "Fail to get extra query task");

                     shared::CDataContainer result;
                     result.set("taskId", taskId);
                     return boost::make_shared<CSuccessAnswer>(result);
                  });
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available instances");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::stopPluginsInstance(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "plugin-instance id was not provided");
               const auto instanceId = static_cast<int>(std::stol(id));

               m_pluginManager->stopInstance(instanceId);
               return boost::make_shared<CNoContentAnswer>();
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available instances");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
