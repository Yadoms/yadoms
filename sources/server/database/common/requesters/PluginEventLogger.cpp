#include "stdafx.h"
#include "PluginEventLogger.h"
#include <shared/currentTime/Provider.h>
#include <shared/exception/EmptyResult.hpp>
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"


namespace database
{
   namespace common
   {
      namespace requesters
      {
         CPluginEventLogger::CPluginEventLogger(boost::shared_ptr<IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         // IPluginEventLoggerRequester implementation

         int CPluginEventLogger::addEvent(const std::string& pluginName,
                                          const std::string& pluginVersion,
                                          const entities::EEventType& eventType,
                                          const std::string& message)
         {
            const auto insertDate = shared::currentTime::Provider().now();

            auto qInsert = m_databaseRequester->newQuery();
            qInsert->InsertInto(CPluginEventLoggerTable::getTableName(),
                                CPluginEventLoggerTable::getPluginNameColumnName(),
                                CPluginEventLoggerTable::getPluginVersionColumnName(),
                                CPluginEventLoggerTable::getEventTypeColumnName(),
                                CPluginEventLoggerTable::getMessageColumnName(),
                                CPluginEventLoggerTable::getEventDateColumnName()).
                     Values(pluginName,
                            pluginVersion,
                            eventType,
                            message,
                            insertDate);

            if (m_databaseRequester->queryStatement(*qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");

            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CPluginEventLoggerTable::getIdColumnName()).
                     From(CPluginEventLoggerTable::getTableName()).
                     Where(CPluginEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
                     And(CPluginEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
                     And(CPluginEventLoggerTable::getEventTypeColumnName(), CQUERY_OP_EQUAL, eventType).
                     And(CPluginEventLoggerTable::getEventDateColumnName(), CQUERY_OP_EQUAL, insertDate).
                     OrderBy(CPluginEventLoggerTable::getIdColumnName(), CQuery::kDesc);

            adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            throw shared::exception::CEmptyResult("Cannot retrieve inserted Plugin");
         }

         int CPluginEventLogger::addEvent(const entities::CPluginEventLogger& pluginLogEntry)
         {
            return addEvent(pluginLogEntry.PluginName(),
                            pluginLogEntry.PluginVersion(),
                            pluginLogEntry.EventType(),
                            pluginLogEntry.Message());
         }

         std::vector<boost::shared_ptr<entities::CPluginEventLogger>> CPluginEventLogger::getPluginEvents(const std::string& pluginName,
                                                                                                          const boost::posix_time::ptime& fromDate)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CPluginEventLoggerTable::getTableName()).
                     Where(CPluginEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName);

            if (!fromDate.is_special())
               qSelect->And(CPluginEventLoggerTable::getEventDateColumnName(), CQUERY_OP_SUP, fromDate);

            qSelect->OrderBy(CPluginEventLoggerTable::getEventDateColumnName(), CQuery::kDesc);

            adapters::CPluginEventLoggerAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         // [END] IPluginEventLoggerRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 
