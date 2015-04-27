#include "stdafx.h"
#include "Update.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"
#include "update/source/Yadoms.h"
#include "update/source/Plugin.h"
#include "update/source/Widget.h"

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

   shared::CDataContainer CUpdate::forPlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      if(parameters.size()>2)
      {
         std::string pluginName = parameters[2];
         std::string whatToDo = parameters[3];

         boost::shared_ptr<update::source::CPlugin> updateSource(new update::source::CPlugin());
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
