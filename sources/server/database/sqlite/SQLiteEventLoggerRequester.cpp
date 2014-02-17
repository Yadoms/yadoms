#include "stdafx.h"
#include "SQLiteEventLoggerRequester.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/SQLiteDatabaseAdapters.h"
#include <shared/Exceptions/EmptyResultException.hpp>
#include "SQLiteDatabaseTables.h"
#include "Query.h"


CSQLiteEventLoggerRequester::CSQLiteEventLoggerRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
   :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
{
}

CSQLiteEventLoggerRequester::~CSQLiteEventLoggerRequester()
{
}

// IEventLoggerRequester implementation
int CSQLiteEventLoggerRequester::addEvent(const int eventType, const std::string & optionalData)
{
   boost::posix_time::ptime insertDate = boost::posix_time::second_clock::universal_time();

   CQuery qInsert;
   qInsert. InsertInto(CEventLoggerTable::getTableName(), CEventLoggerTable::getEventTypeColumnName(), CEventLoggerTable::getOptionalDataColumnName(), CEventLoggerTable::getEventDateColumnName()).
      Values(eventType, optionalData, insertDate);
   
   if(m_databaseRequester->queryStatement(qInsert) <= 0)
      throw CEmptyResultException("No lines affected");

   CQuery qSelect;
   qSelect. Select(CEventLoggerTable::getIdColumnName()).
      From(CEventLoggerTable::getTableName()).
      Where(CEventLoggerTable::getEventTypeColumnName(), CQUERY_OP_EQUAL, eventType).
      And(CEventLoggerTable::getEventDateColumnName(), CQUERY_OP_EQUAL, insertDate).
      OrderBy(CEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC);

   CSingleValueAdapter<int> adapter;
   m_databaseRequester->queryEntities<int>(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
      throw CEmptyResultException("Cannot retrieve inserted ");      
}

int CSQLiteEventLoggerRequester::addEvent(const CEventLogger & logEntry)
{
   return addEvent(logEntry.getEventType(), logEntry.getOptionalData());
}


std::vector<boost::shared_ptr<CEventLogger> > CSQLiteEventLoggerRequester::getEvents()
{
   CQuery qSelect;
   qSelect. Select().
      From(CEventLoggerTable::getTableName()).
      OrderBy(CEventLoggerTable::getEventDateColumnName(), CQUERY_ORDER_DESC);

   CEventLoggerAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CEventLogger> >(&adapter, qSelect);
   return adapter.getResults();
}


// [END] IEventLoggerRequester implementation




