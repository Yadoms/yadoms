#include "RestEndPoint.h"
#include "stdafx.h"
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

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "updates", getAvailableUpdatesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "updates", scanForUpdatesV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "updates/yadoms/{version}", updateYadomsV2)); //TODO à tester
            //TODO

            //TODO les mises à jour des composants sont parallélisables (on peur en faire plusieurs en même temps)
            //TODO Mais pas pendant une mise à jour de Yadoms

            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("yadoms")("update"), CUpdate::updateYadoms);

            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("update")("*"), CUpdate::updateWidget);
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("install"), CUpdate::installWidget);
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("remove")("*"), CUpdate::removeWidget);

            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("update")("*"), CUpdate::updatePlugin);
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("install"), CUpdate::installPlugin);
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("remove")("*"), CUpdate::removePlugin);

            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("update")("*"), CUpdate::updateScriptInterpreter);
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("install"), CUpdate::installScriptInterpreter);
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("remove")("*"), CUpdate::removeScriptInterpreter);

            return m_endPoints;
         }

         std::string CUpdate::findPackageUrl(UpdatePackage updatePackage,
                                             const std::string& version) const
         {
            const auto updates = m_updateManager->getUpdates(true);

            switch (updatePackage)
            {
            case UpdatePackage::kYadoms:
               {
                  // Use '|' separator instead of '.' because version contains '.'
                  if (updates->exists("yadoms|versions|newest|" + version + "|downloadUrl", '|'))
                     return updates->get("yadoms|versions|newest|" + version + "|downloadUrl", '|');
                  if (updates->exists("yadoms|versions|newer|" + version + "|downloadUrl", '|'))
                     return updates->get("yadoms|versions|newer|" + version + "|downloadUrl", '|');
                  if (updates->exists("yadoms|versions|older|" + version + "|downloadUrl", '|'))
                     return updates->get("yadoms|versions|older|" + version + "|downloadUrl", '|');
                  throw std::invalid_argument("Version not found");
               }
               //TODO
            default:
               throw std::invalid_argument("UpdatePackage");
            }
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

               const auto taskUid = m_updateManager->updateYadomsAsync(findPackageUrl(UpdatePackage::kYadoms,
                                                                                      version));
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
      } //namespace service
   } //namespace rest
} //namespace web 
