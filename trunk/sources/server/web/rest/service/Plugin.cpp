#include "stdafx.h"
#include "Plugin.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "shared/Log.h"
#include "web/rest/json/JsonResult.h"

namespace web { namespace rest { namespace service {

   CPlugin::CPlugin(boost::shared_ptr<database::IDataProvider> dataProvider, boost::shared_ptr<pluginSystem::CManager> pluginManager)
      :m_dataProvider(dataProvider), m_pluginManager(pluginManager), m_restKeyword("plugin")
   {
   }


   CPlugin::~CPlugin()
   {
   }

   const std::string & CPlugin::getRestKeyword()
   {
      return m_restKeyword;
   }

   void CPlugin::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CPlugin::getAllAvailablePlugins);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("instance"), CPlugin::getAllPluginsInstance);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CPlugin::getOnePlugin);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("status"), CPlugin::getInstanceStatus);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT",  (m_restKeyword)("*")("start"), CPlugin::startInstance);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT",  (m_restKeyword)("*")("stop"), CPlugin::stopInstance);

      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CPlugin::createPlugin, CPlugin::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CPlugin::updatePlugin, CPlugin::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CPlugin::deleteAllPlugins, CPlugin::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CPlugin::deletePlugin, CPlugin::transactionalMethod);
   }


   web::rest::json::CJson CPlugin::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
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


   web::rest::json::CJson CPlugin::getOnePlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
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

   web::rest::json::CJson CPlugin::getAllPluginsInstance(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      web::rest::json::CPluginEntitySerializer hes;
      std::vector< boost::shared_ptr<database::entities::CPlugin> > hwList = m_pluginManager->getInstanceList();
      return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CPlugin>::SerializeCollection(hwList, hes, getRestKeyword()));
   }

   web::rest::json::CJson CPlugin::getAllAvailablePlugins(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CPluginEntitySerializer hes;
         pluginSystem::CManager::AvalaiblePluginMap pluginList = m_pluginManager->getPluginList();

         pluginSystem::CManager::AvalaiblePluginMap::iterator i;
         web::rest::json::CJson result;
         web::rest::json::CJson pluginCollection;
         for(i=pluginList.begin(); i!=pluginList.end(); ++i)
         {
            web::rest::json::CJson thisPluginData;
            thisPluginData.put("name", i->first);
            thisPluginData.put("author", i->second->getAuthor());
            thisPluginData.put("description", i->second->getDescription());
            thisPluginData.put("nameInformation", i->second->getName());
            thisPluginData.put("identity", i->second->getIdentity());
            thisPluginData.put("releaseType", i->second->getReleaseType());
            thisPluginData.put("url", i->second->getUrl());
            thisPluginData.put("version", i->second->getVersion());

            pluginCollection.push_back(std::make_pair("", thisPluginData));
         }

         result.add_child("plugins", pluginCollection);
         return web::rest::json::CJsonResult::GenerateSuccess(result);
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

   web::rest::json::CJson CPlugin::createPlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
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

   web::rest::json::CJson CPlugin::updatePlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
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


   web::rest::json::CJson CPlugin::deletePlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
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

   web::rest::json::CJson CPlugin::deleteAllPlugins(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CPlugin> > hwList = m_pluginManager->getInstanceList();
      BOOST_FOREACH(boost::shared_ptr<database::entities::CPlugin> toDelete, hwList)
      {
         m_pluginManager->deleteInstance(toDelete->Id());
      }

      return web::rest::json::CJsonResult::GenerateSuccess();
   }

   web::rest::json::CJson CPlugin::getInstanceStatus(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)  
   {
      try
      {
         if(parameters.size()>1)
         {
            int instanceId = boost::lexical_cast<int>(parameters[1]);

            boost::shared_ptr<database::entities::CPlugin> pluginInstanceFound = m_pluginManager->getInstance(instanceId);
            if(pluginInstanceFound)
            {
               web::rest::json::CJson result;
               result.put("running", m_pluginManager->isInstanceRunning(instanceId));
               return web::rest::json::CJsonResult::GenerateSuccess(result);
            }
            else
            {
               return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive instance id");
            }
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
         return web::rest::json::CJsonResult::GenerateError("unknown exception in reading plugin instance status");
      }
   }

   web::rest::json::CJson CPlugin::startInstance(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int instanceId = boost::lexical_cast<int>(parameters[1]);
            
            boost::shared_ptr<database::entities::CPlugin> pluginInstanceFound = m_pluginManager->getInstance(instanceId);
            if(pluginInstanceFound)
            {
               //start instance
               m_pluginManager->startInstance(instanceId);
               //check for instance status
               bool state = m_pluginManager->isInstanceRunning(instanceId);
               if(state)
                  return web::rest::json::CJsonResult::GenerateSuccess();
               return web::rest::json::CJsonResult::GenerateError("Fail to start the plugin instance");
            }
            else
            {
               return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive instance id");
            }
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
         return web::rest::json::CJsonResult::GenerateError("unknown exception in starting plugin instance");
      }
   }

   web::rest::json::CJson CPlugin::stopInstance(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int instanceId = boost::lexical_cast<int>(parameters[1]);
            
            boost::shared_ptr<database::entities::CPlugin> pluginInstanceFound = m_pluginManager->getInstance(instanceId);
            if(pluginInstanceFound)
            {
               //stop instance
               m_pluginManager->stopInstance(instanceId);
               //check for instance status
               bool state = m_pluginManager->isInstanceRunning(instanceId);
               if(!state)
                  return web::rest::json::CJsonResult::GenerateSuccess();
               return web::rest::json::CJsonResult::GenerateError("Fail to stop the plugin instance");
            }
            else
            {
               return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive instance id");
            }
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
         return web::rest::json::CJsonResult::GenerateError("unknown exception in stopping plugin instance");
      }
   }



} //namespace service
} //namespace rest
} //namespace web 
