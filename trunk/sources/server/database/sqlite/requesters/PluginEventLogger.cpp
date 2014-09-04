#include "stdafx.h"
#include "PluginEventLogger.h"
#include <shared/exception/NotImplemented.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"


namespace database { namespace sqlite { namespace requesters { 

   CPluginEventLogger::CPluginEventLogger(CSQLiteDataProvider * databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
   }

   CPluginEventLogger::~CPluginEventLogger()
   {
   }

   // IPluginEventLoggerRequester implementation

   int CPluginEventLogger::addEvent(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::information::EReleaseType & pluginReleaseType, const database::entities::EEventType & eventType, const std::string & message /*= CStringExtension::EmptyString*/)
   {
      boost::posix_time::ptime insertDate = boost::posix_time::second_clock::universal_time();

      CQuery qInsert;
      qInsert. InsertInto(CPluginEventLoggerTable::getTableName(), CPluginEventLoggerTable::getPluginNameColumnName(), CPluginEventLoggerTable::getPluginVersionColumnName(), CPluginEventLoggerTable::getPluginReleaseColumnName(), CPluginEventLoggerTable::getEventTypeColumnName(), CPluginEventLoggerTable::getMessageColumnName(), CPluginEventLoggerTable::getEventDateColumnName()).
         Values(pluginName, pluginVersion, pluginReleaseType, eventType, message, insertDate);

      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");

      CQuery qSelect;
      qSelect. Select(CPluginEventLoggerTable::getIdColumnName()).
         From(CPluginEventLoggerTable::getTableName()).
         Where(CPluginEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
         And(CPluginEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
         And(CPluginEventLoggerTable::getPluginReleaseColumnName(), CQUERY_OP_EQUAL, pluginReleaseType).
         And(CPluginEventLoggerTable::getEventTypeColumnName(), CQUERY_OP_EQUAL, eventType).
         And(CPluginEventLoggerTable::getEventDateColumnName(), CQUERY_OP_EQUAL, insertDate).
         OrderBy(CPluginEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
         throw shared::exception::CEmptyResult("Cannot retrieve inserted Plugin");      
   }

   int CPluginEventLogger::addEvent(const database::entities::CPluginEventLogger & pluginLogEntry)
   {
      return addEvent(pluginLogEntry.PluginName(), pluginLogEntry.PluginVersion(), pluginLogEntry.PluginRelease(), pluginLogEntry.EventType(), pluginLogEntry.Message());
   }

   std::vector<boost::shared_ptr<database::entities::CPluginEventLogger> > CPluginEventLogger::getPluginEvents(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::information::EReleaseType & pluginReleaseType)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CPluginEventLoggerTable::getTableName()).
         Where(CPluginEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
         And(CPluginEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
         And(CPluginEventLoggerTable::getPluginReleaseColumnName(), CQUERY_OP_EQUAL, pluginReleaseType).
         OrderBy(CPluginEventLoggerTable::getEventDateColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CPluginEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CPluginEventLogger> >(&adapter, qSelect);
      return adapter.getResults();
   }


   std::vector<boost::shared_ptr<database::entities::CPluginEventLogger> > CPluginEventLogger::getPluginEvents(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::information::EReleaseType & pluginReleaseType, const boost::posix_time::ptime & fromDate)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CPluginEventLoggerTable::getTableName()).
         Where(CPluginEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
         And(CPluginEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
         And(CPluginEventLoggerTable::getPluginReleaseColumnName(), CQUERY_OP_EQUAL, pluginReleaseType).
         And(CPluginEventLoggerTable::getEventDateColumnName(), CQUERY_OP_SUP, fromDate).
         OrderBy(CPluginEventLoggerTable::getEventDateColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CPluginEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CPluginEventLogger> >(&adapter, qSelect);
      return adapter.getResults();
   }
   // [END] IPluginEventLoggerRequester implementation

} //namespace requesters
} //namespace sqlite
} //namespace database 

