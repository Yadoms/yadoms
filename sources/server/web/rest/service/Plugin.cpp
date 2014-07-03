#include "stdafx.h"
#include "Plugin.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "shared/Log.h"
#include "web/rest/Result.h"

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


   shared::CDataContainer CPlugin::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      boost::shared_ptr<database::ITransactionalProvider> pTransactionalEngine = m_dataProvider->getTransactionalEngine();
      shared::CDataContainer result;
      try
      {
         if(pTransactionalEngine)
            pTransactionalEngine->transactionBegin();
         result = realMethod(parameters, requestContent);
      }
      catch(std::exception &ex)
      {
         result = web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         result = web::rest::CResult::GenerateError("unknown exception plugin rest method");
      }

      if(pTransactionalEngine)
      {
         if(web::rest::CResult::isSuccess(result))
            pTransactionalEngine->transactionCommit();
         else
            pTransactionalEngine->transactionRollback();
      }
      return result;
   }


   shared::CDataContainer CPlugin::getOnePlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int instanceId = boost::lexical_cast<int>(parameters[1]);

            //web::rest::json::CPluginEntitySerializer hes;
            boost::shared_ptr<database::entities::CPlugin> pluginFound = m_pluginManager->getInstance(instanceId);

            return web::rest::CResult::GenerateSuccess(pluginFound);
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in retreiving one plugin instance");
      }
   }

   shared::CDataContainer CPlugin::getAllPluginsInstance(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CPlugin> > hwList = m_pluginManager->getInstanceList();
      shared::CDataContainer t;
      t.set(getRestKeyword(), hwList);
      return web::rest::CResult::GenerateSuccess(t);
   }

   shared::CDataContainer CPlugin::getAllAvailablePlugins(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         pluginSystem::CManager::AvalaiblePluginMap pluginList = m_pluginManager->getPluginList();

         pluginSystem::CManager::AvalaiblePluginMap::iterator i;
         shared::CDataContainer result;
         std::vector<shared::CDataContainer> pluginCollection;
         for(i=pluginList.begin(); i!=pluginList.end(); ++i)
         {
            shared::CDataContainer thisPluginData;
            thisPluginData.set("name", i->first);
            thisPluginData.set("author", i->second->getAuthor());
            thisPluginData.set("description", i->second->getDescription());
            thisPluginData.set("nameInformation", i->second->getName());
            thisPluginData.set("identity", i->second->getIdentity());
            thisPluginData.set("releaseType", i->second->getReleaseType());
            thisPluginData.set("url", i->second->getUrl());
            thisPluginData.set("version", i->second->getVersion());

            pluginCollection.push_back(thisPluginData);
         }

         result.set< std::vector<shared::CDataContainer> >("plugins", pluginCollection);
         return web::rest::CResult::GenerateSuccess(result);
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in creating a new plugin instance");
      }
   }

   shared::CDataContainer CPlugin::createPlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         database::entities::CPlugin p;
         p.fillFromContent(requestContent);
         int idCreated = m_pluginManager->createInstance(p);

         boost::shared_ptr<database::entities::CPlugin> pluginFound = m_pluginManager->getInstance(idCreated);
         return web::rest::CResult::GenerateSuccess(pluginFound);
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in creating a new plugin instance");
      }
   }

   shared::CDataContainer CPlugin::updatePlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         database::entities::CPlugin instanceToUpdate;
         instanceToUpdate.fillFromContent(requestContent);

         m_pluginManager->updateInstance(instanceToUpdate);

         boost::shared_ptr<database::entities::CPlugin> pluginFound = m_pluginManager->getInstance(instanceToUpdate.Id());
         return web::rest::CResult::GenerateSuccess(pluginFound);
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in updating a plugin instance");
      }
   }


   shared::CDataContainer CPlugin::deletePlugin(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int instanceId = boost::lexical_cast<int>(parameters[1]);

            m_dataProvider->getDeviceRequester()->removeAllDeviceForPlugin(instanceId);
            m_pluginManager->deleteInstance(instanceId);
            return web::rest::CResult::GenerateSuccess();
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in deleting one plugin instance");
      }
   }

   shared::CDataContainer CPlugin::deleteAllPlugins(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CPlugin> > hwList = m_pluginManager->getInstanceList();
      BOOST_FOREACH(boost::shared_ptr<database::entities::CPlugin> toDelete, hwList)
      {
         m_pluginManager->deleteInstance(toDelete->Id());
      }

      return web::rest::CResult::GenerateSuccess();
   }

   shared::CDataContainer CPlugin::getInstanceStatus(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)  
   {
      try
      {
         if(parameters.size()>1)
         {
            int instanceId = boost::lexical_cast<int>(parameters[1]);

            boost::shared_ptr<database::entities::CPlugin> pluginInstanceFound = m_pluginManager->getInstance(instanceId);
            if(pluginInstanceFound)
            {
               shared::CDataContainer result;
               result.set("running", m_pluginManager->isInstanceRunning(instanceId));
               return web::rest::CResult::GenerateSuccess(result);
            }
            else
            {
               return web::rest::CResult::GenerateError("invalid parameter. Can not retreive instance id");
            }
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in reading plugin instance status");
      }
   }

   shared::CDataContainer CPlugin::startInstance(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
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
                  return web::rest::CResult::GenerateSuccess();
               return web::rest::CResult::GenerateError("Fail to start the plugin instance");
            }
            else
            {
               return web::rest::CResult::GenerateError("invalid parameter. Can not retreive instance id");
            }
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in starting plugin instance");
      }
   }

   shared::CDataContainer CPlugin::stopInstance(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
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
                  return web::rest::CResult::GenerateSuccess();
               return web::rest::CResult::GenerateError("Fail to stop the plugin instance");
            }
            else
            {
               return web::rest::CResult::GenerateError("invalid parameter. Can not retreive instance id");
            }
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in stopping plugin instance");
      }
   }



} //namespace service
} //namespace rest
} //namespace web 
