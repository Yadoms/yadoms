#include "stdafx.h"
#include "PluginEventLogger.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "shared/Log.h"
#include "web/rest/Result.h"

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

   shared::CDataContainer CPluginEventLogger::getLogsForPluginName(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::string pluginName = "";
      std::string pluginVersion = "";
      shared::versioning::EReleaseType rType = shared::versioning::EReleaseType::kStable;

      if(parameters.size()>1)
         pluginName = parameters[1];
      if(parameters.size()>2)
         pluginVersion = parameters[2];
      if(parameters.size()>3)
         rType.fromString(parameters[3]);

      std::vector< boost::shared_ptr<database::entities::CPluginEventLogger> > dvList = m_dataProvider->getPluginEventLoggerRequester()->getPluginEvents(pluginName, pluginVersion, rType);
      shared::CDataContainer collection;
      collection.set(getRestKeyword(), dvList);
      return CResult::GenerateSuccess(collection);
   }

   shared::CDataContainer CPluginEventLogger::getLogsForPluginNameFromDate(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::string pluginName = "";
      std::string pluginVersion = "";
      boost::posix_time::ptime fromDate;
      shared::versioning::EReleaseType rType = shared::versioning::EReleaseType::kStable;

      if(parameters.size()>1)
         pluginName = parameters[1];
      if(parameters.size()>2)
         pluginVersion = parameters[2];
      if(parameters.size()>3)
         rType.fromString(parameters[3]);
      if(parameters.size()>4)
         fromDate = boost::posix_time::from_iso_string(parameters[4]);
      
      std::vector< boost::shared_ptr<database::entities::CPluginEventLogger> > dvList = m_dataProvider->getPluginEventLoggerRequester()->getPluginEvents(pluginName, pluginVersion, rType, fromDate);
      shared::CDataContainer collection;
      collection.set(getRestKeyword(), dvList);
      return CResult::GenerateSuccess(collection);
   }



} //namespace service
} //namespace rest
} //namespace web 
