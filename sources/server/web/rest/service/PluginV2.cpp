#include "stdafx.h"
#include "Plugin.h"
#include "RestEndPoint.h"
#include "web/rest/ErrorAnswer.h"
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

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "plugins-instances/devices", getInstanceDevices));

            //TODO RAF
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("devices"), CPlugin::getPluginDevices)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("log"), CPlugin::getInstanceLog)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("*")("binding")("*"), CPlugin::getBinding)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("start"), CPlugin::startInstance)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("stop"), CPlugin::stopInstance)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("instanceRunning"), CPlugin::getInstanceRunning)

            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CPlugin::createPlugin, CPlugin::transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("createDevice"), CPlugin::createDevice, CPlugin::transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CPlugin::updatePlugin, CPlugin::transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("extraQuery")("*"), CPlugin::sendExtraQuery, CPlugin::transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("deviceExtraQuery")("*")("*"), CPlugin::sendDeviceExtraQuery, CPlugin::transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CPlugin::deleteAllPlugins, CPlugin::transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CPlugin::deletePlugin, CPlugin::transactionalMethod)

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CPlugin::getAvailablePlugins(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // Filter by type
               const auto typeValue = request->parameter("type", std::string());
               std::vector<boost::shared_ptr<const shared::plugin::information::IInformation>> foundPlugins;
               for (const auto& plugin : m_pluginManager->getPluginList())
               {
                  if (!plugin.second->isSupportedOnThisPlatform())
                     continue;

                  if (!m_developerMode && boost::starts_with(plugin.first, "dev-"))
                     continue;

                  if (typeValue.empty() || typeValue.find(plugin.second->getType()) != std::string::npos)
                     foundPlugins.push_back(plugin.second);
               }

               // Get requested fields. Supported fields are :
               // - version
               // - author
               // - url
               // - SupportManuallyCreatedDevice
               // - SupportDeviceRemovedNotification
               // - package
               const auto fields = request->parameterAsFlagList("fields");
               std::vector<boost::shared_ptr<shared::CDataContainer>> pluginEntries;
               for (const auto& plugin : foundPlugins)
               {
                  auto pluginEntry = boost::make_shared<shared::CDataContainer>();
                  if (fields->empty() || fields->find("type") != fields->end())
                     pluginEntry->set("type", plugin->getType());
                  if (fields->empty() || fields->find("version") != fields->end())
                     pluginEntry->set("version", plugin->getVersion().toString());
                  if (fields->empty() || fields->find("author") != fields->end())
                     pluginEntry->set("author", plugin->getAuthor());
                  if (fields->empty() || fields->find("url") != fields->end())
                     pluginEntry->set("url", plugin->getUrl());
                  if (fields->empty() || fields->find("support-manually-created-device") != fields->end())
                     pluginEntry->set("SupportManuallyCreatedDevice", plugin->getSupportManuallyCreatedDevice());
                  if (fields->empty() || fields->find("support-device-removed-notification") != fields->end())
                     pluginEntry->set("SupportDeviceRemovedNotification", plugin->getSupportDeviceRemovedNotification());
                  if (fields->empty() || fields->find("package") != fields->end())
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
               // Filtering
               const auto id = request->parameter("id", std::string());
               std::vector<boost::shared_ptr<database::entities::CPlugin>> instances;
               if (id.empty())
                  instances = m_pluginManager->getInstanceList();
               else
                  instances.push_back(m_pluginManager->getInstance(static_cast<int>(std::stol(id))));

               const auto filters = request->parameterAsFlagList("filter");
               if (filters->find("for-manual-device-creation") != filters->end())
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

               // Get requested fields. Supported fields are :
               // - id
               // - display-name
               // - type
               // - configuration
               // - auto-start
               // - category
               // - state
               // - full-state (state + messageId if any)
               const auto fields = request->parameterAsFlagList("fields");
               std::vector<boost::shared_ptr<shared::CDataContainer>> instancesEntries;
               for (const auto& instance : instances)
               {
                  auto instanceEntry = boost::make_shared<shared::CDataContainer>();
                  if (fields->empty() || fields->find("id") != fields->end())
                     instanceEntry->set("id", instance->Id());
                  if (fields->empty() || fields->find("display-name") != fields->end())
                     instanceEntry->set("displayName", instance->DisplayName());
                  if (fields->empty() || fields->find("type") != fields->end())
                     instanceEntry->set("type", instance->Type());
                  if (fields->empty() || fields->find("configuration") != fields->end())
                     instanceEntry->set("configuration", instance->Configuration());
                  if (fields->empty() || fields->find("auto-start") != fields->end())
                     instanceEntry->set("autoStart", instance->AutoStart());
                  if (fields->empty() || fields->find("category") != fields->end())
                     instanceEntry->set("category", instance->Category());
                  if (fields->empty() || fields->find("state") != fields->end())
                     instanceEntry->set("state", m_pluginManager->getInstanceState(instance->Id()));
                  if (fields->empty() || fields->find("full-state") != fields->end())
                     instanceEntry->set("fullState", m_pluginManager->getInstanceFullState(instance->Id()));

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

         boost::shared_ptr<IAnswer> CPlugin::getInstanceDevices(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // Filtering
               const auto id = request->parameter("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kUnprocessableentity,
                                                          "plugin-instance id was not provided");
               const auto instanceId = static_cast<int>(std::stol(id));

               const auto filters = request->parameterAsFlagList("filter");
               const auto withBlacklisted = filters->find("with-blacklisted") != filters->end();

               const auto devices = m_dataProvider->getDeviceRequester()->getDevices(instanceId,
                                                                                     withBlacklisted);

               // Get requested fields. Supported fields are :
               // - id
               // - plugin-id
               // - name
               // - friendly-name
               // - model
               // - details
               // - configuration
               // - type
               // - blacklisted
               const auto fields = request->parameterAsFlagList("fields");
               std::vector<boost::shared_ptr<shared::CDataContainer>> deviceEntries;
               for (const auto& device : devices)
               {
                  auto instanceEntry = boost::make_shared<shared::CDataContainer>();
                  if (fields->empty() || fields->find("id") != fields->end())
                     instanceEntry->set("id", device->Id());
                  if (fields->empty() || fields->find("plugin-id") != fields->end())
                     instanceEntry->set("plugin-id", device->PluginId());
                  if (fields->empty() || fields->find("name") != fields->end())
                     instanceEntry->set("name", device->Name());
                  if (fields->empty() || fields->find("friendly-name") != fields->end())
                     instanceEntry->set("friendly-name", device->FriendlyName());
                  if (fields->empty() || fields->find("model") != fields->end())
                     instanceEntry->set("model", device->Model());
                  if (fields->empty() || fields->find("details") != fields->end())
                     instanceEntry->set("details", device->Details());
                  if (fields->empty() || fields->find("configuration") != fields->end())
                     instanceEntry->set("configuration", device->Configuration());
                  if (fields->empty() || fields->find("type") != fields->end())
                     instanceEntry->set("type", device->Type());
                  if (fields->empty() || fields->find("blacklisted") != fields->end())
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
      } //namespace service
   } //namespace rest
} //namespace web 
