#include "RestEndPoint.h"
#include "stdafx.h"
#include "TaskInProgressHandler.h"
#include "Update.h"
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
         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CUpdate::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();

            //TODO revoir la gestion du scan. Lancer le scan si :
            // - au démarrage (avec petit délai pour ne pas allourdir le démarrage ?)
            // - après chaque update/install/remove de Yadoms ou composant
            // - une fois par jour

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "updates", getAvailableUpdatesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "updates", scanForUpdatesV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "updates/yadoms/{version}", updateYadomsV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "updates/plugins/{pluginName}/{version}", updatePluginV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "updates/plugins/{pluginName}", removePluginV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "updates/widgets/{widgetName}/{version}", updateWidgetV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "updates/widgets/{widgetName}", removeWidgetV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "updates/scriptInterpreters/{scriptInterpreterName}/{version}", updateScriptInterpreterV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "updates/scriptInterpreters/{scriptInterpreterName}", removeScriptInterpreterV2));

            return m_endPoints;
         }

         std::string CUpdate::findYadomsPackageUrl(const std::string& version) const
         {
            const auto updates = m_updateManager->getUpdates(true);

            // Use '|' separator instead of '.' because version contains '.'
            if (updates->exists("yadoms|versions|newest|" + version + "|downloadUrl", '|'))
               return updates->get("yadoms|versions|newest|" + version + "|downloadUrl", '|');
            if (updates->exists("yadoms|versions|newer|" + version + "|downloadUrl", '|'))
               return updates->get("yadoms|versions|newer|" + version + "|downloadUrl", '|');
            if (updates->exists("yadoms|versions|older|" + version + "|downloadUrl", '|'))
               return updates->get("yadoms|versions|older|" + version + "|downloadUrl", '|');
            throw std::invalid_argument("Version not found");
         }

         std::pair<bool, std::string> CUpdate::findComponentPackageUrl(const std::string& componentTag,
                                                                       const std::string& componentName,
                                                                       const std::string& version) const
         {
            const auto updates = m_updateManager->getUpdates(true);

            YADOMS_LOG(debug) << updates->serialize(); //TODO virer

            // Use '|' separator instead of '.' because version contains '.'

            // Updateable component
            if (updates->exists(componentTag + "|updateable|" + componentName + "|versions|newest|" + version + "|downloadUrl", '|'))
               return std::make_pair(false,
                                     updates->get(componentTag + "|updateable|" + componentName + "|versions|newest|" + version + "|downloadUrl",
                                                  '|'));
            if (updates->exists(componentTag + "|updateable|" + componentName + "|versions|newer|" + version + "|downloadUrl", '|'))
               return std::make_pair(false,
                                     updates->get(componentTag + "|updateable|" + componentName + "|versions|newer|" + version + "|downloadUrl",
                                                  '|'));
            if (updates->exists(componentTag + "|updateable|" + componentName + "|versions|older|" + version + "|downloadUrl", '|'))
               return std::make_pair(false,
                                     updates->get(componentTag + "|updateable|" + componentName + "|versions|older|" + version + "|downloadUrl",
                                                  '|'));

            // New component
            if (updates->exists(componentTag + "|new|" + componentName + "|versions|newest|" + version + "|downloadUrl", '|'))
               return std::make_pair(true,
                                     updates->get(componentTag + "|new|" + componentName + "|versions|newest|" + version + "|downloadUrl", '|'));
            if (updates->exists(componentTag + "|new|" + componentName + "|versions|versions|" + version + "|downloadUrl", '|'))
               return std::make_pair(true,
                                     updates->get(componentTag + "|new|" + componentName + "|versions|versions|" + version + "|downloadUrl",
                                                  '|'));
            throw std::invalid_argument("Version not found");
         }

         void CUpdate::extractVersions(const boost::shared_ptr<shared::CDataContainer>& updates,
                                       const std::string& path)
         {
            if (!updates->exists(path))
               return;

            const auto versions = updates->getKeys(path);
            updates->remove(path);
            updates->set(path, versions);
         }

         void CUpdate::extractComponentVersions(const boost::shared_ptr<shared::CDataContainer>& updates,
                                                const std::string& componentTag)
         {
            auto nodePath = componentTag + ".updateable";
            if (updates->exists(nodePath))
            {
               for (const auto& plugin : updates->getKeys(nodePath))
               {
                  auto versionPath = nodePath;
                  versionPath.append(".").append(plugin).append(".versions.");
                  extractVersions(updates, versionPath + "newest");
                  extractVersions(updates, versionPath + "newer");
                  extractVersions(updates, versionPath + "older");
               }
            }

            nodePath = componentTag + ".new";
            if (updates->exists(nodePath))
            {
               for (const auto& plugin : updates->getKeys(nodePath))
               {
                  auto versionPath = nodePath;
                  versionPath.append(".").append(plugin).append(".versions.");
                  extractVersions(updates, versionPath + "newest");
                  extractVersions(updates, versionPath + "versions");
               }
            }
         }

         boost::shared_ptr<shared::CDataContainer> CUpdate::formatUpdates(const boost::shared_ptr<shared::CDataContainer>& availableUpdates)
         {
            auto updates = availableUpdates->copy();

            extractVersions(updates, "yadoms.versions.newest");
            extractVersions(updates, "yadoms.versions.newer");
            extractVersions(updates, "yadoms.versions.older");

            extractComponentVersions(updates, "plugins");
            extractComponentVersions(updates, "widgets");
            extractComponentVersions(updates, "scriptInterpreters");

            return updates;
         }

         boost::shared_ptr<IAnswer> CUpdate::getAvailableUpdatesV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               const auto includePrereleases = request->queryParamExists("include-prereleases");
               const auto updates = formatUpdates(m_updateManager->getUpdates(includePrereleases));

               if (updates->empty())
                  return boost::make_shared<CNoContentAnswer>();

               return boost::make_shared<CSuccessAnswer>(*updates);
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available updates");
            }
         }

         boost::shared_ptr<IAnswer> CUpdate::scanForUpdatesV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               if (!m_scanForUpdatesInProgressTaskUidHandler->inProgressTaskUid().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "task already in progress");

               const auto taskUid = m_updateManager->scanForUpdatesAsync();
               m_scanForUpdatesInProgressTaskUidHandler->setInProgressTaskUid(taskUid);

               YADOMS_LOG(information) << "Task : " << taskUid << " successfully started";

               return CHelpers::createLongRunningOperationAnswer(taskUid);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to start task : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to start task");
            }
         }

         boost::shared_ptr<IAnswer> CUpdate::updateYadomsV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               const auto version = request->pathVariable("version");

               if (!m_updateYadomsInProgressTaskUidHandler->inProgressTaskUid().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "task already in progress");

               const auto taskUid = m_updateManager->updateYadomsAsync(findYadomsPackageUrl(version));
               m_updateYadomsInProgressTaskUidHandler->setInProgressTaskUid(taskUid);

               YADOMS_LOG(information) << "Task : " << taskUid << " successfully started";

               return CHelpers::createLongRunningOperationAnswer(taskUid);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to update Yadoms : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to update Yadoms");
            }
         }

         boost::shared_ptr<IAnswer> CUpdate::updateComponentV2(
            const std::string& componentName,
            std::map<std::string, boost::shared_ptr<ITaskInProgressHandler>>& updateComponentsInProgressTaskUidHandler,
            const std::function<std::string()>& updateTask) const
         {
            try
            {
               if (!m_updateYadomsInProgressTaskUidHandler->inProgressTaskUid().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "Yadoms update is in progress");

               const auto inProgressTaskHandler = updateComponentsInProgressTaskUidHandler.find(componentName);
               if (inProgressTaskHandler == updateComponentsInProgressTaskUidHandler.end())
                  updateComponentsInProgressTaskUidHandler[componentName] = boost::make_shared<CTaskInProgressHandler>(m_taskScheduler);
               else if (!inProgressTaskHandler->second->inProgressTaskUid().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "task already in progress");

               const auto taskUid = updateTask();

               updateComponentsInProgressTaskUidHandler[componentName]->setInProgressTaskUid(taskUid);

               YADOMS_LOG(information) << "Task : " << taskUid << " successfully started";

               return CHelpers::createLongRunningOperationAnswer(taskUid);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to update component " << componentName << " : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to update component");
            }
         }

         boost::shared_ptr<IAnswer> CUpdate::removeComponentV2(
            const std::string& componentName,
            std::map<std::string, boost::shared_ptr<ITaskInProgressHandler>>& updateComponentsInProgressTaskUidHandler,
            const std::function<std::string()>& removeTask) const
         {
            try
            {
               if (!m_updateYadomsInProgressTaskUidHandler->inProgressTaskUid().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "Yadoms update is in progress");

               const auto inProgressTaskHandler = updateComponentsInProgressTaskUidHandler.find(componentName);
               if (inProgressTaskHandler == updateComponentsInProgressTaskUidHandler.end())
                  updateComponentsInProgressTaskUidHandler[componentName] = boost::make_shared<CTaskInProgressHandler>(m_taskScheduler);
               else if (!inProgressTaskHandler->second->inProgressTaskUid().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                     "task already in progress");

               const auto taskUid = removeTask();

               updateComponentsInProgressTaskUidHandler[componentName]->setInProgressTaskUid(taskUid);

               YADOMS_LOG(information) << "Task : " << taskUid << " successfully started";

               return CHelpers::createLongRunningOperationAnswer(taskUid);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to remove component : " << componentName << " : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to remove component");
            }
         }

         boost::shared_ptr<IAnswer> CUpdate::updatePluginV2(const boost::shared_ptr<IRequest>& request)
         {
            try
            {
               const auto pluginName = request->pathVariable("pluginName");
               const auto version = request->pathVariable("version");

               return updateComponentV2("plugin/" + pluginName,
                                        m_updatePluginsInProgressTaskUidHandler,
                                        [&pluginName, &version, this]()
                                        {
                                           const auto componentUrl = findComponentPackageUrl("plugins",
                                                                                             pluginName,
                                                                                             version);

                                           return componentUrl.first
                                                     ? m_updateManager->installPluginAsync(componentUrl.second)
                                                     : m_updateManager->updatePluginAsync(pluginName,
                                                                                          componentUrl.second);
                                        });
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to update plugin : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to update plugin");
            }
         }

         boost::shared_ptr<IAnswer> CUpdate::removePluginV2(const boost::shared_ptr<IRequest>& request)
         {
            try
            {
               const auto pluginName = request->pathVariable("pluginName");

               return removeComponentV2(pluginName,
                                        m_updatePluginsInProgressTaskUidHandler,
                                        [&pluginName, this]()
                                        {
                                           return m_updateManager->removePluginAsync(pluginName);
                                        });
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to remove plugin : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to remove plugin");
            }
         }

         boost::shared_ptr<IAnswer> CUpdate::updateWidgetV2(const boost::shared_ptr<IRequest>& request)
         {
            try
            {
               const auto widgetName = request->pathVariable("widgetName");
               const auto version = request->pathVariable("version");

               return updateComponentV2("widget/" + widgetName,
                                        m_updateWidgetsInProgressTaskUidHandler,
                                        [&widgetName, &version, this]()
                                        {
                                           const auto componentUrl = findComponentPackageUrl("widgets",
                                                                                             widgetName,
                                                                                             version);

                                           return componentUrl.first
                                                     ? m_updateManager->installWidgetAsync(componentUrl.second)
                                                     : m_updateManager->updateWidgetAsync(widgetName,
                                                                                          componentUrl.second);
                                        });
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to update widget : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to update widget");
            }
         }

         boost::shared_ptr<IAnswer> CUpdate::removeWidgetV2(const boost::shared_ptr<IRequest>& request)
         {
            try
            {
               const auto widgetName = request->pathVariable("widgetName");

               return removeComponentV2(widgetName,
                                        m_updateWidgetsInProgressTaskUidHandler,
                                        [&widgetName, this]()
                                        {
                                           return m_updateManager->removeWidgetAsync(widgetName);
                                        });
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to remove widget : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to remove widget");
            }
         }

         boost::shared_ptr<IAnswer> CUpdate::updateScriptInterpreterV2(const boost::shared_ptr<IRequest>& request)
         {
            try
            {
               const auto scriptInterpreterName = request->pathVariable("scriptInterpreterName");
               const auto version = request->pathVariable("version");

               return updateComponentV2("scriptInterpreter/" + scriptInterpreterName,
                                        m_updateScriptInterpretersInProgressTaskUidHandler,
                                        [&scriptInterpreterName, &version, this]()
                                        {
                                           const auto componentUrl = findComponentPackageUrl("scriptInterpreters",
                                                                                             scriptInterpreterName,
                                                                                             version);

                                           return componentUrl.first
                                                     ? m_updateManager->installScriptInterpreterAsync(componentUrl.second)
                                                     : m_updateManager->updateScriptInterpreterAsync(scriptInterpreterName,
                                                        componentUrl.second);
                                        });
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to update script interpreter : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to update script interpreter");
            }
         }

         boost::shared_ptr<IAnswer> CUpdate::removeScriptInterpreterV2(const boost::shared_ptr<IRequest>& request)
         {
            try
            {
               const auto scriptInterpreterName = request->pathVariable("scriptInterpreterName");

               return removeComponentV2(scriptInterpreterName,
                                        m_updateScriptInterpretersInProgressTaskUidHandler,
                                        [&scriptInterpreterName, this]()
                                        {
                                           return m_updateManager->removeScriptInterpreterAsync(scriptInterpreterName);
                                        });
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to remove script interpreter : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to remove script interpreter");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
