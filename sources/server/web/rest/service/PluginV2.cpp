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
         const std::string CPlugin::RestKeywordV2 = std::string("plugins");

         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CPlugin::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, RestKeywordV2, getAvailablePlugins));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, RestKeywordV2 + "/instances", getPluginsInstances));

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
               const auto fieldsValue = request->parameter("fields", std::string());
               std::vector<boost::shared_ptr<shared::CDataContainer>> pluginEntries;
               for (const auto& plugin : foundPlugins)
               {
                  auto pluginEntry = boost::make_shared<shared::CDataContainer>();
                  if (fieldsValue.empty() || fieldsValue.find("type") != std::string::npos)
                     pluginEntry->set("type", plugin->getType());
                  if (fieldsValue.empty() || fieldsValue.find("version") != std::string::npos)
                     pluginEntry->set("version", plugin->getVersion().toString());
                  if (fieldsValue.empty() || fieldsValue.find("author") != std::string::npos)
                     pluginEntry->set("author", plugin->getAuthor());
                  if (fieldsValue.empty() || fieldsValue.find("url") != std::string::npos)
                     pluginEntry->set("url", plugin->getUrl());
                  if (fieldsValue.empty() || fieldsValue.find("support-manually-created-device") != std::string::npos)
                     pluginEntry->set("SupportManuallyCreatedDevice", plugin->getSupportManuallyCreatedDevice());
                  if (fieldsValue.empty() || fieldsValue.find("support-device-removed-notification") != std::string::npos)
                     pluginEntry->set("SupportDeviceRemovedNotification", plugin->getSupportDeviceRemovedNotification());
                  if (fieldsValue.empty() || fieldsValue.find("package") != std::string::npos)
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

               const auto filter = request->parameter("filter", std::string());
               if (filter.find("for-manual-device-creation") != std::string::npos)
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
               const auto fieldsValue = request->parameter("fields", std::string());
               std::vector<boost::shared_ptr<shared::CDataContainer>> instancesEntries;
               for (const auto& instance : instances)
               {
                  auto instanceEntry = boost::make_shared<shared::CDataContainer>();
                  if (fieldsValue.empty() || fieldsValue.find("id") != std::string::npos)
                     instanceEntry->set("id", instance->Id());
                  if (fieldsValue.empty() || fieldsValue.find("display-name") != std::string::npos)
                     instanceEntry->set("displayName", instance->DisplayName());
                  if (fieldsValue.empty() || fieldsValue.find("type") != std::string::npos)
                     instanceEntry->set("type", instance->Type());
                  if (fieldsValue.empty() || fieldsValue.find("configuration") != std::string::npos)
                     instanceEntry->set("configuration", instance->Configuration());
                  if (fieldsValue.empty() || fieldsValue.find("auto-start") != std::string::npos)
                     instanceEntry->set("autoStart", instance->AutoStart());
                  if (fieldsValue.empty() || fieldsValue.find("category") != std::string::npos)
                     instanceEntry->set("category", instance->Category());
                  if (fieldsValue.empty() || fieldsValue.find("state") != std::string::npos)
                     instanceEntry->set("state", m_pluginManager->getInstanceState(instance->Id()));
                  if (fieldsValue.empty() || fieldsValue.find("full-state") != std::string::npos)
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
      } //namespace service
   } //namespace rest
} //namespace web 
