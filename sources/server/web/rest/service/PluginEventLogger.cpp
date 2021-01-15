#include "stdafx.h"
#include "PluginEventLogger.h"

#include <utility>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         CPluginEventLogger::CPluginEventLogger(boost::shared_ptr<database::IDataProvider> dataProvider)
            : m_dataProvider(std::move(dataProvider)),
              m_restKeyword("pluginEventLogger")
         {
         }

         const std::string& CPluginEventLogger::getRestKeyword() const
         {
            return m_restKeyword;
         }

         void CPluginEventLogger::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CPluginEventLogger::getLogsForPluginName);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("*"), CPluginEventLogger::getLogsForPluginName);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPluginEventLogger::getLogsForPluginName(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            std::string pluginName;
            boost::posix_time::ptime fromDate;

            if (parameters.size() > 1)
               pluginName = parameters[1];
            if (parameters.size() > 3)
               fromDate = boost::posix_time::from_iso_string(parameters[3]);

            const auto dvList = m_dataProvider->getPluginEventLoggerRequester()->getPluginEvents(pluginName, fromDate);

            shared::CDataContainer collection;
            collection.set(getRestKeyword(), dvList);
            return CResult::GenerateSuccess(collection);
         }
      } //namespace service
   } //namespace rest
} //namespace web 
