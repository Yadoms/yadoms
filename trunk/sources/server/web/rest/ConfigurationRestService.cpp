#include "stdafx.h"
#include "ConfigurationRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "RestDispatcher.h"

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
}

const std::string & CConfigurationRestService::getRestKeyword()
{
   return m_restKeyword;
}


CJson CConfigurationRestService::getConfiguration(const std::vector<std::string> & parameters)
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

CJson CConfigurationRestService::getSectionConfigurations(const std::vector<std::string> & parameters)
{
   CConfigurationEntitySerializer hes;

   std::string section = "";
   if(parameters.size()>1)
      section = parameters[1];


   std::vector< boost::shared_ptr<CConfiguration> > hwList = m_dataProvider->getConfigurationRequester()->getConfigurations(section);
   return CJonCollectionSerializer<CConfiguration>::SerializeCollection(hwList, hes, getRestKeyword());
}

CJson CConfigurationRestService::getAllConfigurations(const std::vector<std::string> & parameters)
{
   CConfigurationEntitySerializer hes;
   std::vector< boost::shared_ptr<CConfiguration> > hwList = m_dataProvider->getConfigurationRequester()->getConfigurations();
   return CJonCollectionSerializer<CConfiguration>::SerializeCollection(hwList, hes, getRestKeyword());
}


