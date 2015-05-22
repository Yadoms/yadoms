#include "stdafx.h"
#include "Update.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"
#include "update/source/Yadoms.h"
#include "update/source/Plugin.h"
#include "update/source/Widget.h"
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
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("yadoms")("*"), CUpdate::forYadoms);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("*")("*"), CUpdate::forPlugin);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("widget")("*")("*"), CUpdate::forWidget);

      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("plugin")("list")("*"), CUpdate::availablePlugins);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("update")("*"), CUpdate::updatePlugin);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("install"), CUpdate::installPlugin);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("plugin")("remove")("*"), CUpdate::removePlugin);
   }

   shared::CDataContainer CUpdate::forYadoms(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      if(parameters.size()>2)
      {
         std::string whatToDo = parameters[2];
         boost::shared_ptr<update::source::CYadoms> updateSource(new update::source::CYadoms());
         return postRequest(updateSource, whatToDo);
      }
      else
      {
         return web::rest::CResult::GenerateError("Invalid parameter count (need what to do in request : update/check )");
      }
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
            return web::rest::CResult::GenerateSuccess("Update in progess");
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
         return web::rest::CResult::GenerateSuccess("Installation in progess");
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
         return web::rest::CResult::GenerateSuccess("Remove in progess");
      }
      else
      {
         return web::rest::CResult::GenerateError("Not enougth parameters in url /rest/plugin/remove/**pluginName**");
      }
   }


   shared::CDataContainer CUpdate::forPlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      if(parameters.size()>2)
      {
         std::string pluginName = parameters[2];
         std::string whatToDo = parameters[3];

         boost::shared_ptr<update::source::CPlugin> updateSource(new update::source::CPlugin(pluginName, tools::CVersion()));
         return postRequest(updateSource, whatToDo);
      }
      else
      {
         return web::rest::CResult::GenerateError("Invalid parameter count (need what to do in request : update/check )");
      }
   }

   shared::CDataContainer CUpdate::forWidget(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      if(parameters.size()>2)
      {
         std::string widgetName = parameters[2];
         std::string whatToDo = parameters[3];
         boost::shared_ptr<update::source::CWidget> updateSource(new update::source::CWidget());
         return postRequest(updateSource, whatToDo);
      }
      else
      {
         return web::rest::CResult::GenerateError("Invalid parameter count (need what to do in request : update/check )");
      }
   }

   shared::CDataContainer CUpdate::postRequest(boost::shared_ptr<update::source::IUpdateSource> source, const std::string whatToDo)
   {
      bool result = false;

      if (EWhatToDo::isDefined(whatToDo))
      {
         EWhatToDo wtd = EWhatToDo::parse(whatToDo);

         switch (wtd)
         {
         case EWhatToDo::kUpdateValue:
            result = m_updateManager->updateAsync(source); 
            break;
         case EWhatToDo::kCheckValue:
            result = m_updateManager->checkForUpdateAsync(source); 
            break;
         default:
            result = false;
            break;
         }

         if (result)
            return web::rest::CResult::GenerateSuccess();
         return web::rest::CResult::GenerateError("Fail to process " + wtd.toString());
      }
      else
      {
         return web::rest::CResult::GenerateError("Fail to interpret " + whatToDo);
      }
     
   }
} //namespace service
} //namespace rest
} //namespace web 
