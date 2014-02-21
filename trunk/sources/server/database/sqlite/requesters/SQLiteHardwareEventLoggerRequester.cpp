#include "stdafx.h"
#include "SQLiteHardwareEventLoggerRequester.h"
#include <shared/exceptions/NotImplemented.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include <shared/exceptions/EmptyResult.hpp>
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"

namespace server { 
   namespace database { 
      namespace sqlite { 
         namespace requesters { 

            CSQLiteHardwareEventLoggerRequester::CSQLiteHardwareEventLoggerRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
               :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
            {
            }

            CSQLiteHardwareEventLoggerRequester::~CSQLiteHardwareEventLoggerRequester()
            {
            }

            // IHardwareEventLoggerRequester implementation

            int CSQLiteHardwareEventLoggerRequester::addEvent(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::information::EReleaseType pluginReleaseType, const server::database::entities::CHardwareEventLogger::EEventType eventType, const std::string & message /*= CStringExtension::EmptyString*/)
            {
               boost::posix_time::ptime insertDate = boost::posix_time::second_clock::universal_time();

               CQuery qInsert;
               qInsert. InsertInto(CHardwareEventLoggerTable::getTableName(), CHardwareEventLoggerTable::getPluginNameColumnName(), CHardwareEventLoggerTable::getPluginVersionColumnName(), CHardwareEventLoggerTable::getPluginReleaseColumnName(), CHardwareEventLoggerTable::getEventTypeColumnName(), CHardwareEventLoggerTable::getMessageColumnName(), CHardwareEventLoggerTable::getEventDateColumnName()).
                  Values(pluginName, pluginVersion, pluginReleaseType, eventType, message, insertDate);

               if(m_databaseRequester->queryStatement(qInsert) <= 0)
                  throw shared::exception::CEmptyResult("No lines affected");

               CQuery qSelect;
               qSelect. Select(CHardwareEventLoggerTable::getIdColumnName()).
                  From(CHardwareEventLoggerTable::getTableName()).
                  Where(CHardwareEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
                  And(CHardwareEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
                  And(CHardwareEventLoggerTable::getPluginReleaseColumnName(), CQUERY_OP_EQUAL, pluginReleaseType).
                  And(CHardwareEventLoggerTable::getEventTypeColumnName(), CQUERY_OP_EQUAL, eventType).
                  And(CHardwareEventLoggerTable::getEventDateColumnName(), CQUERY_OP_EQUAL, insertDate).
                  OrderBy(CHardwareEventLoggerTable::getIdColumnName(), CQUERY_ORDER_DESC);

               server::database::sqlite::adapters::CSingleValueAdapter<int> adapter;
               m_databaseRequester->queryEntities<int>(&adapter, qSelect);
               if(adapter.getResults().size() >= 1)
                  return adapter.getResults()[0];
               else
                  throw shared::exception::CEmptyResult("Cannot retrieve inserted Hardware");      
            }

            int CSQLiteHardwareEventLoggerRequester::addEvent(const server::database::entities::CHardwareEventLogger & hardwareLogEntry)
            {
               return addEvent(hardwareLogEntry.getPluginName(), hardwareLogEntry.getPluginVersion(), hardwareLogEntry.getPluginRelease(), hardwareLogEntry.getEventType(), hardwareLogEntry.getMessage());
            }

            std::vector<boost::shared_ptr<server::database::entities::CHardwareEventLogger> > CSQLiteHardwareEventLoggerRequester::getHardwareEvents(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::information::EReleaseType pluginReleaseType)
            {
               CQuery qSelect;
               qSelect. Select().
                  From(CHardwareEventLoggerTable::getTableName()).
                  Where(CHardwareEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
                  And(CHardwareEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
                  And(CHardwareEventLoggerTable::getPluginReleaseColumnName(), CQUERY_OP_EQUAL, pluginReleaseType).
                  OrderBy(CHardwareEventLoggerTable::getEventDateColumnName(), CQUERY_ORDER_DESC);

               server::database::sqlite::adapters::CHardwareEventLoggerAdapter adapter;
               m_databaseRequester->queryEntities<boost::shared_ptr<server::database::entities::CHardwareEventLogger> >(&adapter, qSelect);
               return adapter.getResults();
            }


            std::vector<boost::shared_ptr<server::database::entities::CHardwareEventLogger> > CSQLiteHardwareEventLoggerRequester::getHardwareEvents(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::information::EReleaseType pluginReleaseType, const boost::posix_time::ptime & fromDate)
            {
               CQuery qSelect;
               qSelect. Select().
                  From(CHardwareEventLoggerTable::getTableName()).
                  Where(CHardwareEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
                  And(CHardwareEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
                  And(CHardwareEventLoggerTable::getPluginReleaseColumnName(), CQUERY_OP_EQUAL, pluginReleaseType).
                  And(CHardwareEventLoggerTable::getEventDateColumnName(), CQUERY_OP_SUP, fromDate).
                  OrderBy(CHardwareEventLoggerTable::getEventDateColumnName(), CQUERY_ORDER_DESC);

               server::database::sqlite::adapters::CHardwareEventLoggerAdapter adapter;
               m_databaseRequester->queryEntities<boost::shared_ptr<server::database::entities::CHardwareEventLogger> >(&adapter, qSelect);
               return adapter.getResults();
            }
            // [END] IHardwareEventLoggerRequester implementation

         } //namespace requesters
      } //namespace sqlite
   } //namespace database 
} //namespace server
