#pragma once

#include <boost/log/trivial.hpp>
#include "SQLiteDatabaseProvider.h"
#include "SQLiteHardwareRequester.h"
#include "sqlite3.h"
#include "tools/Exceptions/NotImplementedException.h"
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include <boost/optional.hpp>

#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

CSQLiteDatabaseProvider::CSQLiteDatabaseProvider(const std::string & dbFile)
   :m_dbFile(dbFile), m_pDatabaseHandler(NULL)
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
      int rc = sqlite3_open(m_dbFile.c_str(), &m_pDatabaseHandler);
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

CSQLiteDatabaseProvider::QueryRow CSQLiteDatabaseProvider::querySingleLine(const std::string & queryFormat, ...)
{
   throw CNotImplementedException();
}

int handleIt(void* params ,int columnCount,char** columnValues,char** columnNames)
{
   CSQLiteDatabaseProvider::QueryResults  * pResults = (CSQLiteDatabaseProvider::QueryResults*)params;
   if(pResults != NULL)
   {
      CSQLiteDatabaseProvider::QueryRow row;
      for(int i=0; i<columnCount; i++)
      {
         row.push_back(columnValues[i]);
      } 

      pResults->push_back(row);
   }
   return 0;
}

template<unsigned ID,typename Functor>
boost::optional<Functor> &get_local()
{
    static boost::optional<Functor> local;
    return local;
}

template<unsigned ID,typename Functor>
typename Functor::result_type wrapper()
{
    return get_local<ID,Functor>().get()();
}

template<typename ReturnType>
struct Func
{
    typedef ReturnType (*type)();
};

template<unsigned ID,typename Functor>
typename Func<typename Functor::result_type>::type get_wrapper(Functor f)
{
    (get_local<ID,Functor>()) = f;
    return wrapper<ID,Functor>;
}


CSQLiteDatabaseProvider::QueryResults CSQLiteDatabaseProvider::query(const std::string & queryFormat, ...)
{
   QueryResults results;

   //create query
   char *zSql;
   va_list ap;
   va_start(ap, queryFormat);
   zSql = sqlite3_vmprintf(queryFormat.c_str(), ap);
   va_end(ap);

   //exec a first time
   char *zErrMsg = NULL;

   //not working with boost
   //typedef boost::function<int (void*, int, char**, char** )> PFunctor;
   //PFunctor f = boost::bind(&CSQLiteDatabaseProvider::handleOneRow, this, _1, _2, _3, _4);
   //int rc = sqlite3_exec(m_pDatabaseHandler, zSql, f.target<int (void*, int, char**, char** )>() , &results, &zErrMsg);

   int rc = sqlite3_exec(m_pDatabaseHandler, zSql,  &handleIt, &results, &zErrMsg);

   //if SQLITE_SCHEMA error, retry one time
   if( zErrMsg )
   {
      BOOST_LOG_TRIVIAL(error) << "Query failed : " << std::endl << "Query: " << zSql << std::endl << "Error : " << zErrMsg;
   }

   //free
   if(zErrMsg)
      sqlite3_free(zErrMsg);

   if(zSql)
      sqlite3_free(zSql);

   return results;
}

int CSQLiteDatabaseProvider::handleOneRow(void* params ,int columnCount,char** columnValues,char** columnNames)
{
   QueryResults  * pResults = (QueryResults*)params;
   if(pResults != NULL)
   {
      QueryRow row;
      for(int i=0; i<columnCount; i++)
      {
         row.push_back(columnValues[i]);
      } 

      pResults->push_back(row);
   }
   return 0;
}
