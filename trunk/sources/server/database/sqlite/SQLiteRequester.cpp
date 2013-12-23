#include "stdafx.h"

#include "SQLiteRequester.h"
#include "tools/Exceptions/NotImplementedException.hpp"
#include "adapters/GenericAdapter.h"
#include "adapters/SingleValueAdapter.hpp"
#include "tools/StringExtension.hpp"

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



