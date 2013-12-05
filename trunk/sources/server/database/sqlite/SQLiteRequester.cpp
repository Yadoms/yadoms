#include "stdafx.h"

#include "SQLiteRequester.h"
#include "tools/Exceptions/NotImplementedException.h"
#include "adapters/GenericAdapter.h"

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

CSQLiteRequester::QueryRow CSQLiteRequester::querySingleLine(const std::string & queryFormat, ...)
{
   throw CNotImplementedException();
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



