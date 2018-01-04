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
                          boost::shared_ptr<update::worker::IUpdateChecker> updateChecker)
            : m_updateManager(updateManager),
              m_updateChecker(updateChecker)
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
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("list")("*")("*"), CUpdate::availableUpdates);

            //TODO faire le ménage
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("yadoms")("list")("*"), CUpdate:: availableYadomsVersions);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("yadoms")("update"), CUpdate::updateYadoms);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("widget")("list")("*"), CUpdate:: availableWidgets);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("update")("*"), CUpdate:: updateWidget);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("install"), CUpdate::installWidget );
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("remove")("*"), CUpdate:: removeWidget);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("plugin")("list")("*")("*"), CUpdate:: availablePlugins);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("update")("*"), CUpdate:: updatePlugin);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("install"), CUpdate::installPlugin );
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("remove")("*"), CUpdate:: removePlugin);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET" , (m_restKeyword)("scriptInterpreter")("list")("*"), CUpdate:: availableScriptInterpreters);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("update")("*"), CUpdate ::updateScriptInterpreter);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("install"), CUpdate:: installScriptInterpreter);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("remove")("*"), CUpdate ::removeScriptInterpreter);
         }

         shared::CDataContainer CUpdate::availableUpdates(const std::vector<std::string>& parameters,
                                                          const std::string& requestContent) const
         {
            if (parameters.size() != 4)
               return CResult::GenerateError("Invalid parameters in url /rest/update/list");

            const auto includePreleases = parameters[2] == "includePreReleases";
            const auto lang = parameters[3];//TODO à gérer ou enlever ?

            return CResult::GenerateSuccess(m_updateChecker->getUpdates(includePreleases));
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
            const auto lang = parameters[4];//TODO à gérer ou enlever ?

            return CResult::GenerateSuccess(m_updateChecker->getUpdates(includePreleases));
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
            m_updateChecker->forceRebuildUpdates();
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
            m_updateChecker->forceRebuildUpdates();
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
      } //namespace service
   } //namespace rest
} //namespace web 
