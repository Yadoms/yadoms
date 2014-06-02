#include "stdafx.h"
#include "PluginEventLogger.h"
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "shared/Log.h"

namespace web { namespace rest { namespace service {

   CPluginEventLogger::CPluginEventLogger(boost::shared_ptr<database::IDataProvider> dataProvider)
      :m_dataProvider(dataProvider), m_restKeyword("pluginEventLogger")
   {
   }


   CPluginEventLogger::~CPluginEventLogger()
   {
   }

   const std::string & CPluginEventLogger::getRestKeyword()
   {
      return m_restKeyword;
   }

   void CPluginEventLogger::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("*")("*"), CPluginEventLogger::getLogsForPluginName);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("*")("*")("*"), CPluginEventLogger::getLogsForPluginNameFromDate);
   }

   web::rest::json::CJson CPluginEventLogger::getLogsForPluginName(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
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

      web::rest::json::CPluginEventLoggerEntitySerializer hes;
      std::vector< boost::shared_ptr<database::entities::CPluginEventLogger> > dvList = m_dataProvider->getPluginEventLoggerRequester()->getPluginEvents(pluginName, pluginVersion, rType);
      return web::rest::json::CJsonCollectionSerializer<database::entities::CPluginEventLogger>::SerializeCollection(dvList, hes, getRestKeyword());
   }

   web::rest::json::CJson CPluginEventLogger::getLogsForPluginNameFromDate(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
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
         fromDate = web::rest::json::CJsonDate::fromString(parameters[4]);
      
      web::rest::json::CPluginEventLoggerEntitySerializer hes;
      std::vector< boost::shared_ptr<database::entities::CPluginEventLogger> > dvList = m_dataProvider->getPluginEventLoggerRequester()->getPluginEvents(pluginName, pluginVersion, rType, fromDate);
      return web::rest::json::CJsonCollectionSerializer<database::entities::CPluginEventLogger>::SerializeCollection(dvList, hes, getRestKeyword());
   }



} //namespace service
} //namespace rest
} //namespace web 
