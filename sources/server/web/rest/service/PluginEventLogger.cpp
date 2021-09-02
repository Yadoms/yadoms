#include "stdafx.h"
#include "PluginEventLogger.h"
#include <utility>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/poco/RestResult.h"

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

         void CPluginEventLogger::configurePocoDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("*"), CPluginEventLogger::getLogsForPluginName);
         }

         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CPluginEventLogger::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            //TODO

            return m_endPoints;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPluginEventLogger::getLogsForPluginName(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            boost::posix_time::ptime fromDate;

            if (parameters.empty())
               return poco::CRestResult::GenerateError("Need plugin name");

            const auto pluginName = parameters[1];

            if (parameters.size() > 2)
               fromDate = boost::posix_time::from_iso_string(parameters[2]);

            const auto dvList = m_dataProvider->getPluginEventLoggerRequester()->getPluginEvents(pluginName, fromDate);

            shared::CDataContainer collection;
            collection.set(getRestKeyword(), dvList);
            return poco::CRestResult::GenerateSuccess(collection);
         }
      } //namespace service
   } //namespace rest
} //namespace web 
