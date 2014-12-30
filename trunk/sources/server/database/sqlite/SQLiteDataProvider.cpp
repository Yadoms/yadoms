#include "stdafx.h"

#include "SQLiteDataProvider.h"
#include "requesters/Plugin.h"
#include "requesters/Configuration.h"
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

namespace database {
   namespace sqlite {

      CSQLiteDataProvider::CSQLiteDataProvider(const std::string & dbFile)
         :m_dbFile(dbFile), m_pDatabaseHandler(NULL)
      {
         //check that library is compiled with THREAD_SAFE= 1 (ensure that it is full mutex access)
         if (sqlite3_threadsafe() != 1)
            throw shared::exception::CNotSupported("SQLite3 non threadsafe");
      }

      CSQLiteDataProvider::~CSQLiteDataProvider()
      {
         unload();
      }


      // IDatabaseProvider implementation
      bool CSQLiteDataProvider::load()
      {
         YADOMS_LOG(information) << "Load database";

         bool result = false;

         try
         {
            if (!boost::filesystem::exists(m_dbFile.c_str()))
            {
               YADOMS_LOG(information) << "Database file is not found : " << m_dbFile;
               YADOMS_LOG(information) << "Yadoms will create a blank one";
            }

            int rc = sqlite3_open(m_dbFile.c_str(), &m_pDatabaseHandler);

            if (rc)
            {
               YADOMS_LOG(fatal) << "Fail to load database: " << sqlite3_errmsg(m_pDatabaseHandler);
               sqlite3_close(m_pDatabaseHandler);
               result = false;
            }
            else
            {
               //extended sql engine
               registerExtendedFunctions();

               //db loaded with succes, create the SQLiteRequester (can execute sql queries) 
               m_databaseRequester.reset(new CSQLiteRequester(m_pDatabaseHandler));

               //check for update
               YADOMS_LOG(information) << "Check for database update...";

               //get the database version
               tools::CVersion currentVersion;

               try
               {
                  CQuery qVersion;
                  qVersion.Select(CConfigurationTable::getValueColumnName()).
                     From(CConfigurationTable::getTableName()).
                     Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_EQUAL, "Database").
                     And(CConfigurationTable::getNameColumnName(), CQUERY_OP_EQUAL, "Version");

                  adapters::CSingleValueAdapter<std::string> adapter;
                  m_databaseRequester->queryEntities<std::string>(&adapter, qVersion);
                  std::vector<const std::string> results = adapter.getResults();


                  if (results.size() >= 1)
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

               versioning::CSQLiteVersionUpgraderFactory::GetUpgrader()->checkForUpgrade(m_databaseRequester, currentVersion);

               //create entities requester (high level querier)
               loadRequesters();

               YADOMS_LOG(information) << "Load database with success";
               result = true;

            }
         }
         catch (versioning::CSQLiteVersionException & exc)
         {
            YADOMS_LOG(error) << "Fail to load database (upgrade error) : " << std::endl << exc.what();
            if (m_pDatabaseHandler != NULL)
               sqlite3_close(m_pDatabaseHandler);
            result = false;
         }
         catch (...)
         {
            YADOMS_LOG(error) << "Unknown exception while loading database";
            if (m_pDatabaseHandler != NULL)
               sqlite3_close(m_pDatabaseHandler);
            result = false;
         }
         return result;
      }


      void isodate(sqlite3_context *context, int argc, sqlite3_value **argv){
         assert(argc == 1);
         switch (sqlite3_value_type(argv[0]))
         {
            case SQLITE_TEXT: 
            {
               std::string sVal = reinterpret_cast<const char*>(sqlite3_value_text(argv[0]));
               sVal.insert(13, ":");
               sVal.insert(11, ":");
               sVal.insert(6, "-");
               sVal.insert(4, "-");
               
               char *buf = static_cast<char *>(malloc(sizeof(char)*(sVal.size())));
               memcpy(buf, sVal.c_str(), sVal.size());
               sqlite3_result_text(context, buf, sVal.size(), free);
               break;
            }

            default: 
               sqlite3_result_null(context);
               break;
            
         }
      }

      void CSQLiteDataProvider::registerExtendedFunctions()
      {
         sqlite3_create_function(m_pDatabaseHandler, "isodate", 1, SQLITE_UTF8, NULL, &isodate, NULL, NULL);
      }

      void CSQLiteDataProvider::unload()
      {
         if (m_pDatabaseHandler != NULL)
            sqlite3_close(m_pDatabaseHandler);
      }

      void CSQLiteDataProvider::loadRequesters()
      {
         m_pluginRequester.reset(new requesters::CPlugin( m_databaseRequester));
         m_configurationRequester.reset(new requesters::CConfiguration( m_databaseRequester));
         m_deviceRequester.reset(new requesters::CDevice( m_databaseRequester));
			m_keywordRequester.reset(new requesters::CKeyword(this, m_databaseRequester));
         m_pageRequester.reset(new requesters::CPage( m_databaseRequester));
         m_widgetRequester.reset(new requesters::CWidget( m_databaseRequester));
         m_pluginEventLoggerRequester.reset(new requesters::CPluginEventLogger( m_databaseRequester));
         m_eventLoggerRequester.reset(new requesters::CEventLogger( m_databaseRequester));
         m_acquisitionRequester.reset(new requesters::CAcquisition(this, m_databaseRequester));
         m_ruleRequester.reset(new requesters::CRule(m_databaseRequester));
         m_recipientRequester.reset(new requesters::CRecipient(m_databaseRequester));
      }



      boost::shared_ptr<ITransactionalProvider> CSQLiteDataProvider::getTransactionalEngine()
      {
         if (!m_databaseRequester->transactionIsAlreadyCreated())
            return m_databaseRequester;
         return boost::shared_ptr<ITransactionalProvider>();
      }

      void CSQLiteDataProvider::backupData(const std::string & backupLocation, ProgressFunc reporter)
      {
         sqlite3 *pFile;             /* Database connection opened on zFilename */

         /* Open the database file identified by zFilename. */
         int rc = sqlite3_open(backupLocation.c_str(), &pFile);
         if (rc == SQLITE_OK)
         {
            /* Open the sqlite3_backup object used to accomplish the transfer */
            sqlite3_backup *pBackup = sqlite3_backup_init(pFile, "main", m_pDatabaseHandler, "main");
            if (pBackup)
            {

               /* Each iteration of this loop copies 20 database pages from database
               ** pDb to the backup database. If the return value of backup_step()
               ** indicates that there are still further pages to copy, sleep for
               ** 250 ms before repeating. */
               do
               {
                  rc = sqlite3_backup_step(pBackup, -1);
                  if (reporter)
                  {
                     reporter(
                        sqlite3_backup_remaining(pBackup),
                        sqlite3_backup_pagecount(pBackup),
                        "Backup in progress"
                        );
                  }

                  if (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED)
                  {
                     sqlite3_sleep(250);
                  }
               } while (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED);

               /* Release resources allocated by backup_init(). */
               (void)sqlite3_backup_finish(pBackup);
            }
            rc = sqlite3_errcode(pFile);
         }

         /* Close the database connection opened on database file zFilename
         ** and return the result of this function. */
         (void)sqlite3_close(pFile);


         if (rc != SQLITE_OK)
         {
            throw shared::exception::CException(sqlite3_errstr(rc));
         }
      }

   } //namespace sqlite
} //namespace database 

