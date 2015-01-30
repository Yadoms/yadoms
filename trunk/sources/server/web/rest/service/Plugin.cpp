#include "stdafx.h"
#include "Plugin.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "shared/Log.h"
#include "web/rest/Result.h"
#include "pluginSystem/ManuallyDeviceCreationData.h"
#include "pluginSystem/BindingQueryData.h"
#include "communication/callback/CallbackRequest.h"
#include "communication/callback/SynchronousCallback.h"

namespace web { namespace rest { namespace service {

   CPlugin::CPlugin(boost::shared_ptr<database::IDataProvider> dataProvider, boost::shared_ptr<pluginSystem::CManager> pluginManager, communication::ISendMessageAsync & messageSender)
      :m_dataProvider(dataProvider), m_pluginManager(pluginManager), m_restKeyword("plugin"), m_messageSender(messageSender)
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
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("instance")("handleManuallyDeviceCreation"), CPlugin::getAllPluginsInstanceForManualDeviceCreation);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CPlugin::getOnePlugin);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("status"), CPlugin::getInstanceStatus);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("devices"), CPlugin::getPluginDevices);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("binding")("*"), CPlugin::getBinding);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("start"), CPlugin::startInstance);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("stop"), CPlugin::stopInstance);

      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CPlugin::createPlugin, CPlugin::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("createDevice"), CPlugin::createDevice, CPlugin::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CPlugin::updatePlugin, CPlugin::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CPlugin::deleteAllPlugins, CPlugin::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CPlugin::deletePlugin, CPlugin::transactionalMethod);
   }


   shared::CDataContainer CPlugin::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const std::string & requestContent)
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
         result = CResult::GenerateError(ex);
      }
      catch(...)
      {
         result = CResult::GenerateError("unknown exception plugin rest method");
      }

      if(pTransactionalEngine)
      {
         if(CResult::isSuccess(result))
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

            boost::shared_ptr<database::entities::CPlugin> pluginFound = m_pluginManager->getInstance(instanceId);

            return CResult::GenerateSuccess(pluginFound);
         }
         
         return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in retreiving one plugin instance");
      }
   }

   shared::CDataContainer CPlugin::getAllPluginsInstance(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CPlugin> > hwList = m_pluginManager->getInstanceList();
      shared::CDataContainer t;
      t.set(getRestKeyword(), hwList);
      return CResult::GenerateSuccess(t);
   }

   shared::CDataContainer CPlugin::getAllPluginsInstanceForManualDeviceCreation(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CPlugin> > result;

      //liste de toutes les instances
      std::vector< boost::shared_ptr<database::entities::CPlugin> > hwList = m_pluginManager->getInstanceList();
      
      //search for manuallyCreatedDevice
      pluginSystem::CManager::AvalaiblePluginMap pluginList = m_pluginManager->getPluginList();

      
      for (std::vector< boost::shared_ptr<database::entities::CPlugin> >::iterator currentInstance = hwList.begin(); currentInstance != hwList.end(); ++currentInstance)
      {
         if (m_pluginManager->isInstanceRunning(currentInstance->get()->Id))
         {
            pluginSystem::CManager::AvalaiblePluginMap::iterator matchingInfo = pluginList.find((*currentInstance)->Type);
            if (matchingInfo != pluginList.end())
            {
               if (matchingInfo->second->getSupportManuallyCreatedDevice())
                  result.push_back(*currentInstance);
            }
         }
      }

      //send result
      shared::CDataContainer t;
      t.set(getRestKeyword(), result);
      return CResult::GenerateSuccess(t);


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
            thisPluginData.set("supportManuallyCreatedDevice", i->second->getSupportManuallyCreatedDevice());
            
            pluginCollection.push_back(thisPluginData);
         }

         result.set< std::vector<shared::CDataContainer> >("plugins", pluginCollection);
         return CResult::GenerateSuccess(result);
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in creating a new plugin instance");
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
         return CResult::GenerateSuccess(pluginFound);
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in creating a new plugin instance");
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
         return CResult::GenerateSuccess(pluginFound);
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in updating a plugin instance");
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
            m_pluginManager->deleteInstance(m_pluginManager->getInstance(instanceId));
            return CResult::GenerateSuccess();
         }
         
         return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in deleting one plugin instance");
      }
   }

   shared::CDataContainer CPlugin::deleteAllPlugins(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CPlugin> > hwList = m_pluginManager->getInstanceList();
      
      for (std::vector<boost::shared_ptr<database::entities::CPlugin> >::iterator i = hwList.begin(); i != hwList.end(); ++i)
      {
         m_pluginManager->deleteInstance(*i);
      }

      return CResult::GenerateSuccess();
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
               return CResult::GenerateSuccess(result);
            }
            
            return CResult::GenerateError("invalid parameter. Can not retreive instance id");
         }
         
         return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in reading plugin instance status");
      }
   }

   shared::CDataContainer CPlugin::getPluginDevices(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if (parameters.size()>1)
         {
            int instanceId = boost::lexical_cast<int>(parameters[1]);

            
            std::vector<boost::shared_ptr<database::entities::CDevice> > devicesFound = m_dataProvider->getDeviceRequester()->getDevices(instanceId);
            //send result
            shared::CDataContainer t;
            t.set("devices", devicesFound);
            return CResult::GenerateSuccess(t);
         }

         return CResult::GenerateError("invalid parameter. Can not retreive pluigin instance id in url");
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in reading plugin instance devices");
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
                  return CResult::GenerateSuccess();
               return CResult::GenerateError("Fail to start the plugin instance");
            }
            
            return CResult::GenerateError("invalid parameter. Can not retreive instance id");
         }
         
         return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in starting plugin instance");
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
                  return CResult::GenerateSuccess();
               return CResult::GenerateError("Fail to stop the plugin instance");
            }
            
            return CResult::GenerateError("invalid parameter. Can not retreive instance id");
         }
         
         return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in stopping plugin instance");
      }
   }

   shared::CDataContainer CPlugin::createDevice(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if (parameters.size() >= 2)
         {
            int pluginId = boost::lexical_cast<int>(parameters[1]);

            if (!requestContent.hasValue("name") || !requestContent.hasValue("configuration"))
               return CResult::GenerateError("invalid request content. There must be a name and a configuration field");

            try
            {
               //create a callback (allow waiting for result)              
               communication::callback::CSynchronousCallback<std::string> cb;

               //create the data container to send to plugin
               pluginSystem::CManuallyDeviceCreationData data(requestContent.get<std::string>("name"), requestContent.get<shared::CDataContainer>("configuration"));

               //send request to plugin
               m_messageSender.sendManuallyDeviceCreationRequest(pluginId, data, cb);

               //wait for result
               //communication::callback::CSynchronousCallback< std::string >::CSynchronousResult res = cb.waitForResult();
               switch (cb.waitForResult())
               {
               case communication::callback::CSynchronousCallback< std::string >::kResult:
                  {
                     communication::callback::CSynchronousCallback< std::string >::CSynchronousResult res = cb.getCallbackResult();

                     if (res.Success)
                     {
                        //find created device
                           boost::shared_ptr<database::entities::CDevice> createdDevice = m_dataProvider->getDeviceRequester()->getDevice(pluginId, res.Result);

                        //update friendly name
                        m_dataProvider->getDeviceRequester()->updateDeviceFriendlyName(createdDevice->Id(), requestContent.get<std::string>("name"));

                        //get device with friendly name updated
                        createdDevice = m_dataProvider->getDeviceRequester()->getDevice(pluginId, res.Result);

                        return CResult::GenerateSuccess(createdDevice);
                     }
                     
                     //the plugin failed to create the device
                     return CResult::GenerateError(res.ErrorMessage);
                  }

               case shared::event::kTimeout :
                  return CResult::GenerateError("The plugin did not respond");
                     
               default:
                  return CResult::GenerateError("Unkown plugin result");
               }
            }
            catch (shared::exception::CException & ex)
            {
               return CResult::GenerateError(ex);
            }
         }
         
         return CResult::GenerateError("invalid parameter. Can not retreive keyword id in url");
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in retreiving keyword");
      }
   }

   shared::CDataContainer CPlugin::getBinding(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
	{
		try
		{
		   if (parameters.size() >= 3)
			{
				int pluginId = boost::lexical_cast<int>(parameters[1]);

				std::string query = parameters[3];
				try
				{
					//create a callback (allow waiting for result)              
               communication::callback::CSynchronousCallback< shared::CDataContainer > cb;

					//create the data container to send to plugin
					pluginSystem::CBindingQueryData data(query);

					//send request to plugin
					m_messageSender.sendBindingQueryRequest(pluginId, data, cb);

					//wait for result
					//communication::callback::CSynchronousCallback< std::string >::CSynchronousResult res = cb.waitForResult();
					switch (cb.waitForResult())
					{
               case communication::callback::CSynchronousCallback< shared::CDataContainer >::kResult:
					{
                  communication::callback::CSynchronousCallback< shared::CDataContainer >::CSynchronousResult res = cb.getCallbackResult();

						if (res.Success)
                     return CResult::GenerateSuccess(res.Result);
		      
                  //the plugin failed to create the device
                  return CResult::GenerateError(res.ErrorMessage);
               }

					case shared::event::kTimeout:
						return CResult::GenerateError("The plugin did not respond");

					default:
						return CResult::GenerateError("Unkown plugin result");
					}
				}
				catch (shared::exception::CException & ex)
				{
					return CResult::GenerateError(ex);
				}
				
			}
		   
         return CResult::GenerateError("invalid parameter. Can not retreive plugin id in url");
		}
		catch (std::exception &ex)
		{
			return CResult::GenerateError(ex);
		}
		catch (...)
		{
			return CResult::GenerateError("unknown exception in executing binding query");
		}
	}

} //namespace service
} //namespace rest
} //namespace web 
