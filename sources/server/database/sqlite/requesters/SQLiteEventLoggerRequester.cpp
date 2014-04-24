#include "stdafx.h"
#include "SQLiteEventLoggerRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/InvalidParameter.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"


namespace database { namespace sqlite { namespace requesters { 

   CSQLiteEventLoggerRequester::CSQLiteEventLoggerRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
   }

   CSQLiteEventLoggerRequester::~CSQLiteEventLoggerRequester()
   {
   }

   // IEventLoggerRequester implementation

   int CSQLiteEventLoggerRequester::addEvent(const database::entities::ESystemEventCode & eventCode, const std::string & who, const std::string & what)
   {
      boost::posix_time::ptime insertDate = boost::posix_time::second_clock::universal_time();

      CQuery qInsert;
      qInsert. InsertInto(CEventLoggerTable::getTableName(), CEventLoggerTable::getCodeColumnName(), CEventLoggerTable::getWhoColumnName(), CEventLoggerTable::getWhatColumnName(), CEventLoggerTable::getDateColumnName()).
         Values(eventCode, who, what, insertDate);

      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");

      CQuery qSelect;
      qSelect. Select(CEventLoggerTable::getIdColumnName()).
         From(CEventLoggerTable::getTableName()).
         Where(CEventLoggerTable::getCodeColumnName(), CQUERY_OP_EQUAL, eventCode).
         And(CEventLoggerTable::getDateColumnName(), CQUERY_OP_EQUAL, insertDate).
         OrderBy(CEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
         throw shared::exception::CEmptyResult("Cannot retrieve inserted ");      
   }

   int CSQLiteEventLoggerRequester::addEvent(const database::entities::CEventLogger & logEntry)
   {
      if(!logEntry.Code.isDefined())
         throw shared::exception::CInvalidParameter("The event code must be filled");      

      return addEvent(logEntry.Code(), logEntry.Who(), logEntry.What());
   }


   std::vector<boost::shared_ptr<database::entities::CEventLogger> > CSQLiteEventLoggerRequester::getEvents()
   {
      CQuery qSelect;
      qSelect. Select().
         From(CEventLoggerTable::getTableName()).
         OrderBy(CEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CEventLogger> >(&adapter, qSelect);
      return adapter.getResults();
   }

   boost::shared_ptr<entities::CEventLogger>  CSQLiteEventLoggerRequester::getLastEvent()
   {
      CQuery qSelect;
      qSelect. Select().
         From(CEventLoggerTable::getTableName()).
         OrderBy(CEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC).
         Limit(1);

      database::sqlite::adapters::CEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CEventLogger> >(&adapter, qSelect);

      std::vector<boost::shared_ptr<database::entities::CEventLogger> > list = adapter.getResults();
      if(list.empty())
         return boost::shared_ptr<entities::CEventLogger>(); 
      return adapter.getResults()[0];
   }

   std::vector<boost::shared_ptr<entities::CEventLogger> >  CSQLiteEventLoggerRequester::getEventsFrom(const int eventId)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CEventLoggerTable::getTableName()).
         Where(CEventLoggerTable::getIdColumnName(), CQUERY_OP_SUP, eventId).
         OrderBy(CEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC, CEventLoggerTable::getDateColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CEventLogger> >(&adapter, qSelect);
      return adapter.getResults();
   }

   std::vector<boost::shared_ptr<entities::CEventLogger> >  CSQLiteEventLoggerRequester::getEventsRange(const int offset, const int count)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CEventLoggerTable::getTableName()).
         OrderBy(CEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC, CEventLoggerTable::getDateColumnName(), CQUERY_ORDER_DESC).
         Limit(count, offset);

      database::sqlite::adapters::CEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CEventLogger> >(&adapter, qSelect);
      return adapter.getResults();
   }

   // [END] IEventLoggerRequester implementation

} //namespace requesters
} //namespace sqlite
} //namespace database 

