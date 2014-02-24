#include "stdafx.h"
#include "PluginEventLoggerRestService.h"
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "shared/Log.h"


CPluginEventLoggerRestService::CPluginEventLoggerRestService(boost::shared_ptr<database::IDataProvider> dataProvider)
   :m_dataProvider(dataProvider), m_restKeyword("pluginEventLogger")
{
}


CPluginEventLoggerRestService::~CPluginEventLoggerRestService()
{
}

const std::string & CPluginEventLoggerRestService::getRestKeyword()
{
   return m_restKeyword;
}

void CPluginEventLoggerRestService::configureDispatcher(CRestDispatcher & dispatcher)
{
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("*")("*"), CPluginEventLoggerRestService::getLogsForPluginName);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("*")("*")("*"), CPluginEventLoggerRestService::getLogsForPluginNameFromDate);
}

CJson CPluginEventLoggerRestService::getLogsForPluginName(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   std::string pluginName = "";
   std::string pluginVersion = "";
   shared::plugin::information::EReleaseType rType = shared::plugin::information::kStable;

   if(parameters.size()>1)
      pluginName = parameters[1];
   if(parameters.size()>2)
      pluginVersion = parameters[2];
   if(parameters.size()>3)
      rType = (shared::plugin::information::EReleaseType)atoi(parameters[3].c_str());

   CPluginEventLoggerEntitySerializer hes;
   std::vector< boost::shared_ptr<database::entities::CPluginEventLogger> > dvList = m_dataProvider->getPluginEventLoggerRequester()->getPluginEvents(pluginName, pluginVersion, rType);
   return CJsonCollectionSerializer<database::entities::CPluginEventLogger>::SerializeCollection(dvList, hes, getRestKeyword());
}

CJson CPluginEventLoggerRestService::getLogsForPluginNameFromDate(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   std::string pluginName = "";
   std::string pluginVersion = "";
   boost::posix_time::ptime fromDate;
   shared::plugin::information::EReleaseType rType = shared::plugin::information::kStable;

   if(parameters.size()>1)
      pluginName = parameters[1];
   if(parameters.size()>2)
      pluginVersion = parameters[2];
   if(parameters.size()>3)
      rType = (shared::plugin::information::EReleaseType)atoi(parameters[3].c_str());
   if(parameters.size()>4)
      fromDate = boost::posix_time::time_from_string(parameters[4]);

   CPluginEventLoggerEntitySerializer hes;
   std::vector< boost::shared_ptr<database::entities::CPluginEventLogger> > dvList = m_dataProvider->getPluginEventLoggerRequester()->getPluginEvents(pluginName, pluginVersion, rType, fromDate);
   return CJsonCollectionSerializer<database::entities::CPluginEventLogger>::SerializeCollection(dvList, hes, getRestKeyword());
}
 