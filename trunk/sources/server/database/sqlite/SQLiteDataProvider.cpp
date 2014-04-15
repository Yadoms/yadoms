#include "stdafx.h"

#include "SQLiteDataProvider.h"
#include "requesters/SQLitePluginRequester.h"
#include "requesters/SQLiteConfigurationRequester.h"
#include "sqlite3.h"
#include <boost/filesystem.hpp>
#include "SQLiteRequester.h"
#include <shared/Log.h>
#include "versioning/SQLiteVersionUpgraderFactory.h"
#include "versioning/SQLiteVersionException.h"
#include <shared/exception/NotSupported.hpp>
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "tools/Version.h"


namespace database { namespace sqlite { 

   CSQLiteDataProvider::CSQLiteDataProvider(const std::string & dbFile)
      :m_dbFile(dbFile), m_pDatabaseHandler(NULL)
   {
      //check that library is compiled with THREAD_SAFE= 1 (ensure that it is full mutex access)
      if(sqlite3_threadsafe() != 1)
         throw shared::exception::CNotSupported("SQLite3 non threadsafe");
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
            tools::CVersion currentVersion;

            try
            {
               CQuery qVersion;
               qVersion.Select(CConfigurationTable::getValueColumnName()).
                  From(CConfigurationTable::getTableName()).
                  Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_EQUAL, "Database").
                  And(CConfigurationTable::getNameColumnName(), CQUERY_OP_EQUAL, "Version");

               database::sqlite::adapters::CSingleValueAdapter<std::string> adapter;
               m_databaseRequester->queryEntities<std::string>(&adapter, qVersion);
               std::vector<std::string> results = adapter.getResults();


               if(results.size() >= 1)
               {
                  currentVersion = tools::CVersion(results[0]);
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

            database::sqlite::versioning::CSQLiteVersionUpgraderFactory::GetUpgrader()->checkForUpgrade(m_databaseRequester, currentVersion);

            //create entities requester (high level querier)
            loadRequesters();

            YADOMS_LOG(info) << "Load database with success";
            result = true;

         }
      }
      catch(database::sqlite::versioning::CSQLiteVersionException & exc)
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
      m_pluginRequester.reset(new database::sqlite::requesters::CSQLitePluginRequester(*this, m_databaseRequester));
      m_configurationRequester.reset(new database::sqlite::requesters::CSQLiteConfigurationRequester(*this, m_databaseRequester));
      m_deviceRequester.reset(new database::sqlite::requesters::CSQLiteDeviceRequester(*this, m_databaseRequester));
      m_keywordRequester.reset(new database::sqlite::requesters::CSQLiteKeywordRequester(*this, m_databaseRequester));
      m_pageRequester.reset(new database::sqlite::requesters::CSQLitePageRequester(*this, m_databaseRequester));
      m_widgetRequester.reset(new database::sqlite::requesters::CSQLiteWidgetRequester(*this, m_databaseRequester));
      m_pluginEventLoggerRequester.reset(new database::sqlite::requesters::CSQLitePluginEventLoggerRequester(*this, m_databaseRequester));
      m_eventLoggerRequester.reset(new database::sqlite::requesters::CSQLiteEventLoggerRequester(*this, m_databaseRequester));
      m_messageRequester.reset(new database::sqlite::requesters::CSQLiteMessageRequester(*this, m_databaseRequester));
   }



   boost::shared_ptr<database::ITransactionalProvider> CSQLiteDataProvider::getTransactionalEngine() 
   {
      if(!m_databaseRequester->transactionIsAlreadyCreated())
         return m_databaseRequester; 
      return boost::shared_ptr<ITransactionalProvider>();
   }

   void CSQLiteDataProvider::backupData(const std::string & backupLocation, ProgressFunc reporter)
   {
      int rc;                     /* Function return code */
      sqlite3 *pFile;             /* Database connection opened on zFilename */
      sqlite3_backup *pBackup;    /* Backup handle used to copy data */

      /* Open the database file identified by zFilename. */
      rc = sqlite3_open(backupLocation.c_str(), &pFile);
      if( rc==SQLITE_OK )
      {
         /* Open the sqlite3_backup object used to accomplish the transfer */
         pBackup = sqlite3_backup_init(pFile, "main", m_pDatabaseHandler, "main");
         if( pBackup )
         {

            /* Each iteration of this loop copies 5 database pages from database
            ** pDb to the backup database. If the return value of backup_step()
            ** indicates that there are still further pages to copy, sleep for
            ** 250 ms before repeating. */
            do
            {
               rc = sqlite3_backup_step(pBackup, 5);

               if(reporter)
               {
                  reporter(
                     sqlite3_backup_remaining(pBackup),
                     sqlite3_backup_pagecount(pBackup)
                     );
               }
               
               if( rc==SQLITE_OK || rc==SQLITE_BUSY || rc==SQLITE_LOCKED )
               {
                  sqlite3_sleep(250);
               }
            } while( rc==SQLITE_OK || rc==SQLITE_BUSY || rc==SQLITE_LOCKED );

            /* Release resources allocated by backup_init(). */
            (void)sqlite3_backup_finish(pBackup);
         }
         rc = sqlite3_errcode(pFile);
      }

      /* Close the database connection opened on database file zFilename
      ** and return the result of this function. */
      (void)sqlite3_close(pFile);
      
      
      if(rc != SQLITE_OK)
      {
         throw shared::exception::CException(sqlite3_errstr(rc));
      }
   }

} //namespace sqlite
} //namespace database 

