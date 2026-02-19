#include "stdafx.h"

#include <utility>
#include "Update.h"
#include "task/RunningTaskMutex.h"
#include "web/poco/RestDispatcherHelpers.hpp"
#include "web/poco/RestResult.h"


namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CUpdate::m_restKeyword = std::string("update");

         CUpdate::CUpdate(boost::shared_ptr<update::IUpdateManager> updateManager,
                          boost::shared_ptr<task::CScheduler> taskScheduler)
            : m_updateManager(std::move(updateManager)),
              m_taskScheduler(std::move(taskScheduler)),
              m_updateYadomsInProgressTaskUidHandler(boost::make_shared<task::CRunningTaskMutex>(m_taskScheduler))
         {
         }

         const std::string& CUpdate::getRestKeyword()
         {
            return m_restKeyword;
         }

         void CUpdate::configurePocoDispatcher(poco::CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("list")("*"), CUpdate::availableUpdatesV1)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("yadoms")("update"), CUpdate::updateYadomsV1)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("update")("*"), CUpdate::updateWidgetV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("install"), CUpdate::installWidgetV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("remove")("*"), CUpdate::removeWidgetV1)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("update")("*"), CUpdate::updatePluginV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("install"), CUpdate::installPluginV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("remove")("*"), CUpdate::removePluginV1)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("update")("*"), CUpdate::updateScriptInterpreterV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("install"), CUpdate::installScriptInterpreterV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("remove")("*"), CUpdate::removeScriptInterpreterV1)
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::availableUpdatesV1(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const
         {
            if (parameters.size() != 3)
               return poco::CRestResult::GenerateError("Invalid parameters in url /rest/update/list");

            const auto includePrereleases = parameters[2] == "includePreReleases";

            return poco::CRestResult::GenerateSuccess(m_updateManager->getUpdates(includePrereleases));
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::updateYadomsV1(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            try
            {
               //the request content should contain the downloadURL
               if (parameters.size() <= 2)
                  return poco::CRestResult::GenerateError("Not enough parameters in url /rest/update/yadoms/update");

               shared::CDataContainer content(requestContent);
               if (!content.containsValue("downloadUrl"))
                  return poco::CRestResult::GenerateError("The request should contains the downloadURL.");

               const auto downloadUrl = content.get<std::string>("downloadUrl");
               const auto taskId = m_updateManager->updateYadomsAsync(downloadUrl);
               shared::CDataContainer result;
               result.set("taskId", taskId);
               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to update Yadoms, ") + e.what());
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::updatePluginV1(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            //the request url should contain the pluginName
            //the request content should contain the downloadURL
            if (parameters.size() <= 3)
               return poco::CRestResult::GenerateError("Not enough parameters in url /rest/update/plugin/update/**pluginName**");

            const auto pluginName = parameters[3];

            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return poco::CRestResult::GenerateError("The request should contains the downloadURL.");

            const auto downloadUrl = content.get<std::string>("downloadUrl");
            const auto taskId = m_updateManager->updatePluginAsync(pluginName, downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return poco::CRestResult::GenerateSuccess(result);
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::installPluginV1(const std::vector<std::string>& parameters,
                                                                                              const std::string& requestContent) const
         {
            //the request content should contain the downloadURL
            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return poco::CRestResult::GenerateError("The request should contains the downloadURL.");

            const auto downloadUrl = content.get<std::string>("downloadUrl");
            const auto taskId = m_updateManager->installPluginAsync(downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return poco::CRestResult::GenerateSuccess(result);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::removePluginV1(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            //the request url should contain the pluginName
            if (parameters.size() <= 3)
               return poco::CRestResult::GenerateError("Not enough parameters in url /rest/plugin/remove/**pluginName**");

            const auto pluginName = parameters[3];
            const auto taskId = m_updateManager->removePluginAsync(pluginName);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return poco::CRestResult::GenerateSuccess(result);
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::updateWidgetV1(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            //the request url should contain the widgetName
            //the request content should contain the downloadURL
            if (parameters.size() <= 3)
               return poco::CRestResult::GenerateError("Not enough parameters in url /rest/widget/update/**widgetName**");

            const auto widgetName = parameters[3];

            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return poco::CRestResult::GenerateError("The request should contains the downloadURL.");

            const auto downloadUrl = content.get<std::string>("downloadUrl");
            const auto taskId = m_updateManager->updateWidgetAsync(widgetName, downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return poco::CRestResult::GenerateSuccess(result);
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::installWidgetV1(const std::vector<std::string>& parameters,
                                                                                              const std::string& requestContent) const
         {
            //the request content should contain the downloadURL
            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return poco::CRestResult::GenerateError("The request should contains the downloadURL.");

            const auto downloadUrl = content.get<std::string>("downloadUrl");
            const auto taskId = m_updateManager->installWidgetAsync(downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return poco::CRestResult::GenerateSuccess(result);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::removeWidgetV1(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            //the request url should contain the pluginName
            if (parameters.size() <= 3)
               return poco::CRestResult::GenerateError("Not enough parameters in url /rest/widget/remove/**widgetName**");

            const auto widgetName = parameters[3];
            const auto taskId = m_updateManager->removeWidgetAsync(widgetName);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return poco::CRestResult::GenerateSuccess(result);
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::updateScriptInterpreterV1(const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            //the request url should contain the scriptInterpreterName
            //the request content should contain the downloadURL
            if (parameters.size() <= 3)
               return poco::CRestResult::GenerateError(
                  "Not enough parameters in url /rest/scriptInterpreter/update/**scriptInterpreterName**");

            const auto scriptInterpreterName = parameters[3];

            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return poco::CRestResult::GenerateError("The request should contains the downloadURL.");

            const auto downloadUrl = content.get<std::string>("downloadUrl");
            const auto taskId = m_updateManager->updateScriptInterpreterAsync(scriptInterpreterName, downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return poco::CRestResult::GenerateSuccess(result);
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::installScriptInterpreterV1(const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            //the request content should contain the downloadURL
            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return poco::CRestResult::GenerateError("The request should contains the downloadURL.");

            const auto downloadUrl = content.get<std::string>("downloadUrl");
            const auto taskId = m_updateManager->installScriptInterpreterAsync(downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return poco::CRestResult::GenerateSuccess(result);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::removeScriptInterpreterV1(const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            //the request url should contain the scriptInterpreterName
            if (parameters.size() <= 3)
               return poco::CRestResult::GenerateError(
                  "Not enough parameters in url /rest/scriptInterpreter/remove/**scriptInterpreterName**");

            const auto scriptInterpreterName = parameters[3];
            const auto taskId = m_updateManager->removeScriptInterpreterAsync(scriptInterpreterName);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return poco::CRestResult::GenerateSuccess(result);
         }
      } //namespace service
   } //namespace rest
} //namespace web 
