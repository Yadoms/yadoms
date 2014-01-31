#include "stdafx.h"

#include "SQLiteRequester.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "adapters/GenericAdapter.h"
#include "adapters/SingleValueAdapter.hpp"
#include <shared/StringExtension.h>
#include "SQLiteSystemTables.h"


CSQLiteRequester::CSQLiteRequester(sqlite3 * pDatabaseHandler)
   :m_pDatabaseHandler(pDatabaseHandler)
{
}

CSQLiteRequester::~CSQLiteRequester()
{
}


int CSQLiteRequester::queryStatement(const CQuery & querytoExecute)
{
   query(querytoExecute);
   return sqlite3_changes(m_pDatabaseHandler);
}


int CSQLiteRequester::queryCount(const CQuery & querytoExecute)
{
   CSingleValueAdapter<int> countAdapter;
   queryEntities<int>(&countAdapter, querytoExecute);

   if(countAdapter.getResults().size() >= 1)
      return countAdapter.getResults()[0];
   return -1;
}

CSQLiteRequester::QueryRow CSQLiteRequester::querySingleLine(const CQuery & querytoExecute)
{
   CSQLiteRequester::QueryResults results = query(querytoExecute);
   if(results.size() >= 1)
      return results[0];

   return CSQLiteRequester::QueryRow(); //returns empty data
}


CSQLiteRequester::QueryResults CSQLiteRequester::query(const CQuery & querytoExecute)
{
   CGenericAdapter genericAdapter;
   queryEntities<std::map<std::string, std::string> >(&genericAdapter, querytoExecute);
   return genericAdapter.getResults();
}


void CSQLiteRequester::transactionBegin()
{
   sqlite3_exec(m_pDatabaseHandler, "BEGIN", 0, 0, 0);
}

void CSQLiteRequester::transactionCommit()
{
   sqlite3_exec(m_pDatabaseHandler, "COMMIT", 0, 0, 0);
}

void CSQLiteRequester::transactionRollback()
{
   sqlite3_exec(m_pDatabaseHandler, "ROLLBACK", 0, 0, 0);
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
      queryStatement(CQuery::CustomQuery(tableScript));
      return checkTableExists(tableName);
   }
   return true;
}


