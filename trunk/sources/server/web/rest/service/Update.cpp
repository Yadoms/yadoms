#include "stdafx.h"
#include "Update.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"
#include "update/info/UpdateSite.h"
#include <shared/ServiceLocator.h>
#include "IRunningInformation.h"
#include "startupOptions/IStartupOptions.h"

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
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("yadoms")("check"), CUpdate::checkForYadomsUpdate);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("yadoms")("update"), CUpdate::updateYadoms);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("yadoms")("update")("last"), CUpdate::updateYadomsToLastVersion);
      
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("widget")("list")("*"), CUpdate::availableWidgets);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("widget")("update")("*"), CUpdate::updateWidget);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("widget")("install"), CUpdate::installWidget);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("widget")("remove")("*"), CUpdate::removeWidget);

      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("plugin")("list")("*"), CUpdate::availablePlugins);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("update")("*"), CUpdate::updatePlugin);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("install"), CUpdate::installPlugin);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("remove")("*"), CUpdate::removePlugin);
   }

   shared::CDataContainer CUpdate::checkForYadomsUpdate(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      m_updateManager->checkForYadomsUpdateAsync();
      return web::rest::CResult::GenerateSuccess("Check for Yadoms update in progress");
   }   
   
   shared::CDataContainer CUpdate::updateYadoms(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      if (requestContent.containsChild("versionData"))
      {
         shared::CDataContainer versionData = requestContent.get<shared::CDataContainer>("versionData");
         m_updateManager->updateYadomsAsync(versionData);
         return web::rest::CResult::GenerateSuccess("Updating Yadoms in progress");
      }
      else
      {
         requestContent.printToLog();
         return web::rest::CResult::GenerateError("The request should contains the versionData.");
      }
   }
   
   shared::CDataContainer CUpdate::updateYadomsToLastVersion(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      m_updateManager->updateYadomsAsync(shared::CDataContainer::EmptyContainer);
      return web::rest::CResult::GenerateSuccess("Updating Yadoms in progress");
   }

   
   

   shared::CDataContainer CUpdate::availablePlugins(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::string lang = "";
         
      if (parameters.size()>2)
            lang = parameters[3];

      //make some initializations
      boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
      boost::shared_ptr<startupOptions::IStartupOptions> startupOptions(shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>());

      //ask site info
      update::info::CUpdateSite updateSite(startupOptions, runningInformation);
      return CResult::GenerateSuccess(updateSite.getAllPluginVersions(lang));
   }



   shared::CDataContainer CUpdate::updatePlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request url should contain the pluginName
      //the request content should contain the downloadURL
      if (parameters.size() > 2)
      {
         std::string pluginName = parameters[3];

         if (requestContent.containsValue("downloadUrl"))
         {
            std::string downloadUrl = requestContent.get<std::string>("downloadUrl");
            m_updateManager->updatePluginAsync(pluginName, downloadUrl);
            return web::rest::CResult::GenerateSuccess("Update a plugin in progess");
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
         m_updateManager->installPluginAsync(downloadUrl);
         return web::rest::CResult::GenerateSuccess("Installation of a plugin in progess");
      }
      else
      {
         return web::rest::CResult::GenerateError("The request should contains the downloadURL.");
      }
   }

   shared::CDataContainer CUpdate::removePlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request url should contain the pluginName
      if (parameters.size() > 2)
      {
         std::string pluginName = parameters[3];
         m_updateManager->removePluginAsync(pluginName);
         return web::rest::CResult::GenerateSuccess("Remove a plugin in progess");
      }
      else
      {
         return web::rest::CResult::GenerateError("Not enougth parameters in url /rest/plugin/remove/**pluginName**");
      }
   }























   shared::CDataContainer CUpdate::availableWidgets(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::string lang = "";

      if (parameters.size()>2)
         lang = parameters[3];

      //make some initializations
      boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
      boost::shared_ptr<startupOptions::IStartupOptions> startupOptions(shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>());

      //ask site info
      update::info::CUpdateSite updateSite(startupOptions, runningInformation);
      return CResult::GenerateSuccess(updateSite.getAllWidgetsVersions(lang));
   }



   shared::CDataContainer CUpdate::updateWidget(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request url should contain the widgetName
      //the request content should contain the downloadURL
      if (parameters.size() > 2)
      {
         std::string widgetName = parameters[3];

         if (requestContent.containsValue("downloadUrl"))
         {
            std::string downloadUrl = requestContent.get<std::string>("downloadUrl");
            m_updateManager->updateWidgetAsync(widgetName, downloadUrl);
            return web::rest::CResult::GenerateSuccess("Update a widget in progess");
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
         m_updateManager->installWidgetAsync(downloadUrl);
         return web::rest::CResult::GenerateSuccess("Installation of a widget in progess");
      }
      else
      {
         return web::rest::CResult::GenerateError("The request should contains the downloadURL.");
      }
   }

   shared::CDataContainer CUpdate::removeWidget(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //the request url should contain the pluginName
      if (parameters.size() > 2)
      {
         std::string widgetName = parameters[3];
         m_updateManager->removeWidgetAsync(widgetName);
         return web::rest::CResult::GenerateSuccess("Remove a widget in progess");
      }
      else
      {
         return web::rest::CResult::GenerateError("Not enougth parameters in url /rest/widget/remove/**widgetName**");
      }
   }


} //namespace service
} //namespace rest
} //namespace web 
