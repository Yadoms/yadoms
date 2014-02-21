#include "stdafx.h"

#include "SQLiteDataProvider.h"
#include "requesters/SQLiteHardwareRequester.h"
#include "requesters/SQLiteConfigurationRequester.h"
#include "sqlite3.h"
#include <boost/filesystem.hpp>
#include "SQLiteRequester.h"
#include <shared/Log.h>
#include "versioning/SQLiteVersionUpgraderFactory.h"
#include "versioning/SQLiteVersionException.h"
#include <shared/Exceptions/NotSupportedException.hpp>
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "tools/Version.h"

namespace server { 
namespace database { 
namespace sqlite { 

CSQLiteDataProvider::CSQLiteDataProvider(const std::string & dbFile)
   :m_dbFile(dbFile), m_pDatabaseHandler(NULL)
{
   //check that library is comiled with THREAD_SAFE= 1 (ensure that it is full mutex access)
   if(sqlite3_threadsafe() != 1)
      throw CNotSupportedException("SQLite3 non threadsafe");
}

CSQLiteDataProvider::~CSQLiteDataProvider()
{
   unload();
}


// IDatabaseProvider implementation
bool CSQLiteDataProvider::load()
{
   YADOMS_LOG(info) << "Load database";

   bool result = false;

   try
   {
      if ( !boost::filesystem::exists( m_dbFile.c_str() ) )
      {
         YADOMS_LOG(info) << "Database file is not found : " << m_dbFile;
         YADOMS_LOG(info) << "Yadoms will create a blank one";
      }

      int rc = sqlite3_open(m_dbFile.c_str(), &m_pDatabaseHandler);

      if(rc)
      {
         YADOMS_LOG(fatal) << "Fail to load database: " << sqlite3_errmsg(m_pDatabaseHandler);
         sqlite3_close(m_pDatabaseHandler);
         result = false;
      }
      else
      {
         //db loaded with succes, create the SQLiteRequester (can execute sql queries) 
         m_databaseRequester.reset(new CSQLiteRequester(m_pDatabaseHandler));

         //check for update
         YADOMS_LOG(info) << "Check for database update...";

         //get the database version
         CVersion currentVersion;

         try
         {
            CQuery qVersion;
            qVersion.Select(CConfigurationTable::getValueColumnName()).
               From(CConfigurationTable::getTableName()).
               Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_EQUAL, "Database").
               And(CConfigurationTable::getNameColumnName(), CQUERY_OP_EQUAL, "Version");

            server::database::sqlite::adapters::CSingleValueAdapter<std::string> adapter;
            m_databaseRequester->queryEntities<std::string>(&adapter, qVersion);
            std::vector<std::string> results = adapter.getResults();
         
         
            if(results.size() >= 1)
            {
               currentVersion = CVersion(results[0]);
            }
         }
         catch (std::exception & ex)
         {
            YADOMS_LOG(warning) << "Fail to get version of database : " << ex.what();
         }
         catch (...)
         {
            YADOMS_LOG(warning) << "Fail to get version of database : Unkonown exception";
         }
         
         server::database::sqlite::versioning::CSQLiteVersionUpgraderFactory::GetUpgrader()->checkForUpgrade(m_databaseRequester, currentVersion);

         //create entities requester (high level querier)
         loadRequesters();

         YADOMS_LOG(info) << "Load database with success";
         result = true;

      }
   }
   catch(server::database::sqlite::versioning::CSQLiteVersionException & exc)
   {
      YADOMS_LOG(error) << "Fail to load database (upgrade error) : " << std::endl << exc.what();
      if(m_pDatabaseHandler != NULL)
         sqlite3_close(m_pDatabaseHandler);
      result = false;
   }
   catch(...)
   {
      YADOMS_LOG(error) << "Unknown exception while loading database";
      if(m_pDatabaseHandler != NULL)
         sqlite3_close(m_pDatabaseHandler);
      result = false;
   }
   return result;
}



void CSQLiteDataProvider::unload()
{
   if(m_pDatabaseHandler != NULL)
      sqlite3_close(m_pDatabaseHandler);  
}

void CSQLiteDataProvider::loadRequesters()
{
   m_hardwareRequester.reset(new server::database::sqlite::requesters::CSQLiteHardwareRequester(*this, m_databaseRequester));
   m_configurationRequester.reset(new server::database::sqlite::requesters::CSQLiteConfigurationRequester(*this, m_databaseRequester));
   m_deviceRequester.reset(new server::database::sqlite::requesters::CSQLiteDeviceRequester(*this, m_databaseRequester));
   m_acquisitionRequester.reset(new server::database::sqlite::requesters::CSQLiteAcquisitionRequester(*this, m_databaseRequester));
   m_keywordRequester.reset(new server::database::sqlite::requesters::CSQLiteKeywordRequester(*this, m_databaseRequester));
   m_pageRequester.reset(new server::database::sqlite::requesters::CSQLitePageRequester(*this, m_databaseRequester));
   m_widgetRequester.reset(new server::database::sqlite::requesters::CSQLiteWidgetRequester(*this, m_databaseRequester));
   m_hardwareEventLoggerRequester.reset(new server::database::sqlite::requesters::CSQLiteHardwareEventLoggerRequester(*this, m_databaseRequester));
   m_eventLoggerRequester.reset(new server::database::sqlite::requesters::CSQLiteEventLoggerRequester(*this, m_databaseRequester));
}



boost::shared_ptr<server::database::ITransactionalProvider> CSQLiteDataProvider::getTransactionalEngine() 
{
   if(!m_databaseRequester->transactionIsAlreadyCreated())
      return m_databaseRequester; 
   return boost::shared_ptr<ITransactionalProvider>();
}

} //namespace sqlite
} //namespace database 
} //namespace server
