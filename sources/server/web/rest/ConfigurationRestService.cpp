#include "stdafx.h"
#include "ConfigurationRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonError.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "RestDispatcher.h"
#include <shared/Log.h>

CConfigurationRestService::CConfigurationRestService(boost::shared_ptr<IDataProvider> dataProvider)
   :m_dataProvider(dataProvider), m_restKeyword("configuration")
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
   REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("*")("*"), CConfigurationRestService::updateOneConfiguration);
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
   return hes.serialize(*widgetFound.get());
}

CJson CConfigurationRestService::getSectionConfigurations(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CConfigurationEntitySerializer hes;

   std::string section = "";
   if(parameters.size()>1)
      section = parameters[1];


   std::vector< boost::shared_ptr<CConfiguration> > hwList = m_dataProvider->getConfigurationRequester()->getConfigurations(section);
   return CJonCollectionSerializer<CConfiguration>::SerializeCollection(hwList, hes, getRestKeyword());
}

CJson CConfigurationRestService::getAllConfigurations(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CConfigurationEntitySerializer hes;
   std::vector< boost::shared_ptr<CConfiguration> > hwList = m_dataProvider->getConfigurationRequester()->getConfigurations();
   return CJonCollectionSerializer<CConfiguration>::SerializeCollection(hwList, hes, getRestKeyword());
}


CJson CConfigurationRestService::updateOneConfiguration(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CConfigurationEntitySerializer hes;
   CConfiguration configToUpdate = hes.deserialize(requestContent);

   std::string section = "";
   std::string keyname = "";
   if(parameters.size()>1)
      section = parameters[1];
   if(parameters.size()>2)
      keyname = parameters[2];

   
   if((configToUpdate.getSection().empty() && configToUpdate.getName().empty()) ||
      (boost::iequals(configToUpdate.getSection(), section) &&  boost::iequals(configToUpdate.getName(), keyname)))
   {
      //ensure section and name are corretly filled
      configToUpdate.setSection(section);
      configToUpdate.setName(keyname);
      //update modification date
      configToUpdate.setLastModificationDate(boost::posix_time::second_clock::local_time());
      //commit changes to database
      m_dataProvider->getConfigurationRequester()->updateConfiguration(configToUpdate);
      return getConfiguration(parameters, requestContent);
   }
   else
   {
      return CJsonError::GenerateError("section and name in query content do not match to rest url");
   }
}

CJson CConfigurationRestService::deleteOneConfiguration(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   return CJson();
}
