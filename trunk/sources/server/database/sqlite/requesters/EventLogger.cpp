#include "stdafx.h"
#include "EventLogger.h"
#include <shared/currentTime/Provider.h>
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/InvalidParameter.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"


namespace database { namespace sqlite { namespace requesters { 

   CEventLogger::CEventLogger(boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseRequester(databaseRequester)
   {
   }

   CEventLogger::~CEventLogger()
   {
   }

   // IEventLoggerRequester implementation

   int CEventLogger::addEvent(const entities::ESystemEventCode & eventCode, const std::string & who, const std::string & what)
   {
      boost::posix_time::ptime insertDate = shared::currentTime::Provider::now();

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

      adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      
      throw shared::exception::CEmptyResult("Cannot retrieve inserted ");
   }


   std::vector<boost::shared_ptr<entities::CEventLogger> > CEventLogger::getEvents()
   {
      CQuery qSelect;
      qSelect. Select().
         From(CEventLoggerTable::getTableName()).
         OrderBy(CEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC);

      adapters::CEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<entities::CEventLogger> >(&adapter, qSelect);
      return adapter.getResults();
   }

   boost::shared_ptr<entities::CEventLogger>  CEventLogger::getEvent(const int eventId)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CEventLoggerTable::getTableName()).
         Where(CEventLoggerTable::getIdColumnName(), CQUERY_OP_EQUAL, eventId).
         Limit(1);

      adapters::CEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<entities::CEventLogger> >(&adapter, qSelect);
      if(adapter.getResults().empty())
         return boost::shared_ptr<entities::CEventLogger>(); 
      return adapter.getResults()[0];
   }

   boost::shared_ptr<entities::CEventLogger>  CEventLogger::getLastEvent()
   {
      CQuery qSelect;
      qSelect. Select().
         From(CEventLoggerTable::getTableName()).
         OrderBy(CEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC).
         Limit(1);

      adapters::CEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<entities::CEventLogger> >(&adapter, qSelect);

      std::vector<boost::shared_ptr<entities::CEventLogger> > list = adapter.getResults();
      if(list.empty())
         return boost::shared_ptr<entities::CEventLogger>(); 
      return adapter.getResults()[0];
   }

   std::vector<boost::shared_ptr<entities::CEventLogger> >  CEventLogger::getEventsFrom(const int eventId)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CEventLoggerTable::getTableName()).
         Where(CEventLoggerTable::getIdColumnName(), CQUERY_OP_SUP, eventId).
         OrderBy(CEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC, CEventLoggerTable::getDateColumnName(), CQUERY_ORDER_DESC);

      adapters::CEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<entities::CEventLogger> >(&adapter, qSelect);
      return adapter.getResults();
   }

   std::vector<boost::shared_ptr<entities::CEventLogger> >  CEventLogger::getEventsRange(const int offset, const int count)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CEventLoggerTable::getTableName()).
         OrderBy(CEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC, CEventLoggerTable::getDateColumnName(), CQUERY_ORDER_DESC).
         Limit(count, offset);

      adapters::CEventLoggerAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<entities::CEventLogger> >(&adapter, qSelect);
      return adapter.getResults();
   }

   // [END] IEventLoggerRequester implementation

} //namespace requesters
} //namespace sqlite
} //namespace database 

