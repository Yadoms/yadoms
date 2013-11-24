#pragma once

#include <boost/log/trivial.hpp>
#include "SQLiteDatabaseProvider.h"
#include "SQLiteHardwareRequester.h"
#include "sqlite3.h"
#include "tools/Exceptions/NotImplementedException.h"
#include "boost/function.hpp"
#include "boost/bind.hpp"

CSQLiteDatabaseProvider::CSQLiteDatabaseProvider(const IStartupOptions& startupOptions)
   :m_startupOptions(startupOptions), m_pDatabaseHandler(NULL)
{
}

CSQLiteDatabaseProvider::~CSQLiteDatabaseProvider()
{
   BOOST_ASSERT(m_pDatabaseHandler == NULL);
}


// IDatabaseProvider implementation
bool CSQLiteDatabaseProvider::load()
{
   BOOST_LOG_TRIVIAL(info) << "Load database";

   bool result = false;

   try
   {
      int rc = sqlite3_open(m_startupOptions.getDatabaseFile().c_str(), &m_pDatabaseHandler);
      if(rc)
      {
         BOOST_LOG_TRIVIAL(fatal) << "Fail to load database: " << sqlite3_errmsg(m_pDatabaseHandler);
         sqlite3_close(m_pDatabaseHandler);
         result = false;
      }
      else
      {
         //db loaded with succes, create requesters
         loadRequesters();

         BOOST_LOG_TRIVIAL(info) << "Load database with success";
         result = true;

      }
   }
   catch(...)
   {
      BOOST_LOG_TRIVIAL(error) << "Unknow exception while loading database";
      sqlite3_close(m_pDatabaseHandler);
      result = false;
   }
   return result;
}



void CSQLiteDatabaseProvider::unload()
{
   sqlite3_close(m_pDatabaseHandler);  
}

// [END] IHardwarePlugin implementation


void CSQLiteDatabaseProvider::loadRequesters()
{
   m_hardwareRequester.reset(new CSQLiteHardwareRequester(this));
}


int CSQLiteDatabaseProvider::queryStatement(const std::string & queryFormat, ...)
{
   throw CNotImplementedException();
}

IDatabaseProvider::QueryRow CSQLiteDatabaseProvider::querySingleLine(const std::string & queryFormat, ...)
{
   throw CNotImplementedException();
}

IDatabaseProvider::QueryResults CSQLiteDatabaseProvider::query(const std::string & queryFormat, ...)
{
   throw CNotImplementedException();
   //ASSERT(T is IRowHandler);

   //CResultEntityController<T> results;

   //create query
   /*
   char *zSql;
   va_list ap;
   va_start(ap, queryFormat.c_str());
   zSql = sqlite3_vmprintf(queryFormat.c_str(), ap);
   va_end(ap);

   //exec a first time
   char *zErrMsg = NULL;

   typedef int (CSQLiteDatabaseProvider::* Callback)( void*, int, char**, char** );

   Callback f = (Callback)boost::bind(&CSQLiteDatabaseProvider::handleOneRow, this, _1, _2, _3, _4);
   //   f = std::bind1st( std::mem_fun(&CSQLiteDatabaseProvider::handleOneRow), this);

   int rc = sqlite3_exec(m_pDatabaseHandler, zSql,  f, &results, zErrMsg);

   //if SQLITE_SCHEMA error, retry one time
   if( zErrMsg )
   {
      fprintf(stdout,"%s: query failed: %s - %s\n", zFile, zSql, zErrMsg);
   }

   //free
   sqlite3_free(zErrMsg);
   sqlite3_free(zSql);

   return results.getResults();*/
}

int CSQLiteDatabaseProvider::handleOneRow(void* params ,int columnCount,char** columnValues,char** columnNames)
{
   /*
CResultEntityController<IRowHandler>  * pResults = (CResultEntityController<IRowHandler>  *)params;
	
//create map of <columnName, columnValue>
std::map<std::string, std::string> columnsWithValues;
for(int i=0; i<columnCount; i++)
{
	columnsWithValues.insert(std::make_pair(columnNames[i], sqlite3_mprintf("%s",columnValues[i] ? columnValues[i] : "")));
}
	
//make entity parse result
pResults->createNewEntity()->HandleRow(columnsWithValues);
*/
   throw CNotImplementedException();
}
