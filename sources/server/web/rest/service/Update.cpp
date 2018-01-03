#include "stdafx.h"
#include "Update.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"
#include "update/info/UpdateSite.h"
#include <shared/Log.h>


namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CUpdate::m_restKeyword = std::string("update");

         DECLARE_ENUM_IMPLEMENTATION_NESTED(CUpdate::EWhatToDo, EWhatToDo,
            ((Update))
            ((Check))
         )

         CUpdate::CUpdate(boost::shared_ptr<update::CUpdateManager> updateManager,
                          boost::shared_ptr<pluginSystem::CManager> pluginManager)
            : m_updateManager(updateManager),
              m_pluginManager(pluginManager)
         {
         }


         CUpdate::~CUpdate()
         {
         }

         const std::string& CUpdate::getRestKeyword()
         {
            return m_restKeyword;
         }

         void CUpdate::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("yadoms")("list")("*"), CUpdate::
               availableYadomsVersions);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("yadoms")("update"), CUpdate::updateYadoms);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("widget")("list")("*"), CUpdate::
               availableWidgets);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("update")("*"), CUpdate::
               updateWidget);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("install"), CUpdate::installWidget
            );
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("remove")("*"), CUpdate::
               removeWidget);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("plugin")("list")("*")("*"), CUpdate::
               availablePlugins);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("update")("*"), CUpdate::
               updatePlugin);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("install"), CUpdate::installPlugin
            );
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("remove")("*"), CUpdate::
               removePlugin);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET" , (m_restKeyword)("scriptInterpreter")("list")("*"), CUpdate::
               availableScriptInterpreters);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("update")("*"), CUpdate
               ::updateScriptInterpreter);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("install"), CUpdate::
               installScriptInterpreter);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("remove")("*"), CUpdate
               ::removeScriptInterpreter);
         }

         shared::CDataContainer CUpdate::availableYadomsVersions(const std::vector<std::string>& parameters,
                                                                 const std::string& requestContent) const
         {
            std::string lang = "";

            if (parameters.size() > 3)
               lang = parameters[3];

            //ask site info
            return CResult::GenerateSuccess(update::info::CUpdateSite::getAllYadomsVersions(lang));
         }

         shared::CDataContainer CUpdate::updateYadoms(const std::vector<std::string>& parameters,
                                                      const std::string& requestContent) const
         {
            shared::CDataContainer content(requestContent);
            if (!content.containsChild("versionData"))
            {
               content.printToLog(YADOMS_LOG(information));
               return CResult::GenerateError("The request should contains the versionData.");
            }

            const auto versionData = content.get<shared::CDataContainer>("versionData");
            const auto taskId = m_updateManager->updateYadomsAsync(versionData);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }

         shared::CDataContainer CUpdate::availablePlugins(const std::vector<std::string>& parameters,
                                                          const std::string& requestContent) const
         {
            if (parameters.size() != 5)
               return CResult::GenerateError("Invalid parameters in url /rest/plugin/list");

            const auto includePreleases = parameters[3] == "includePreReleases";
            const auto lang = parameters[4];

            const auto localVersions = m_pluginManager->getPluginList();

            const auto availableVersions = update::info::CUpdateSite::getAllPluginVersions(lang);

            //ask site info
            return CResult::GenerateSuccess(buildPluginList(localVersions, availableVersions, includePreleases));
         }


         shared::CDataContainer CUpdate::updatePlugin(const std::vector<std::string>& parameters,
                                                      const std::string& requestContent) const
         {
            //the request url should contain the pluginName
            //the request content should contain the downloadURL
            if (parameters.size() <= 3)
               return CResult::GenerateError("Not enougth parameters in url /rest/plugin/update/**pluginName**");

            const auto pluginName = parameters[3];

            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return CResult::GenerateError("The request should contains the downloadURL.");

            const auto downloadUrl = content.get<std::string>("downloadUrl");
            const auto taskId = m_updateManager->updatePluginAsync(pluginName, downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }


         shared::CDataContainer CUpdate::installPlugin(const std::vector<std::string>& parameters,
                                                       const std::string& requestContent) const
         {
            //the request content should contain the downloadURL
            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return CResult::GenerateError("The request should contains the downloadURL.");

            auto downloadUrl = content.get<std::string>("downloadUrl");
            auto taskId = m_updateManager->installPluginAsync(downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }

         shared::CDataContainer CUpdate::removePlugin(const std::vector<std::string>& parameters,
                                                      const std::string& requestContent) const
         {
            //the request url should contain the pluginName
            if (parameters.size() <= 3)
               return CResult::GenerateError("Not enougth parameters in url /rest/plugin/remove/**pluginName**");

            auto pluginName = parameters[3];
            auto taskId = m_updateManager->removePluginAsync(pluginName);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }


         shared::CDataContainer CUpdate::availableWidgets(const std::vector<std::string>& parameters,
                                                          const std::string& requestContent) const
         {
            std::string lang = "";

            if (parameters.size() > 3)
               lang = parameters[3];

            //ask site info
            return CResult::GenerateSuccess(update::info::CUpdateSite::getAllWidgetsVersions(lang));
         }


         shared::CDataContainer CUpdate::updateWidget(const std::vector<std::string>& parameters,
                                                      const std::string& requestContent) const
         {
            //the request url should contain the widgetName
            //the request content should contain the downloadURL
            if (parameters.size() <= 3)
               return CResult::GenerateError("Not enougth parameters in url /rest/widget/update/**widgetName**");

            auto widgetName = parameters[3];

            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return CResult::GenerateError("The request should contains the downloadURL.");

            auto downloadUrl = content.get<std::string>("downloadUrl");
            auto taskId = m_updateManager->updateWidgetAsync(widgetName, downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }


         shared::CDataContainer CUpdate::installWidget(const std::vector<std::string>& parameters,
                                                       const std::string& requestContent) const
         {
            //the request content should contain the downloadURL
            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return CResult::GenerateError("The request should contains the downloadURL.");

            auto downloadUrl = content.get<std::string>("downloadUrl");
            auto taskId = m_updateManager->installWidgetAsync(downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }

         shared::CDataContainer CUpdate::removeWidget(const std::vector<std::string>& parameters,
                                                      const std::string& requestContent) const
         {
            //the request url should contain the pluginName
            if (parameters.size() <= 3)
               return CResult::GenerateError("Not enougth parameters in url /rest/widget/remove/**widgetName**");

            auto widgetName = parameters[3];
            auto taskId = m_updateManager->removeWidgetAsync(widgetName);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }

         shared::CDataContainer CUpdate::availableScriptInterpreters(const std::vector<std::string>& parameters,
                                                                     const std::string& requestContent) const
         {
            std::string lang = "";

            if (parameters.size() > 3)
               lang = parameters[3];

            //ask site info
            return CResult::GenerateSuccess(update::info::CUpdateSite::getAllScriptInterpreterVersions(lang));
         }


         shared::CDataContainer CUpdate::updateScriptInterpreter(const std::vector<std::string>& parameters,
                                                                 const std::string& requestContent) const
         {
            //the request url should contain the scriptInterpreterName
            //the request content should contain the downloadURL
            if (parameters.size() <= 3)
               return CResult::GenerateError(
                  "Not enougth parameters in url /rest/scriptInterpreter/update/**scriptInterpreterName**");

            auto scriptInterpreterName = parameters[3];

            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return CResult::GenerateError("The request should contains the downloadURL.");

            auto downloadUrl = content.get<std::string>("downloadUrl");
            auto taskId = m_updateManager->updateScriptInterpreterAsync(scriptInterpreterName, downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }


         shared::CDataContainer CUpdate::installScriptInterpreter(const std::vector<std::string>& parameters,
                                                                  const std::string& requestContent) const
         {
            //the request content should contain the downloadURL
            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return CResult::GenerateError("The request should contains the downloadURL.");

            auto downloadUrl = content.get<std::string>("downloadUrl");
            auto taskId = m_updateManager->installScriptInterpreterAsync(downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }

         shared::CDataContainer CUpdate::removeScriptInterpreter(const std::vector<std::string>& parameters,
                                                                 const std::string& requestContent) const
         {
            //the request url should contain the scriptInterpreterName
            if (parameters.size() <= 3)
               return CResult::GenerateError(
                  "Not enougth parameters in url /rest/scriptInterpreter/remove/**scriptInterpreterName**");

            const auto scriptInterpreterName = parameters[3];
            const auto taskId = m_updateManager->removeScriptInterpreterAsync(scriptInterpreterName);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }

         shared::CDataContainer CUpdate::buildPluginList(
            const pluginSystem::IFactory::AvailablePluginMap& localVersions,
            const shared::CDataContainer& availableVersions,
            bool includePreleases) const
         {
            shared::CDataContainer list;
            for (const auto& localVersion : localVersions)
            {
               try
               {
                  const auto pluginType = localVersion.first;
                  shared::CDataContainer item;

                  //TODO conserver ? Le client doit pouvoir le récupérer comme le name/description
                  item.set("iconUrl", (localVersion.second->getPath() / "icon.png").generic_string());

                  shared::CDataContainer versions;
                  versions.set("installed", localVersion.second->getVersion().toString());
                  std::map<std::string, shared::CDataContainer> older; // Pass by a map to sort versions list
                  std::map<std::string, shared::CDataContainer> newer; // Pass by a map to sort versions list
                  if (availableVersions.exists(pluginType))
                  {
                     try
                     {
                        const auto availableVersionsForPlugin =
                           availableVersions.get<std::vector<shared::CDataContainer>>(pluginType);
                        for (auto& version : availableVersionsForPlugin)
                        {
                           shared::versioning::CVersion v(version.get<std::string>("version"));

                           // Don't add prereleases versions if not asked
                           if (!v.prerelease().empty() && !includePreleases)
                              continue;

                           if (v == localVersion.second->getVersion())
                              continue;

                           shared::CDataContainer versionData;
                           versionData.set("downloadUrl", version.get<std::string>("downloadUrl"));

                           if (v < localVersion.second->getVersion())
                              older[version.get<std::string>("version")] = versionData;
                           else
                              newer[version.get<std::string>("version")] = versionData;
                        }
                     }
                     catch (std::exception& exception)
                     {
                        YADOMS_LOG(warning) << "Invalid remote package for " << pluginType << ", will be ignored";
                        YADOMS_LOG(debug) << "exception : " << exception.what();
                     }
                  }

                  if (!older.empty())
                  {
                     // Sort (newer version first)
                     shared::CDataContainer olderVersions;
                     for (auto v = older.rbegin(); v != older.rend(); ++v)
                        // Force different path char to not cut version string into subPaths
                        olderVersions.set(v->first, v->second, 0);
                     versions.set("older", olderVersions);
                  }

                  if (!newer.empty())
                  {
                     // Sort (newer version first)
                     shared::CDataContainer newerVersions;
                     for (auto v = newer.rbegin(); v != newer.rend(); ++v)
                        // Force different path char to not cut version string into subPaths
                        newerVersions.set(v->first, v->second, 0);
                     versions.set("newer", newerVersions);

                     // Find the newest version
                     shared::CDataContainer newestVersion;
                     // Force different path char to not cut version string into subPaths
                     newestVersion.set(newer.rbegin()->first, newer.rbegin()->second, 0);
                     versions.set("newest", newestVersion);
                  }

                  item.set("versions", versions);

                  list.set(pluginType, item);
               }
               catch (std::exception& exception)
               {
                  YADOMS_LOG(warning) << "Invalid package " << localVersion.first << ", will be ignored";
                  YADOMS_LOG(debug) << "exception : " << exception.what();
               }
            }

            // Add items not already installed (ie not in localVersions list)
            for (const auto& pluginType : availableVersions.getKeys())
            {
               if (localVersions.find(pluginType) != localVersions.end())
                  continue; // Module already installed

               // Module not installed
               const auto availableModule = availableVersions.get<shared::CDataContainer>(pluginType);

               // Pass by a map to sort versions list
               std::map<std::string, shared::CDataContainer> newPluginAvailableVersions;
               try
               {
                  const auto availableVersionsForPlugin =
                     availableVersions.get<std::vector<shared::CDataContainer>>(pluginType);
                  for (auto& version : availableVersionsForPlugin)
                  {
                     shared::versioning::CVersion v(version.get<std::string>("version"));

                     // Don't add prereleases versions if not asked
                     if (!v.prerelease().empty() && !includePreleases)
                        continue;

                     shared::CDataContainer versionData;
                     versionData.set("downloadUrl", version.get<std::string>("downloadUrl"));
                     newPluginAvailableVersions[version.get<std::string>("version")] = versionData;
                  }
               }
               catch (std::exception& exception)
               {
                  YADOMS_LOG(warning) << "Invalid remote package for " << pluginType << ", will be ignored";
                  YADOMS_LOG(debug) << "exception : " << exception.what();
               }


               if (newPluginAvailableVersions.empty())
                  continue;

               shared::CDataContainer versions;
               // Sort (newer version first)
               shared::CDataContainer sortedVersions;
               for (auto v = newPluginAvailableVersions.rbegin(); v != newPluginAvailableVersions.rend(); ++v)
                  // Force different path char to not cut version string into subPaths
                  sortedVersions.set(v->first, v->second, 0);
               versions.set("versions", sortedVersions);

               // Find the newest version
               shared::CDataContainer newestVersion;
               // Force different path char to not cut version string into subPaths
               newestVersion.set(newPluginAvailableVersions.rbegin()->first,
                                 newPluginAvailableVersions.rbegin()->second, 0);
               versions.set("newest", newestVersion);

               try
               {
                  shared::CDataContainer item;
                  const auto& availableVersionsForPlugin = availableVersions.get<std::vector<shared::CDataContainer>>(
                     pluginType);
                  const auto& newestVersionLabel = newPluginAvailableVersions.rbegin()->first;
                  const auto& newestVersionData = std::find_if(availableVersionsForPlugin.begin(),
                                                               availableVersionsForPlugin.end(),
                                                               [&newestVersionLabel](
                                                               const shared::CDataContainer& availableVersionForPlugin)
                                                               {
                                                                  return availableVersionForPlugin.get<std::string>(
                                                                     "version") == newestVersionLabel;
                                                               });
                  newestVersionData->printToLog(YADOMS_LOG(debug));
                  item.set("iconUrl", newestVersionData->get<std::string>("iconUrl"));
                  item.set("versions", versions);

                  list.set(pluginType, item);
               }
               catch (std::exception& exception)
               {
                  YADOMS_LOG(warning) << "Invalid remote package for " << pluginType << ", will be ignored";
                  YADOMS_LOG(debug) << "exception : " << exception.what();
               }
            }


            YADOMS_LOG(debug) << "list : ";//TODO virer
            list.printToLog(YADOMS_LOG(debug));

            return list;
         }
      } //namespace service
   } //namespace rest
} //namespace web 
