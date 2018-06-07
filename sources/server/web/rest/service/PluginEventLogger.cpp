#include "stdafx.h"
#include "PluginEventLogger.h"
#include "web/rest/RestDispatcherHelpers.hpp"
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

   boost::shared_ptr<shared::serialization::IDataSerializable> CPluginEventLogger::getLogsForPluginName(const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      std::string pluginName = "";
      std::string pluginVersion = "";
      if(parameters.size()>1)
         pluginName = parameters[1];
      if(parameters.size()>2)
         pluginVersion = parameters[2];

      std::vector< boost::shared_ptr<database::entities::CPluginEventLogger> > dvList = m_dataProvider->getPluginEventLoggerRequester()->getPluginEvents(pluginName, pluginVersion);
      shared::CDataContainer collection;
      collection.set(getRestKeyword(), dvList);
      return CResult::GenerateSuccess(collection);
   }

   boost::shared_ptr<shared::serialization::IDataSerializable> CPluginEventLogger::getLogsForPluginNameFromDate(const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      std::string pluginName = "";
      std::string pluginVersion = "";
      boost::posix_time::ptime fromDate;

      if(parameters.size()>1)
         pluginName = parameters[1];
      if(parameters.size()>2)
         pluginVersion = parameters[2];
      if(parameters.size()>3)
         fromDate = boost::posix_time::from_iso_string(parameters[3]);
      
      std::vector< boost::shared_ptr<database::entities::CPluginEventLogger> > dvList = m_dataProvider->getPluginEventLoggerRequester()->getPluginEvents(pluginName, pluginVersion, fromDate);
      shared::CDataContainer collection;
      collection.set(getRestKeyword(), dvList);
      return CResult::GenerateSuccess(collection);
   }



} //namespace service
} //namespace rest
} //namespace web 
