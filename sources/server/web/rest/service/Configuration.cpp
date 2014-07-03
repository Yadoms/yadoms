#include "stdafx.h"
#include "Configuration.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/Result.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include <shared/Log.h>

namespace web { namespace rest { namespace service {

   std::string CConfiguration::m_restKeyword= std::string("configuration");


   CConfiguration::CConfiguration(boost::shared_ptr<database::IDataProvider> dataProvider)
      :m_dataProvider(dataProvider)
   {

   }


   CConfiguration::~CConfiguration()
   {
   }


   void CConfiguration::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CConfiguration::getAllConfigurations);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CConfiguration::getSectionConfigurations);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("*"), CConfiguration::getConfiguration);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword), CConfiguration::createOneConfiguration);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("*"), CConfiguration::updateOneConfiguration);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword), CConfiguration::updateAllConfigurations);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("*")("*"), CConfiguration::deleteOneConfiguration);
   }

   const std::string & CConfiguration::getRestKeyword()
   {
      return m_restKeyword;
   }


   shared::CDataContainer CConfiguration::getConfiguration(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::string section = "";
      std::string keyname = "";
      if(parameters.size()>1)
         section = parameters[1];
      if(parameters.size()>2)
         keyname = parameters[2];

      boost::shared_ptr<database::entities::CConfiguration> configFound =  m_dataProvider->getConfigurationRequester()->getConfiguration(section, keyname);
      return web::rest::CResult::GenerateSuccess(configFound);
   }

   shared::CDataContainer CConfiguration::getSectionConfigurations(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::string section = "";
      if(parameters.size()>1)
         section = parameters[1];


      std::vector< boost::shared_ptr<database::entities::CConfiguration> > hwList = m_dataProvider->getConfigurationRequester()->getConfigurations(section);
      shared::CDataContainer collection;
      collection.set(getRestKeyword(), hwList);
      return web::rest::CResult::GenerateSuccess(collection);
   }

   shared::CDataContainer CConfiguration::getAllConfigurations(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CConfiguration> > hwList = m_dataProvider->getConfigurationRequester()->getConfigurations();
      shared::CDataContainer collection;
      collection.set(getRestKeyword(), hwList);
      return web::rest::CResult::GenerateSuccess(collection);
   }

   shared::CDataContainer CConfiguration::createOneConfiguration(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      //get data from request content
      database::entities::CConfiguration configToCreate;
      configToCreate.fillFromContent(requestContent);

      //check that configuration entry do not already exists
      boost::shared_ptr<database::entities::CConfiguration> checkExistEntity = m_dataProvider->getConfigurationRequester()->getConfiguration(configToCreate.Section(), configToCreate.Name());
      if(checkExistEntity.get() != NULL)
         return web::rest::CResult::GenerateError("The entry to create already exists");

      //update modification date
      configToCreate.LastModificationDate = boost::posix_time::second_clock::universal_time();

      //commit changes to database
      m_dataProvider->getConfigurationRequester()->create(configToCreate);

      boost::shared_ptr<database::entities::CConfiguration> widgetFound =  m_dataProvider->getConfigurationRequester()->getConfiguration(configToCreate.Section(), configToCreate.Name());
      return web::rest::CResult::GenerateSuccess(widgetFound);
   }


   shared::CDataContainer CConfiguration::updateOneConfiguration(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         database::entities::CConfiguration configToUpdate;
         configToUpdate.fillFromContent(requestContent);

         std::string section = "";
         std::string keyname = "";
         if(parameters.size()>1)
            section = parameters[1];
         if(parameters.size()>2)
            keyname = parameters[2];


         if((configToUpdate.Section().empty() && configToUpdate.Name().empty()) ||
            (boost::iequals(configToUpdate.Section(), section) &&  boost::iequals(configToUpdate.Name(), keyname)))
         {
            //ensure section and name are corretly filled
            configToUpdate.Section = section;
            configToUpdate.Name = keyname;
            //update modification date
            configToUpdate.LastModificationDate = boost::posix_time::second_clock::universal_time();
            //commit changes to database
            m_dataProvider->getConfigurationRequester()->updateConfiguration(configToUpdate);
            return web::rest::CResult::GenerateSuccess(getConfiguration(parameters, requestContent));
         }
         else
         {
            return web::rest::CResult::GenerateError("section and name in query content do not match to rest url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in updating a configuration value");
      }
   }




   shared::CDataContainer CConfiguration::updateAllConfigurations(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         std::vector<boost::shared_ptr<database::entities::CConfiguration> > listToUpdate = requestContent.get< std::vector<boost::shared_ptr<database::entities::CConfiguration> > >(getRestKeyword());

         BOOST_FOREACH(boost::shared_ptr<database::entities::CConfiguration> pw, listToUpdate)
         {
            m_dataProvider->getConfigurationRequester()->updateConfiguration(*pw);
         }

         return web::rest::CResult::GenerateSuccess();
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in updating all configuration");
      }
   }



   shared::CDataContainer CConfiguration::deleteOneConfiguration(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
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
         configToRemove.Section = section;
         configToRemove.Name = keyname;
         m_dataProvider->getConfigurationRequester()->removeConfiguration(configToRemove);
      }

      return shared::CDataContainer();
   }



} //namespace service
} //namespace rest
} //namespace web 
