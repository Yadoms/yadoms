#include "stdafx.h"
#include "ConfigurationRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonResult.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "RestDispatcher.h"
#include <shared/Log.h>

std::string CConfigurationRestService::m_restKeyword= std::string("configuration");


CConfigurationRestService::CConfigurationRestService(boost::shared_ptr<IDataProvider> dataProvider)
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


CJson CConfigurationRestService::getConfiguration(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CConfigurationEntitySerializer hes;

   std::string section = "";
   std::string keyname = "";
   if(parameters.size()>1)
      section = parameters[1];
   if(parameters.size()>2)
      keyname = parameters[2];

   boost::shared_ptr<CConfiguration> widgetFound =  m_dataProvider->getConfigurationRequester()->getConfiguration(section, keyname);
   return CJsonResult::GenerateSuccess(hes.serialize(*widgetFound.get()));
}

CJson CConfigurationRestService::getSectionConfigurations(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CConfigurationEntitySerializer hes;

   std::string section = "";
   if(parameters.size()>1)
      section = parameters[1];


   std::vector< boost::shared_ptr<CConfiguration> > hwList = m_dataProvider->getConfigurationRequester()->getConfigurations(section);
   return CJsonResult::GenerateSuccess(CJsonCollectionSerializer<CConfiguration>::SerializeCollection(hwList, hes, getRestKeyword()));
}

CJson CConfigurationRestService::getAllConfigurations(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CConfigurationEntitySerializer hes;
   std::vector< boost::shared_ptr<CConfiguration> > hwList = m_dataProvider->getConfigurationRequester()->getConfigurations();
   return CJsonResult::GenerateSuccess(CJsonCollectionSerializer<CConfiguration>::SerializeCollection(hwList, hes, getRestKeyword()));
}

CJson CConfigurationRestService::createOneConfiguration(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   //get data from request content
   CConfigurationEntitySerializer hes;
   boost::shared_ptr<CConfiguration> configToCreate = hes.deserialize(requestContent);

   //check that configuration entry do not already exists
   boost::shared_ptr<CConfiguration> checkExistEntity = m_dataProvider->getConfigurationRequester()->getConfiguration(configToCreate->getSection(), configToCreate->getName());
   if(checkExistEntity.get() != NULL)
      return CJsonResult::GenerateError("The entry to create already exists", hes.serialize(*checkExistEntity.get()));

   //update modification date
   configToCreate->setLastModificationDate(boost::posix_time::second_clock::universal_time());

   //commit changes to database
   m_dataProvider->getConfigurationRequester()->create(*configToCreate.get());

   boost::shared_ptr<CConfiguration> widgetFound =  m_dataProvider->getConfigurationRequester()->getConfiguration(configToCreate->getSection(), configToCreate->getName());
   return CJsonResult::GenerateSuccess(hes.serialize(*widgetFound.get()));
}


CJson CConfigurationRestService::updateOneConfiguration(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   try
   {
      CConfigurationEntitySerializer hes;
      boost::shared_ptr<CConfiguration> configToUpdate = hes.deserialize(requestContent);

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
         return CJsonResult::GenerateSuccess(getConfiguration(parameters, requestContent));
      }
      else
      {
         return CJsonResult::GenerateError("section and name in query content do not match to rest url");
      }
   }
   catch(std::exception &ex)
   {
      return CJsonResult::GenerateError(ex);
   }
   catch(...)
   {
      return CJsonResult::GenerateError("unknown exception in updating a configuration value");
   }
}




CJson CConfigurationRestService::updateAllConfigurations(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   try
   {
      CConfigurationEntitySerializer hes;
      std::vector<boost::shared_ptr<CConfiguration> > listToUpdate = CJsonCollectionSerializer<CConfiguration>::DeserializeCollection(requestContent, hes, getRestKeyword());

      BOOST_FOREACH(boost::shared_ptr<CConfiguration> pw, listToUpdate)
      {
         m_dataProvider->getConfigurationRequester()->updateConfiguration(*pw);
      }

      return CJsonResult::GenerateSuccess();
   }
   catch(std::exception &ex)
   {
      return CJsonResult::GenerateError(ex);
   }
   catch(...)
   {
      return CJsonResult::GenerateError("unknown exception in updating all configuration");
   }
}



CJson CConfigurationRestService::deleteOneConfiguration(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   std::string section = "";
   std::string keyname = "";
   if(parameters.size()>1)
      section = parameters[1];
   if(parameters.size()>2)
      keyname = parameters[2];

   if(!section.empty() && !keyname.empty())
   {
      CConfiguration configToRemove;
      configToRemove.setSection(section);
      configToRemove.setName(keyname);
      m_dataProvider->getConfigurationRequester()->removeConfiguration(configToRemove);
   }

   return CJson();
}

CJson CConfigurationRestService::deleteAllConfigurations(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   std::string section = "";
   std::string keyname = "";
   if(parameters.size()>1)
      section = parameters[1];
   if(parameters.size()>2)
      keyname = parameters[2];

   //m_dataProvider->getConfigurationRequester()->removeConfiguration();
   return CJsonResult::GenerateError();
}
