#include "stdafx.h"
#include "ConfigurationRestService.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonResult.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include <shared/Log.h>

namespace web { namespace rest { namespace service {

   std::string CConfigurationRestService::m_restKeyword= std::string("configuration");


   CConfigurationRestService::CConfigurationRestService(boost::shared_ptr<database::IDataProvider> dataProvider)
      :m_dataProvider(dataProvider)
   {

   }


   CConfigurationRestService::~CConfigurationRestService()
   {
   }


   void CConfigurationRestService::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CConfigurationRestService::getAllConfigurations);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CConfigurationRestService::getSectionConfigurations);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("*"), CConfigurationRestService::getConfiguration);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword), CConfigurationRestService::createOneConfiguration);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("*"), CConfigurationRestService::updateOneConfiguration);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword), CConfigurationRestService::updateAllConfigurations);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("*")("*"), CConfigurationRestService::deleteOneConfiguration);
   }

   const std::string & CConfigurationRestService::getRestKeyword()
   {
      return m_restKeyword;
   }


   web::rest::json::CJson CConfigurationRestService::getConfiguration(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      web::rest::json::CConfigurationEntitySerializer hes;

      std::string section = "";
      std::string keyname = "";
      if(parameters.size()>1)
         section = parameters[1];
      if(parameters.size()>2)
         keyname = parameters[2];

      boost::shared_ptr<database::entities::CConfiguration> widgetFound =  m_dataProvider->getConfigurationRequester()->getConfiguration(section, keyname);
      return web::rest::json::CJsonResult::GenerateSuccess(hes.serialize(*widgetFound.get()));
   }

   web::rest::json::CJson CConfigurationRestService::getSectionConfigurations(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      web::rest::json::CConfigurationEntitySerializer hes;

      std::string section = "";
      if(parameters.size()>1)
         section = parameters[1];


      std::vector< boost::shared_ptr<database::entities::CConfiguration> > hwList = m_dataProvider->getConfigurationRequester()->getConfigurations(section);
      return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CConfiguration>::SerializeCollection(hwList, hes, getRestKeyword()));
   }

   web::rest::json::CJson CConfigurationRestService::getAllConfigurations(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      web::rest::json::CConfigurationEntitySerializer hes;
      std::vector< boost::shared_ptr<database::entities::CConfiguration> > hwList = m_dataProvider->getConfigurationRequester()->getConfigurations();
      return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CConfiguration>::SerializeCollection(hwList, hes, getRestKeyword()));
   }

   web::rest::json::CJson CConfigurationRestService::createOneConfiguration(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      //get data from request content
      web::rest::json::CConfigurationEntitySerializer hes;
      boost::shared_ptr<database::entities::CConfiguration> configToCreate = hes.deserialize(requestContent);

      //check that configuration entry do not already exists
      boost::shared_ptr<database::entities::CConfiguration> checkExistEntity = m_dataProvider->getConfigurationRequester()->getConfiguration(configToCreate->getSection(), configToCreate->getName());
      if(checkExistEntity.get() != NULL)
         return web::rest::json::CJsonResult::GenerateError("The entry to create already exists", hes.serialize(*checkExistEntity.get()));

      //update modification date
      configToCreate->setLastModificationDate(boost::posix_time::second_clock::universal_time());

      //commit changes to database
      m_dataProvider->getConfigurationRequester()->create(*configToCreate.get());

      boost::shared_ptr<database::entities::CConfiguration> widgetFound =  m_dataProvider->getConfigurationRequester()->getConfiguration(configToCreate->getSection(), configToCreate->getName());
      return web::rest::json::CJsonResult::GenerateSuccess(hes.serialize(*widgetFound.get()));
   }


   web::rest::json::CJson CConfigurationRestService::updateOneConfiguration(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CConfigurationEntitySerializer hes;
         boost::shared_ptr<database::entities::CConfiguration> configToUpdate = hes.deserialize(requestContent);

         std::string section = "";
         std::string keyname = "";
         if(parameters.size()>1)
            section = parameters[1];
         if(parameters.size()>2)
            keyname = parameters[2];


         if((configToUpdate->getSection().empty() && configToUpdate->getName().empty()) ||
            (boost::iequals(configToUpdate->getSection(), section) &&  boost::iequals(configToUpdate->getName(), keyname)))
         {
            //ensure section and name are corretly filled
            configToUpdate->setSection(section);
            configToUpdate->setName(keyname);
            //update modification date
            configToUpdate->setLastModificationDate(boost::posix_time::second_clock::universal_time());
            //commit changes to database
            m_dataProvider->getConfigurationRequester()->updateConfiguration(*configToUpdate);
            return web::rest::json::CJsonResult::GenerateSuccess(getConfiguration(parameters, requestContent));
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("section and name in query content do not match to rest url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in updating a configuration value");
      }
   }




   web::rest::json::CJson CConfigurationRestService::updateAllConfigurations(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CConfigurationEntitySerializer hes;
         std::vector<boost::shared_ptr<database::entities::CConfiguration> > listToUpdate = web::rest::json::CJsonCollectionSerializer<database::entities::CConfiguration>::DeserializeCollection(requestContent, hes, getRestKeyword());

         BOOST_FOREACH(boost::shared_ptr<database::entities::CConfiguration> pw, listToUpdate)
         {
            m_dataProvider->getConfigurationRequester()->updateConfiguration(*pw);
         }

         return web::rest::json::CJsonResult::GenerateSuccess();
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in updating all configuration");
      }
   }



   web::rest::json::CJson CConfigurationRestService::deleteOneConfiguration(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      std::string section = "";
      std::string keyname = "";
      if(parameters.size()>1)
         section = parameters[1];
      if(parameters.size()>2)
         keyname = parameters[2];

      if(!section.empty() && !keyname.empty())
      {
         database::entities::CConfiguration configToRemove;
         configToRemove.setSection(section);
         configToRemove.setName(keyname);
         m_dataProvider->getConfigurationRequester()->removeConfiguration(configToRemove);
      }

      return web::rest::json::CJson();
   }



} //namespace service
} //namespace rest
} //namespace web 
