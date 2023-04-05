#include <utility>

#include "stdafx.h"
#include "Plugin.h"
#include "RestEndPoint.h"
#include "communication/callback/SynchronousCallback.h"
#include "pluginSystem/BindingQueryData.h"
#include "pluginSystem/ExtraQueryData.h"
#include "shared/exception/EmptyResult.hpp"
#include "tools/OperatingSystem.h"
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
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "plugins", getAvailablePluginsV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "plugins-instances", getPluginsInstancesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "plugins-instances/{id}", getPluginsInstancesV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "plugins-instances", createPluginsInstanceV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPatch, "plugins-instances/{id}", updatePluginsInstanceV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "plugins-instances/{id}", deletePluginsInstanceV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "plugins-instances/{id}/logs", getPluginsInstancesLogV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "plugins-instances/{id}/binding/{query}", getPluginsInstancesBindingV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "plugins-instances/{id}/start", startPluginsInstanceV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "plugins-instances/{id}/stop", stopPluginsInstanceV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "plugins-instances/{id}/extra-query/{query}", sendExtraQueryToPluginInstanceV2));

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CPlugin::getAvailablePluginsV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // Filter by type
               const auto types = request->queryParamAsList("byType");
               std::vector<boost::shared_ptr<const shared::plugin::information::IInformation>> foundPlugins;
               for (const auto& plugin : m_pluginManager->getPluginList())
                  if (types->empty() || types->find(plugin.second->getType()) != types->end())
                     foundPlugins.push_back(plugin.second);

               if (foundPlugins.empty())
                  return boost::make_shared<CNoContentAnswer>();

               // Pagination
               const auto page = request->queryParamExists("page")
                                    ? boost::make_optional(static_cast<int>(std::stol(request->queryParam("page"))))
                                    : boost::optional<int>();
               const auto pageSize = request->queryParamExists("perPage")
                                        ? boost::make_optional(static_cast<int>(std::stol(request->queryParam("perPage"))))
                                        : boost::optional<int>();
               const auto firstItem = (page && pageSize) ? *page * *pageSize : 0u;
               auto lastItem = 0;

               // Get requested props
               const auto props = request->queryParamAsList("prop");

               // Specific case for icon : icon can only be asked alone, and for only one plugin type
               if (!props->empty() && props->find("icon") != props->end())
               {
                  if (props->size() != 1 || types->size() != 1 || foundPlugins.size() != 1)
                     return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                             "icon can only be asked alone, and for only one plugin type");

                  return boost::make_shared<CSuccessAnswer>(foundPlugins.at(0)->getPath() / "icon.png",
                                                            EContentType::kImagePng);
               }

               std::vector<boost::shared_ptr<shared::CDataContainer>> pluginEntries;
               for (const auto& plugin : foundPlugins)
               {
                  auto pluginEntry = boost::make_shared<shared::CDataContainer>();

                  const auto locales = getLocales(request->acceptLanguage(),
                                                  plugin);
                  if (!locales->empty() && (props->empty() || props->find("name") != props->end()))
                     pluginEntry->set("name", locales->get<std::string>("name"));
                  if (!locales->empty() && (props->empty() || props->find("description") != props->end()))
                     pluginEntry->set("description", locales->get<std::string>("description"));

                  if (props->empty() || props->find("type") != props->end())
                     pluginEntry->set("type", plugin->getType());
                  if (props->empty() || props->find("version") != props->end())
                     pluginEntry->set("version", plugin->getVersion().toString());
                  if (props->empty() || props->find("author") != props->end())
                     pluginEntry->set("author", plugin->getAuthor());
                  if (props->empty() || props->find("url") != props->end())
                     pluginEntry->set("url", plugin->getUrl());
                  if (props->empty() || props->find("supportManuallyCreatedDevice") != props->end())
                     pluginEntry->set("supportManuallyCreatedDevice", plugin->getSupportManuallyCreatedDevice());
                  if (props->empty() || props->find("supportDeviceRemovedNotification") != props->end())
                     pluginEntry->set("supportDeviceRemovedNotification", plugin->getSupportDeviceRemovedNotification());
                  if (props->empty() || props->find("configurationSchema") != props->end())
                     pluginEntry->set("configurationSchema", getPluginConfigurationSchema(plugin,
                                                                                          locales));

                  if (!pluginEntry->empty())
                  {
                     if (page && pageSize)
                     {
                        if (lastItem >= firstItem && lastItem < (firstItem + *pageSize))
                           pluginEntries.push_back(pluginEntry);
                        ++lastItem;
                     }
                     else
                     {
                        pluginEntries.push_back(pluginEntry);
                     }
                  }
               }

               if (pluginEntries.empty())
                  return boost::make_shared<CNoContentAnswer>();

               boost::optional<CPaging> paging;
               if (page && pageSize)
                  paging = CPaging(*page, lastItem / *pageSize + 1, *pageSize);


               return CHelpers::formatGetMultiItemsAnswer(types->size() == 1,
                                                          pluginEntries,
                                                          "plugins",
                                                          paging);
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available plugins");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::getPluginsInstancesV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               std::vector<boost::shared_ptr<database::entities::CPlugin>> instances;
               if (id.empty())
                  instances = m_pluginManager->getInstanceList(false);
               else
                  instances.push_back(m_pluginManager->getInstance(static_cast<int>(std::stol(id))));

               // Filtering
               if (request->queryParamExists("forManualDeviceCreation"))
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

               // Pagination
               const auto page = request->queryParamExists("page")
                                    ? boost::make_optional(static_cast<int>(std::stol(request->queryParam("page"))))
                                    : boost::optional<int>();
               const auto pageSize = request->queryParamExists("perPage")
                                        ? boost::make_optional(static_cast<int>(std::stol(request->queryParam("perPage"))))
                                        : boost::optional<int>();
               const auto firstItem = (page && pageSize) ? *page * *pageSize : 0u;
               auto lastItem = 0;

               // Get requested props
               const auto props = request->queryParamAsList("prop");
               std::vector<boost::shared_ptr<shared::CDataContainer>> instancesEntries;
               for (const auto& instance : instances)
               {
                  auto instanceEntry = boost::make_shared<shared::CDataContainer>();
                  if (props->empty() || props->find("id") != props->end())
                     instanceEntry->set("id", instance->Id());
                  if (props->empty() || props->find("displayName") != props->end())
                     instanceEntry->set("displayName", instance->DisplayName());
                  if (props->empty() || props->find("type") != props->end())
                     instanceEntry->set("type", instance->Type());
                  if (props->empty() || props->find("configuration") != props->end())
                     instanceEntry->set("configuration", instance->Configuration());
                  if (props->empty() || props->find("autoStart") != props->end())
                     instanceEntry->set("autoStart", instance->AutoStart());
                  if (props->empty() || props->find("category") != props->end())
                     instanceEntry->set("category", instance->Category());
                  if (props->empty() || props->find("state") != props->end())
                     instanceEntry->set("state", m_pluginManager->getInstanceState(instance->Id()));
                  if (props->empty() || props->find("fullState") != props->end())
                     instanceEntry->set("fullState", m_pluginManager->getInstanceFullState(instance->Id()));

                  if (page && pageSize)
                  {
                     if (lastItem >= firstItem && lastItem < (firstItem + *pageSize))
                        instancesEntries.push_back(instanceEntry);
                     ++lastItem;
                  }
                  else
                  {
                     instancesEntries.push_back(instanceEntry);
                  }
               }

               if (instancesEntries.empty())
                  return boost::make_shared<CNoContentAnswer>();

               boost::optional<CPaging> paging;
               if (page && pageSize)
                  paging = CPaging(*page, lastItem / *pageSize + 1, *pageSize);


               return CHelpers::formatGetMultiItemsAnswer(!id.empty(),
                                                          instancesEntries,
                                                          "instances",
                                                          paging);
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available instances");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::createPluginsInstanceV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  request,
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

         boost::shared_ptr<IAnswer> CPlugin::updatePluginsInstanceV2(const boost::shared_ptr<IRequest>& request) const
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
            catch (const shared::exception::CEmptyResult&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kNotFound,
                                                       "Fail to update plugin instance : instance not found");
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to update plugin instance");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::deletePluginsInstanceV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this](const auto& req) -> boost::shared_ptr<IAnswer>
                  {
                     // ID
                     const auto id = req->pathVariable("id", std::string());
                     if (id.empty())
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                                "plugin-instance id was not provided");
                     const auto instanceId = static_cast<int>(std::stol(id));

                     m_pluginManager->deleteInstance(instanceId);

                     return boost::make_shared<CNoContentAnswer>();
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to delete plugin instance");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::getPluginsInstancesLogV2(const boost::shared_ptr<IRequest>& request) const
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

         boost::shared_ptr<IAnswer> CPlugin::getPluginsInstancesBindingV2(const boost::shared_ptr<IRequest>& request) const
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

         boost::shared_ptr<IAnswer> CPlugin::startPluginsInstanceV2(const boost::shared_ptr<IRequest>& request) const
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

         boost::shared_ptr<IAnswer> CPlugin::sendExtraQueryToPluginInstanceV2(const boost::shared_ptr<IRequest>& request) const
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

               const auto taskId = m_messageSender.sendExtraQueryAsync(instanceId,
                                                                       boost::make_shared<pluginSystem::CExtraQueryData>(query,
                                                                          request->body().empty()
                                                                             ? shared::CDataContainer::make()
                                                                             : shared::CDataContainer::make(request->body())));

               if (taskId.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                          "Fail to get extra query task");

               return CHelpers::createLongRunningOperationAnswer(taskId);
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available instances");
            }
         }

         boost::shared_ptr<IAnswer> CPlugin::stopPluginsInstanceV2(const boost::shared_ptr<IRequest>& request) const
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

         boost::shared_ptr<shared::CDataContainer> CPlugin::getPluginConfigurationSchema(
            const boost::shared_ptr<const shared::plugin::information::IInformation>& pluginInformation,
            boost::shared_ptr<shared::CDataContainer> locales) const
         {
            auto schema = pluginInformation->getConfigurationSchema()->copy();

            if (!locales->empty())
               schema->mergeFrom(locales->getChild("configurationSchema"));

            // Manage binding
            schema->replaceAllNodesByName(
               "__Binding__",
               [this](boost::shared_ptr<const shared::CDataContainer> bindingNode)-> boost::shared_ptr<shared::CDataContainer>
               {
                  if (bindingNode->get<std::string>("type") != "system")
                     return nullptr;

                  const auto query = bindingNode->get<std::string>("query");

                  if (query == "serialPorts")
                     return CHelpers::getSerialPortsV2();
                  if (query == "usbDevices")
                  {
                     std::vector<std::pair<int, int>> requestedUsbDevices;
                     for (const auto& requestedDevice : bindingNode->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>("content.oneOf"))
                        requestedUsbDevices.emplace_back(std::make_pair(requestedDevice->get<int>("vendorId"),
                                                                        requestedDevice->get<int>("productId")));

                     const auto result = CHelpers::getUsbDevicesV2(requestedUsbDevices,
                                                                   m_usbDevicesLister);

                     return CHelpers::getUsbDevicesV2(requestedUsbDevices,
                                                      m_usbDevicesLister);
                  }
                  if (query == "NetworkInterfaces")
                     return CHelpers::getNetworkInterfacesV2(false);
                  if (query == "NetworkInterfacesWithoutLoopback")
                     return CHelpers::getNetworkInterfacesV2(true);
                  if (query == "platformIsWindows")
                  {
                     auto result = shared::CDataContainer::make();
                     result->set(bindingNode->get<std::string>("key"),
                                 tools::COperatingSystem::getName() == "windows");
                     return result;
                  }
                  if (query == "platformIsLinux")
                  {
                     auto result = shared::CDataContainer::make();
                     result->set(bindingNode->get<std::string>("key"),
                                 tools::COperatingSystem::getName() == "linux");
                     return result;
                  }
                  if (query == "platformIsMac")
                  {
                     auto result = shared::CDataContainer::make();
                     result->set(bindingNode->get<std::string>("key"),
                                 tools::COperatingSystem::getName() == "mac");
                     return result;
                  }
                  if (query == "supportedTimezones")
                  {
                     std::set<std::string> filter;
                     if (bindingNode->exists("filter"))
                     {
                        const auto requestedFilter = bindingNode->get<std::string>("filter");
                        char separator = '|';
                        for (const auto& t : boost::tokenizer<boost::char_separator<char>>(requestedFilter,
                                                                                           boost::char_separator<char>(&separator)))
                           filter.insert(t);
                     }
                     return CHelpers::getSupportedTimezonesV2(filter,
                                                              m_timezoneDatabase);
                  }

                  return nullptr;
               });


            return schema;
         }

         boost::shared_ptr<shared::CDataContainer> CPlugin::getLocales(
            const std::string& requestedLocale,
            const boost::shared_ptr<const shared::plugin::information::IInformation>& plugin) const
         {
            if (requestedLocale.empty())
               return shared::CDataContainer::make();

            try
            {
               return shared::CDataContainer::make(plugin->getPath() / std::string("locales/" + requestedLocale + ".json"));
            }
            catch (const std::exception&)
            {
               try
               {
                  return shared::CDataContainer::make(plugin->getPath() / std::string("locales/en.json"));
               }
               catch (const std::exception&)
               {
                  throw std::invalid_argument(
                     "Unable to find valid locale file for plugin " + plugin->getType() +
                     ". Tried requested '" + requestedLocale + "', and 'en' as default");
               }
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
