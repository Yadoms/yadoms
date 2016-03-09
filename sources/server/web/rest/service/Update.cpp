#include "stdafx.h"
#include "Update.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"
#include "update/info/UpdateSite.h"


namespace web { namespace rest { namespace service {

   std::string CUpdate::m_restKeyword = std::string("update");

   DECLARE_ENUM_IMPLEMENTATION_NESTED(CUpdate::EWhatToDo, EWhatToDo,
      ((Update))
      ((Check))
   )

   CUpdate::CUpdate(boost::shared_ptr<update::CUpdateManager> updateManager)
      :m_updateManager(updateManager)
   {
   }


   CUpdate::~CUpdate()
   {
   }

   const std::string & CUpdate::getRestKeyword()
   {
      return m_restKeyword;
   }

   void CUpdate::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("yadoms")("list")("*"), CUpdate::availableYadomsVersions);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("yadoms")("update"), CUpdate::updateYadoms);
      
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("widget")("list")("*"), CUpdate::availableWidgets);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("update")("*"), CUpdate::updateWidget);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("install"), CUpdate::installWidget);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("remove")("*"), CUpdate::removeWidget);

      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("plugin")("list")("*"), CUpdate::availablePlugins);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("update")("*"), CUpdate::updatePlugin);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("install"), CUpdate::installPlugin);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("remove")("*"), CUpdate::removePlugin);

      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET" , (m_restKeyword)("scriptInterpreter")("list")("*"), CUpdate::availableScriptInterpreters);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("update")("*"), CUpdate::updateScriptInterpreter);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("install"), CUpdate::installScriptInterpreter);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("scriptInterpreter")("remove")("*"), CUpdate::removeScriptInterpreter);
   }

   shared::CDataContainer CUpdate::availableYadomsVersions(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::string lang = "";

      if (parameters.size()>3)
         lang = parameters[3];

      //ask site info
      return CResult::GenerateSuccess(update::info::CUpdateSite::getAllYadomsVersions(lang));
   }   
   
   shared::CDataContainer CUpdate::updateYadoms(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      if (requestContent.containsChild("versionData"))
      {
         shared::CDataContainer versionData = requestContent.get<shared::CDataContainer>("versionData");
         std::string taskId = m_updateManager->updateYadomsAsync(versionData);
         shared::CDataContainer result;
         result.set("taskId", taskId);
         return web::rest::CResult::GenerateSuccess(result);
      }
      else
      {
         requestContent.printToLog();
         return web::rest::CResult::GenerateError("The request should contains the versionData.");
      }
   }
   
   shared::CDataContainer CUpdate::availablePlugins(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::string lang = "";
         
      if (parameters.size()>3)
            lang = parameters[3];

      //ask site info
      return CResult::GenerateSuccess(update::info::CUpdateSite::getAllPluginVersions(lang));
   }



   shared::CDataContainer CUpdate::updatePlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request url should contain the pluginName
      //the request content should contain the downloadURL
      if (parameters.size() > 3)
      {
         std::string pluginName = parameters[3];

         if (requestContent.containsValue("downloadUrl"))
         {
            std::string downloadUrl = requestContent.get<std::string>("downloadUrl");
            std::string taskId = m_updateManager->updatePluginAsync(pluginName, downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return web::rest::CResult::GenerateSuccess(result);
         }
         else
         {
            return web::rest::CResult::GenerateError("The request should contains the downloadURL.");
         }
      }
      else
      {
         return web::rest::CResult::GenerateError("Not enougth parameters in url /rest/plugin/update/**pluginName**");
      }
   }

     
   shared::CDataContainer CUpdate::installPlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request content should contain the downloadURL
      if (requestContent.containsValue("downloadUrl"))
      {
         std::string downloadUrl = requestContent.get<std::string>("downloadUrl");
         std::string taskId = m_updateManager->installPluginAsync(downloadUrl);
         shared::CDataContainer result;
         result.set("taskId", taskId);
         return web::rest::CResult::GenerateSuccess(result);
      }
      else
      {
         return web::rest::CResult::GenerateError("The request should contains the downloadURL.");
      }
   }

   shared::CDataContainer CUpdate::removePlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request url should contain the pluginName
      if (parameters.size() > 3)
      {
         std::string pluginName = parameters[3];
         std::string taskId = m_updateManager->removePluginAsync(pluginName);
         shared::CDataContainer result;
         result.set("taskId", taskId);
         return web::rest::CResult::GenerateSuccess(result);
      }
      else
      {
         return web::rest::CResult::GenerateError("Not enougth parameters in url /rest/plugin/remove/**pluginName**");
      }
   }



   shared::CDataContainer CUpdate::availableWidgets(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::string lang = "";

      if (parameters.size()>3)
         lang = parameters[3];

      //ask site info
      return CResult::GenerateSuccess(update::info::CUpdateSite::getAllWidgetsVersions(lang));
   }



   shared::CDataContainer CUpdate::updateWidget(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request url should contain the widgetName
      //the request content should contain the downloadURL
      if (parameters.size() > 3)
      {
         std::string widgetName = parameters[3];

         if (requestContent.containsValue("downloadUrl"))
         {
            std::string downloadUrl = requestContent.get<std::string>("downloadUrl");
            std::string taskId = m_updateManager->updateWidgetAsync(widgetName, downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return web::rest::CResult::GenerateSuccess(result);
         }
         else
         {
            return web::rest::CResult::GenerateError("The request should contains the downloadURL.");
         }
      }
      else
      {
         return web::rest::CResult::GenerateError("Not enougth parameters in url /rest/widget/update/**widgetName**");
      }
   }


   shared::CDataContainer CUpdate::installWidget(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request content should contain the downloadURL
      if (requestContent.containsValue("downloadUrl"))
      {
         std::string downloadUrl = requestContent.get<std::string>("downloadUrl");
         std::string taskId = m_updateManager->installWidgetAsync(downloadUrl);
         shared::CDataContainer result;
         result.set("taskId", taskId);
         return web::rest::CResult::GenerateSuccess(result);
      }
      else
      {
         return web::rest::CResult::GenerateError("The request should contains the downloadURL.");
      }
   }

   shared::CDataContainer CUpdate::removeWidget(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request url should contain the pluginName
      if (parameters.size() > 3)
      {
         std::string widgetName = parameters[3];
         std::string taskId = m_updateManager->removeWidgetAsync(widgetName);
         shared::CDataContainer result;
         result.set("taskId", taskId);
         return web::rest::CResult::GenerateSuccess(result);
      }
      else
      {
         return web::rest::CResult::GenerateError("Not enougth parameters in url /rest/widget/remove/**widgetName**");
      }
   }

   shared::CDataContainer CUpdate::availableScriptInterpreters(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::string lang = "";

      if (parameters.size()>3)
         lang = parameters[3];

      //ask site info
      return CResult::GenerateSuccess(update::info::CUpdateSite::getAllScriptInterpreterVersions(lang));
   }



   shared::CDataContainer CUpdate::updateScriptInterpreter(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request url should contain the scriptInterpreterName
      //the request content should contain the downloadURL
      if (parameters.size() > 3)
      {
         std::string scriptInterpreterName = parameters[3];

         if (requestContent.containsValue("downloadUrl"))
         {
            std::string downloadUrl = requestContent.get<std::string>("downloadUrl");
            std::string taskId = m_updateManager->updateScriptInterpreterAsync(scriptInterpreterName, downloadUrl);
            shared::CDataContainer result;
            result.set("taskId", taskId);
            return web::rest::CResult::GenerateSuccess(result);
         }
         else
         {
            return web::rest::CResult::GenerateError("The request should contains the downloadURL.");
         }
      }
      else
      {
         return web::rest::CResult::GenerateError("Not enougth parameters in url /rest/scriptInterpreter/update/**scriptInterpreterName**");
      }
   }


   shared::CDataContainer CUpdate::installScriptInterpreter(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request content should contain the downloadURL
      if (requestContent.containsValue("downloadUrl"))
      {
         std::string downloadUrl = requestContent.get<std::string>("downloadUrl");
         std::string taskId = m_updateManager->installScriptInterpreterAsync(downloadUrl);
         shared::CDataContainer result;
         result.set("taskId", taskId);
         return web::rest::CResult::GenerateSuccess(result);
      }
      else
      {
         return web::rest::CResult::GenerateError("The request should contains the downloadURL.");
      }
   }

   shared::CDataContainer CUpdate::removeScriptInterpreter(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request url should contain the scriptInterpreterName
      if (parameters.size() > 3)
      {
         std::string scriptInterpreterName = parameters[3];
         std::string taskId = m_updateManager->removeScriptInterpreterAsync(scriptInterpreterName);
         shared::CDataContainer result;
         result.set("taskId", taskId);
         return web::rest::CResult::GenerateSuccess(result);
      }
      else
      {
         return web::rest::CResult::GenerateError("Not enougth parameters in url /rest/scriptInterpreter/remove/**scriptInterpreterName**");
      }
   }





} //namespace service
} //namespace rest
} //namespace web 
