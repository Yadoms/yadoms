#include "stdafx.h"

#include "SQLiteRequester.h"
#include "tools/Exceptions/NotImplementedException.h"
#include "adapters/GenericAdapter.h"
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
   throw CNotImplementedException();
}


int CSQLiteRequester::queryCount(const std::string & queryFormat, ...)
{
   char *zSql;
   va_list ap;
   va_start(ap, queryFormat);
   zSql = sqlite3_vmprintf(queryFormat.c_str(), ap);
   va_end(ap);

   CSQLiteRequester::QueryResults results = query(zSql);

   if(zSql)
      sqlite3_free(zSql);

   if(results.size() >= 1 && results[0].size() == 1)
      return CStringExtension::parse<int>(results[0][0].c_str());
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



