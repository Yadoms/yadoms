#include "stdafx.h"

#include "SQLiteDataProvider.h"
#include "SQLiteHardwareRequester.h"
#include "sqlite3.h"
#include "tools/Exceptions/NotImplementedException.h"
#include "adapters/GenericAdapter.h"
#include <boost/filesystem.hpp>

#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

CSQLiteDataProvider::CSQLiteDataProvider(const std::string & dbFile)
   :m_dbFile(dbFile), m_pDatabaseHandler(NULL)
{
}

CSQLiteDataProvider::~CSQLiteDataProvider()
{
   BOOST_ASSERT(m_pDatabaseHandler == NULL);
}


// IDatabaseProvider implementation
bool CSQLiteDataProvider::load()
{
   BOOST_LOG_TRIVIAL(info) << "Load database";

   bool result = false;

   try
   {
      if ( boost::filesystem::exists( m_dbFile.c_str() ) )
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
      else
      {
         BOOST_LOG_TRIVIAL(error) << "Database file is not found : " << m_dbFile << std::endl;
         result = false;
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



void CSQLiteDataProvider::unload()
{
   sqlite3_close(m_pDatabaseHandler);  
}

// [END] IHardwarePlugin implementation


void CSQLiteDataProvider::loadRequesters()
{
   m_hardwareRequester.reset(new CSQLiteHardwareRequester(this));
}


int CSQLiteDataProvider::queryStatement(const std::string & queryFormat, ...)
{
   throw CNotImplementedException();
}

CSQLiteDataProvider::QueryRow CSQLiteDataProvider::querySingleLine(const std::string & queryFormat, ...)
{
   throw CNotImplementedException();
}


CSQLiteDataProvider::QueryResults CSQLiteDataProvider::query(const std::string & queryFormat, ...)
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







