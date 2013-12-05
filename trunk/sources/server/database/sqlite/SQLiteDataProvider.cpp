#include "stdafx.h"

#include "SQLiteDataProvider.h"
#include "SQLiteHardwareRequester.h"
#include "sqlite3.h"
#include <boost/filesystem.hpp>
#include "SQLiteRequester.h"
#include "tools/Log.h"


CSQLiteDataProvider::CSQLiteDataProvider(const std::string & dbFile)
   :m_dbFile(dbFile)
{
}

CSQLiteDataProvider::~CSQLiteDataProvider()
{
}


// IDatabaseProvider implementation
bool CSQLiteDataProvider::load()
{
   YADOMS_LOG(info) << "Load database";

   bool result = false;

   try
   {
      if ( boost::filesystem::exists( m_dbFile.c_str() ) )
      {
         int rc = sqlite3_open(m_dbFile.c_str(), &m_pDatabaseHandler);

         if(rc)
         {
            YADOMS_LOG(fatal) << "Fail to load database: " << sqlite3_errmsg(m_pDatabaseHandler);
            sqlite3_close(m_pDatabaseHandler);
            result = false;
         }
         else
         {
            m_databaseRequester.reset(new CSQLiteRequester(m_pDatabaseHandler));
            //db loaded with succes, create requesters
            loadRequesters();

            YADOMS_LOG(info) << "Load database with success";
            result = true;

         }
      }
      else
      {
         YADOMS_LOG(error) << "Database file is not found : " << m_dbFile << std::endl;
         result = false;
      }


   }
   catch(...)
   {
      YADOMS_LOG(error) << "Unknow exception while loading database";
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
   m_hardwareRequester.reset(new CSQLiteHardwareRequester(*this, m_databaseRequester));
}





