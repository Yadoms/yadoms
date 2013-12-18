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


int CSQLiteRequester::queryStatement(const std::string & queryFormat, ...)
{
   char *zSql;
   va_list ap;
   va_start(ap, queryFormat);
   zSql = sqlite3_vmprintf(queryFormat.c_str(), ap);
   va_end(ap);

   query(zSql);

   int nbRowsAffected = sqlite3_changes(m_pDatabaseHandler);

    if(zSql)
      sqlite3_free(zSql);

    return nbRowsAffected;
}


int CSQLiteRequester::queryCount(const std::string & queryFormat, ...)
{
   char *zSql;
   va_list ap;
   va_start(ap, queryFormat);
   zSql = sqlite3_vmprintf(queryFormat.c_str(), ap);
   va_end(ap);

   CSingleValueAdapter<int> countAdapter;
   queryEntities<int>(&countAdapter, zSql);

   if(zSql)
      sqlite3_free(zSql);

   
   if(countAdapter.getResults().size() >= 1)
      return countAdapter.getResults()[0];
   return -1;
}

CSQLiteRequester::QueryRow CSQLiteRequester::querySingleLine(const std::string & queryFormat, ...)
{
   char *zSql;
   va_list ap;
   va_start(ap, queryFormat);
   zSql = sqlite3_vmprintf(queryFormat.c_str(), ap);
   va_end(ap);

   CSQLiteRequester::QueryResults results = query(zSql);

   if(zSql)
      sqlite3_free(zSql);

   if(results.size() >= 1)
      return results[0];

   return CSQLiteRequester::QueryRow(); //returns empty data
}


CSQLiteRequester::QueryResults CSQLiteRequester::query(const std::string & queryFormat, ...)
{
   char *zSql;
   va_list ap;
   va_start(ap, queryFormat);
   zSql = sqlite3_vmprintf(queryFormat.c_str(), ap);
   va_end(ap);

   CGenericAdapter genericAdapter;
   queryEntities<std::map<std::string, std::string> >(&genericAdapter, zSql);

   if(zSql)
      sqlite3_free(zSql);
   return genericAdapter.getResults();
}



