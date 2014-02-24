#include "stdafx.h"
#include "HardwareEventLoggerRestService.h"
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "shared/Log.h"


CHardwareEventLoggerRestService::CHardwareEventLoggerRestService(boost::shared_ptr<database::IDataProvider> dataProvider)
   :m_dataProvider(dataProvider), m_restKeyword("hardwareEventLogger")
{
}


CHardwareEventLoggerRestService::~CHardwareEventLoggerRestService()
{
}

const std::string & CHardwareEventLoggerRestService::getRestKeyword()
{
   return m_restKeyword;
}

void CHardwareEventLoggerRestService::configureDispatcher(CRestDispatcher & dispatcher)
{
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("*")("*"), CHardwareEventLoggerRestService::getLogsForPluginName);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("*")("*")("*"), CHardwareEventLoggerRestService::getLogsForPluginNameFromDate);
}

CJson CHardwareEventLoggerRestService::getLogsForPluginName(const std::vector<std::string> & parameters, const CJson & requestContent)
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

   CHardwareEventLoggerEntitySerializer hes;
   std::vector< boost::shared_ptr<database::entities::CHardwareEventLogger> > dvList = m_dataProvider->getHardwareEventLoggerRequester()->getHardwareEvents(pluginName, pluginVersion, rType);
   return CJsonCollectionSerializer<database::entities::CHardwareEventLogger>::SerializeCollection(dvList, hes, getRestKeyword());
}

CJson CHardwareEventLoggerRestService::getLogsForPluginNameFromDate(const std::vector<std::string> & parameters, const CJson & requestContent)
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

   CHardwareEventLoggerEntitySerializer hes;
   std::vector< boost::shared_ptr<database::entities::CHardwareEventLogger> > dvList = m_dataProvider->getHardwareEventLoggerRequester()->getHardwareEvents(pluginName, pluginVersion, rType, fromDate);
   return CJsonCollectionSerializer<database::entities::CHardwareEventLogger>::SerializeCollection(dvList, hes, getRestKeyword());
}
 