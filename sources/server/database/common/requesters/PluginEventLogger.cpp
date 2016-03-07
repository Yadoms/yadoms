#include "stdafx.h"
#include "PluginEventLogger.h"
#include <shared/currentTime/Provider.h>
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"


namespace database { namespace common { namespace requesters {

   CPluginEventLogger::CPluginEventLogger(boost::shared_ptr<IDatabaseRequester> databaseRequester)
      :m_databaseRequester(databaseRequester)
   {
   }

   CPluginEventLogger::~CPluginEventLogger()
   {
   }

   // IPluginEventLoggerRequester implementation

   int CPluginEventLogger::addEvent(const std::string & pluginName, const std::string & pluginVersion, const shared::versioning::EReleaseType & pluginReleaseType, const entities::EEventType & eventType, const std::string & message /*= CStringExtension::EmptyString*/)
   {
      boost::posix_time::ptime insertDate = shared::currentTime::Provider::now();

      CQuery qInsert = m_databaseRequester->newQuery();
      qInsert. InsertInto(CPluginEventLoggerTable::getTableName(), CPluginEventLoggerTable::getPluginNameColumnName(), CPluginEventLoggerTable::getPluginVersionColumnName(), CPluginEventLoggerTable::getPluginReleaseColumnName(), CPluginEventLoggerTable::getEventTypeColumnName(), CPluginEventLoggerTable::getMessageColumnName(), CPluginEventLoggerTable::getEventDateColumnName()).
         Values(pluginName, pluginVersion, pluginReleaseType, eventType, message, insertDate);

      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");

      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect. Select(CPluginEventLoggerTable::getIdColumnName()).
         From(CPluginEventLoggerTable::getTableName()).
         Where(CPluginEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
         And(CPluginEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
         And(CPluginEventLoggerTable::getPluginReleaseColumnName(), CQUERY_OP_EQUAL, pluginReleaseType).
         And(CPluginEventLoggerTable::getEventTypeColumnName(), CQUERY_OP_EQUAL, eventType).
         And(CPluginEventLoggerTable::getEventDateColumnName(), CQUERY_OP_EQUAL, insertDate).
         OrderBy(CPluginEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC);

      adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      
      throw shared::exception::CEmptyResult("Cannot retrieve inserted Plugin");
   }

   int CPluginEventLogger::addEvent(const entities::CPluginEventLogger & pluginLogEntry)
   {
      return addEvent(pluginLogEntry.PluginName(), pluginLogEntry.PluginVersion(), pluginLogEntry.PluginRelease(), pluginLogEntry.EventType(), pluginLogEntry.Message());
   }

   std::vector<boost::shared_ptr<entities::CPluginEventLogger> > CPluginEventLogger::getPluginEvents(const std::string & pluginName, const std::string & pluginVersion, const shared::versioning::EReleaseType & pluginReleaseType)
   {
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect. Select().
         From(CPluginEventLoggerTable::getTableName()).
         Where(CPluginEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
         And(CPluginEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
         And(CPluginEventLoggerTable::getPluginReleaseColumnName(), CQUERY_OP_EQUAL, pluginReleaseType).
         OrderBy(CPluginEventLoggerTable::getEventDateColumnName(), CQUERY_ORDER_DESC);

      adapters::CPluginEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);
      return adapter.getResults();
   }


   std::vector<boost::shared_ptr<entities::CPluginEventLogger> > CPluginEventLogger::getPluginEvents(const std::string & pluginName, const std::string & pluginVersion, const shared::versioning::EReleaseType & pluginReleaseType, const boost::posix_time::ptime & fromDate)
   {
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect. Select().
         From(CPluginEventLoggerTable::getTableName()).
         Where(CPluginEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
         And(CPluginEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
         And(CPluginEventLoggerTable::getPluginReleaseColumnName(), CQUERY_OP_EQUAL, pluginReleaseType).
         And(CPluginEventLoggerTable::getEventDateColumnName(), CQUERY_OP_SUP, fromDate).
         OrderBy(CPluginEventLoggerTable::getEventDateColumnName(), CQUERY_ORDER_DESC);

      adapters::CPluginEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);
      return adapter.getResults();
   }
   // [END] IPluginEventLoggerRequester implementation

} //namespace requesters
} //namespace common
} //namespace database 

