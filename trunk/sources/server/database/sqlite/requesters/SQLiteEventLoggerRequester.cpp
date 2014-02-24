#include "stdafx.h"
#include "SQLiteEventLoggerRequester.h"
#include <shared/exceptions/NotImplemented.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include <shared/exceptions/EmptyResult.hpp>
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"


   namespace database { 
      namespace sqlite { 
         namespace requesters { 

            CSQLiteEventLoggerRequester::CSQLiteEventLoggerRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
               :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
            {
            }

            CSQLiteEventLoggerRequester::~CSQLiteEventLoggerRequester()
            {
            }

            // IEventLoggerRequester implementation
            int CSQLiteEventLoggerRequester::addEvent(const std::string & eventType, const std::string & optionalData)
            {
               boost::posix_time::ptime insertDate = boost::posix_time::second_clock::universal_time();

               CQuery qInsert;
               qInsert. InsertInto(CEventLoggerTable::getTableName(), CEventLoggerTable::getEventTypeColumnName(), CEventLoggerTable::getOptionalDataColumnName(), CEventLoggerTable::getEventDateColumnName()).
                  Values(eventType, optionalData, insertDate);

               if(m_databaseRequester->queryStatement(qInsert) <= 0)
                  throw shared::exception::CEmptyResult("No lines affected");

               CQuery qSelect;
               qSelect. Select(CEventLoggerTable::getIdColumnName()).
                  From(CEventLoggerTable::getTableName()).
                  Where(CEventLoggerTable::getEventTypeColumnName(), CQUERY_OP_EQUAL, eventType).
                  And(CEventLoggerTable::getEventDateColumnName(), CQUERY_OP_EQUAL, insertDate).
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
               return addEvent(logEntry.getEventType(), logEntry.getOptionalData());
            }


            std::vector<boost::shared_ptr<database::entities::CEventLogger> > CSQLiteEventLoggerRequester::getEvents()
            {
               CQuery qSelect;
               qSelect. Select().
                  From(CEventLoggerTable::getTableName()).
                  OrderBy(CEventLoggerTable::getEventDateColumnName(), CQUERY_ORDER_DESC);

               database::sqlite::adapters::CEventLoggerAdapter adapter;
               m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CEventLogger> >(&adapter, qSelect);
               return adapter.getResults();
            }
            // [END] IEventLoggerRequester implementation

         } //namespace requesters
      } //namespace sqlite
   } //namespace database 

