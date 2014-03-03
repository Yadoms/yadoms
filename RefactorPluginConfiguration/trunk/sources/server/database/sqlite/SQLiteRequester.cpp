#include "stdafx.h"

#include "SQLiteRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include "adapters/GenericAdapter.h"
#include "adapters/SingleValueAdapter.hpp"
#include <shared/StringExtension.h>
#include "SQLiteSystemTables.h"
#include "database/DatabaseException.hpp"


namespace database { 
namespace sqlite { 

   CSQLiteRequester::CSQLiteRequester(sqlite3 * pDatabaseHandler)
      :m_pDatabaseHandler(pDatabaseHandler), m_bOneTransactionActive(false)
   {
   }

   CSQLiteRequester::~CSQLiteRequester()
   {
   }


   int CSQLiteRequester::queryStatement(const CQuery & querytoExecute)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() != CQuery::kSelect);

      //execute the query
      char *zErrMsg = NULL;
      sqlite3_exec(m_pDatabaseHandler, querytoExecute.c_str(),  NULL, 0, &zErrMsg);

      if(zErrMsg)
      {
         //make a copy of the err message
         std::string errMessage(zErrMsg);

         //log the message
         YADOMS_LOG(error) << "Query failed : " << std::endl << "Query: " << querytoExecute.str() << std::endl << "Error : " << zErrMsg;

         //free allocated memory by sqlite
         sqlite3_free(zErrMsg);

         //throw
         throw CDatabaseException(errMessage);
      }
      return sqlite3_changes(m_pDatabaseHandler);
   }


   int CSQLiteRequester::queryCount(const CQuery & querytoExecute)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() == CQuery::kSelect);

      database::sqlite::adapters::CSingleValueAdapter<int> countAdapter;
      queryEntities<int>(&countAdapter, querytoExecute);

      if(countAdapter.getResults().size() >= 1)
         return countAdapter.getResults()[0];
      return -1;
   }

   CSQLiteRequester::QueryRow CSQLiteRequester::querySingleLine(const CQuery & querytoExecute)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() == CQuery::kSelect);

      CSQLiteRequester::QueryResults results = query(querytoExecute);
      if(results.size() >= 1)
         return results[0];

      return CSQLiteRequester::QueryRow(); //returns empty data
   }


   CSQLiteRequester::QueryResults CSQLiteRequester::query(const CQuery & querytoExecute)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() == CQuery::kSelect);

      database::sqlite::adapters::CGenericAdapter genericAdapter;
      queryEntities<std::map<std::string, std::string> >(&genericAdapter, querytoExecute);
      return genericAdapter.getResults();
   }


   void CSQLiteRequester::transactionBegin()
   {
      if(!m_bOneTransactionActive)
      {
         sqlite3_exec(m_pDatabaseHandler, "BEGIN", 0, 0, 0);
         m_bOneTransactionActive = true;
      }
   }

   void CSQLiteRequester::transactionCommit()
   {
      if(m_bOneTransactionActive)
      {
         sqlite3_exec(m_pDatabaseHandler, "COMMIT", 0, 0, 0);
         m_bOneTransactionActive = false;
      }
   }

   void CSQLiteRequester::transactionRollback()
   {
      if(m_bOneTransactionActive)
      {
         sqlite3_exec(m_pDatabaseHandler, "ROLLBACK", 0, 0, 0);
         m_bOneTransactionActive = false;
      }
   }

   bool CSQLiteRequester::transactionIsAlreadyCreated()
   {
      return m_bOneTransactionActive;
   }

   bool CSQLiteRequester::checkTableExists(const std::string & tableName)
   {
      //check that table Configuration exists
      CQuery sCheckForTableExists;
      sCheckForTableExists.SelectCount().
         From(CSqliteMasterTable::getTableName()).
         Where(CSqliteMasterTable::getTypeColumnName(), CQUERY_OP_EQUAL, SQLITEMASTER_TABLE).
         And(CSqliteMasterTable::getNameColumnName(), CQUERY_OP_EQUAL, tableName);
      int count = queryCount(sCheckForTableExists);
      return (count == 1);
   }


   bool CSQLiteRequester::dropTableIfExists(const std::string & tableName)
   {
      if(checkTableExists(tableName))
      {
         CQuery drop;
         queryStatement(drop.DropTable(tableName));
         return !checkTableExists(tableName);
      }
      return true;
   }

   bool CSQLiteRequester::createTableIfNotExists(const std::string & tableName, const std::string & tableScript)
   {
      if(!checkTableExists(tableName))
      {
         queryStatement(CQuery::CustomQuery(tableScript, CQuery::kCreate));
         return checkTableExists(tableName);
      }
      return true;
   }


} //namespace sqlite
} //namespace database 

