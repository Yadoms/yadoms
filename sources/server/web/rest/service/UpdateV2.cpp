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
            //TODO

            
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

         boost::shared_ptr<IAnswer> CUpdate::getAvailableUpdatesV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               const auto includePrereleases = request->queryParamExists("include-prereleases");
               const auto updates = m_updateManager->getUpdates(includePrereleases);

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
      } //namespace service
   } //namespace rest
} //namespace web 
