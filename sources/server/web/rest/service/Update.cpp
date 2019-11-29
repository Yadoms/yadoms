﻿#include "stdafx.h"
#include "Update.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"


namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CUpdate::m_restKeyword = std::string("update");

         CUpdate::CUpdate(boost::shared_ptr<update::CUpdateManager> updateManager)
            : m_updateManager(updateManager)
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
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scan"), CUpdate::scanForUpdates);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("list")("*"), CUpdate::availableUpdates);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("yadoms")("update"), CUpdate::updateYadoms);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("update")("*"), CUpdate:: updateWidget);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("install"), CUpdate::installWidget );
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("remove")("*"), CUpdate:: removeWidget);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("update")("*"), CUpdate:: updatePlugin);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("install"), CUpdate::installPlugin );
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("remove")("*"), CUpdate:: removePlugin);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("update")("*"), CUpdate ::updateScriptInterpreter);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("install"), CUpdate:: installScriptInterpreter);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("remove")("*"), CUpdate ::removeScriptInterpreter);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::scanForUpdates(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            const auto taskId = m_updateManager->scanForUpdatesAsync();
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::availableUpdates(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const
         {
            if (parameters.size() != 3)
               return CResult::GenerateError("Invalid parameters in url /rest/update/list");

            const auto includePrereleases = parameters[2] == "includePreReleases";

            return CResult::GenerateSuccess(m_updateManager->getUpdates(includePrereleases));
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::updateYadoms(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            try
            {
               //the request content should contain the downloadURL
               if (parameters.size() <= 2)
                  return CResult::GenerateError("Not enough parameters in url /rest/update/yadoms/update");

               shared::CDataContainer content(requestContent);
               if (!content.containsValue("downloadUrl"))
                  return CResult::GenerateError("The request should contains the downloadURL.");

               const auto downloadUrl = content.get<std::string>("downloadUrl");
               const auto taskId = m_updateManager->updateYadomsAsync(downloadUrl);
               shared::CDataContainer result;
               result.set("taskId", taskId);
               return CResult::GenerateSuccess(result);
            }
            catch (std::exception& e)
            {
               return CResult::GenerateError(std::string("Fail to update Yadoms, ") + e.what());
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::updatePlugin(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            //the request url should contain the pluginName
            //the request content should contain the downloadURL
            if (parameters.size() <= 3)
               return CResult::GenerateError("Not enough parameters in url /rest/update/plugin/update/**pluginName**");

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


         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::installPlugin(const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const
         {
            //the request content should contain the downloadURL
            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return CResult::GenerateError("The request should contains the downloadURL.");

            const auto downloadUrl = content.get<std::string>("downloadUrl");
            const auto taskId = m_updateManager->installPluginAsync(downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::removePlugin(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            //the request url should contain the pluginName
            if (parameters.size() <= 3)
               return CResult::GenerateError("Not enough parameters in url /rest/plugin/remove/**pluginName**");

            const auto pluginName = parameters[3];
            const auto taskId = m_updateManager->removePluginAsync(pluginName);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::updateWidget(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            //the request url should contain the widgetName
            //the request content should contain the downloadURL
            if (parameters.size() <= 3)
               return CResult::GenerateError("Not enough parameters in url /rest/widget/update/**widgetName**");

            const auto widgetName = parameters[3];

            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return CResult::GenerateError("The request should contains the downloadURL.");

            const auto downloadUrl = content.get<std::string>("downloadUrl");
            const auto taskId = m_updateManager->updateWidgetAsync(widgetName, downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::installWidget(const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const
         {
            //the request content should contain the downloadURL
            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return CResult::GenerateError("The request should contains the downloadURL.");

            const auto downloadUrl = content.get<std::string>("downloadUrl");
            const auto taskId = m_updateManager->installWidgetAsync(downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::removeWidget(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            //the request url should contain the pluginName
            if (parameters.size() <= 3)
               return CResult::GenerateError("Not enough parameters in url /rest/widget/remove/**widgetName**");

            const auto widgetName = parameters[3];
            const auto taskId = m_updateManager->removeWidgetAsync(widgetName);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::updateScriptInterpreter(const std::vector<std::string>& parameters,
                                                                                                      const std::string& requestContent) const
         {
            //the request url should contain the scriptInterpreterName
            //the request content should contain the downloadURL
            if (parameters.size() <= 3)
               return CResult::GenerateError(
                  "Not enough parameters in url /rest/scriptInterpreter/update/**scriptInterpreterName**");

            const auto scriptInterpreterName = parameters[3];

            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return CResult::GenerateError("The request should contains the downloadURL.");

            const auto downloadUrl = content.get<std::string>("downloadUrl");
            const auto taskId = m_updateManager->updateScriptInterpreterAsync(scriptInterpreterName, downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::installScriptInterpreter(const std::vector<std::string>& parameters,
                                                                                                       const std::string& requestContent) const
         {
            //the request content should contain the downloadURL
            shared::CDataContainer content(requestContent);
            if (!content.containsValue("downloadUrl"))
               return CResult::GenerateError("The request should contains the downloadURL.");

            const auto downloadUrl = content.get<std::string>("downloadUrl");
            const auto taskId = m_updateManager->installScriptInterpreterAsync(downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CUpdate::removeScriptInterpreter(const std::vector<std::string>& parameters,
                                                                                                      const std::string& requestContent) const
         {
            //the request url should contain the scriptInterpreterName
            if (parameters.size() <= 3)
               return CResult::GenerateError(
                  "Not enough parameters in url /rest/scriptInterpreter/remove/**scriptInterpreterName**");

            const auto scriptInterpreterName = parameters[3];
            const auto taskId = m_updateManager->removeScriptInterpreterAsync(scriptInterpreterName);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return CResult::GenerateSuccess(result);
         }
      } //namespace service
   } //namespace rest
} //namespace web 
