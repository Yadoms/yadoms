#include "stdafx.h"
#include "PluginRestService.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "shared/Log.h"
#include "web/rest/json/JsonResult.h"

namespace web { namespace rest { namespace service {

   CPluginRestService::CPluginRestService(boost::shared_ptr<database::IDataProvider> dataProvider, boost::shared_ptr<pluginSystem::CManager> pluginManager)
      :m_dataProvider(dataProvider), m_pluginManager(pluginManager), m_restKeyword("plugin")
   {
   }


   CPluginRestService::~CPluginRestService()
   {
   }

   const std::string & CPluginRestService::getRestKeyword()
   {
      return m_restKeyword;
   }

   void CPluginRestService::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CPluginRestService::getAllPlugins);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CPluginRestService::getOnePlugin);

      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CPluginRestService::createPlugin, CPluginRestService::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CPluginRestService::updatePlugin, CPluginRestService::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CPluginRestService::deleteAllPlugins, CPluginRestService::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CPluginRestService::deletePlugin, CPluginRestService::transactionalMethod);
   }


   web::rest::json::CJson CPluginRestService::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      boost::shared_ptr<database::ITransactionalProvider> pTransactionalEngine = m_dataProvider->getTransactionalEngine();
      web::rest::json::CJson result;
      try
      {
         if(pTransactionalEngine)
            pTransactionalEngine->transactionBegin();
         result = realMethod(parameters, requestContent);
      }
      catch(std::exception &ex)
      {
         result = web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         result = web::rest::json::CJsonResult::GenerateError("unknown exception plugin rest method");
      }

      if(pTransactionalEngine)
      {
         if(web::rest::json::CJsonResult::isSuccess(result))
            pTransactionalEngine->transactionCommit();
         else
            pTransactionalEngine->transactionRollback();
      }
      return result;
   }


   web::rest::json::CJson CPluginRestService::getOnePlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int instanceId = boost::lexical_cast<int>(parameters[1]);

            web::rest::json::CPluginEntitySerializer hes;
            boost::shared_ptr<database::entities::CPlugin> pluginFound = m_pluginManager->getInstance(instanceId);
            return web::rest::json::CJsonResult::GenerateSuccess(hes.serialize(*pluginFound.get()));
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive instance id in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in retreiving one plugin instance");
      }
   }

   web::rest::json::CJson CPluginRestService::getAllPlugins(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      web::rest::json::CPluginEntitySerializer hes;
      std::vector< boost::shared_ptr<database::entities::CPlugin> > hwList = m_pluginManager->getInstanceList();
      return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CPlugin>::SerializeCollection(hwList, hes, getRestKeyword()));
   }

   web::rest::json::CJson CPluginRestService::createPlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CPluginEntitySerializer hes;
         boost::shared_ptr<database::entities::CPlugin> instanceToAdd = hes.deserialize(requestContent);
         int idCreated = m_pluginManager->createInstance(*instanceToAdd);

         boost::shared_ptr<database::entities::CPlugin> pluginFound = m_pluginManager->getInstance(idCreated);
         return web::rest::json::CJsonResult::GenerateSuccess(hes.serialize(*pluginFound.get()));
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in creating a new plugin instance");
      }
   }

   web::rest::json::CJson CPluginRestService::updatePlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CPluginEntitySerializer hes;
         boost::shared_ptr<database::entities::CPlugin> instanceToUpdate = hes.deserialize(requestContent);
         m_pluginManager->updateInstance(*instanceToUpdate);

         boost::shared_ptr<database::entities::CPlugin> pluginFound = m_pluginManager->getInstance(instanceToUpdate->Id());
         return web::rest::json::CJsonResult::GenerateSuccess(hes.serialize(*pluginFound.get()));
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in updating a plugin instance");
      }
   }


   web::rest::json::CJson CPluginRestService::deletePlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int instanceId = boost::lexical_cast<int>(parameters[1]);
            m_pluginManager->deleteInstance(instanceId);
            return web::rest::json::CJsonResult::GenerateSuccess();
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive instance id in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in deleting one plugin instance");
      }
   }

   web::rest::json::CJson CPluginRestService::deleteAllPlugins(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CPlugin> > hwList = m_pluginManager->getInstanceList();
      BOOST_FOREACH(boost::shared_ptr<database::entities::CPlugin> toDelete, hwList)
      {
         m_pluginManager->deleteInstance(toDelete->Id());
      }

      return web::rest::json::CJsonResult::GenerateSuccess();
   }



} //namespace service
} //namespace rest
} //namespace web 
