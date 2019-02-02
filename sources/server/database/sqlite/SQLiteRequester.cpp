#include "stdafx.h"

#include "SQLiteRequester.h"
#include "database/common/adapters/GenericAdapter.h"
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "SQLiteSystemTables.h"
#include "database/DatabaseException.hpp"
#include "SQLiteResultHandler.h"
#include "SQLiteTableCreationScriptProvider.h"
#include "SQLiteQuery.h"
#include <shared/Log.h>
#include <shared/exception/NullReference.hpp>
#include "i18n/ClientStrings.h"

namespace database
{
   namespace sqlite
   {
      //---------------------------
      // Maximum tries
      //---------------------------
      int CSQLiteRequester::m_maxTries = 3;


      CSQLiteRequester::CSQLiteRequester(const std::string& dbFile)
         : m_pDatabaseHandler(nullptr),
           m_dbFile(dbFile),
           m_bOneTransactionActive(false)
      {
      }

      CSQLiteRequester::~CSQLiteRequester()
      {
      }

      void CSQLiteRequester::initialize()
      {
         YADOMS_LOG(information) << "Initialize SQLite database";
         try
         {
            if (!boost::filesystem::exists(m_dbFile.c_str()))
            {
               YADOMS_LOG(information) << "Database file is not found : " << m_dbFile;
               YADOMS_LOG(information) << "Yadoms will create a blank one";
            }

            const auto rc = sqlite3_open(m_dbFile.c_str(), &m_pDatabaseHandler);
            if (rc)
            {
               const std::string error = sqlite3_errmsg(m_pDatabaseHandler);
               throw CDatabaseException(error);
            }

            //extended sql engine
            registerExtendedFunctions();
         }
         catch (std::exception& exc)
         {
            YADOMS_LOG(error) << "Fail to load SQLite database : " << std::endl << exc.what();
            if (m_pDatabaseHandler != nullptr)
               sqlite3_close(m_pDatabaseHandler);
            throw;
         }
         catch (...)
         {
            YADOMS_LOG(error) << "Unknown exception while loading SQLite database";
            if (m_pDatabaseHandler != nullptr)
               sqlite3_close(m_pDatabaseHandler);
            throw;
         }
      }

      void CSQLiteRequester::finalize()
      {
         //close database access
         if (m_pDatabaseHandler != nullptr)
            sqlite3_close(m_pDatabaseHandler);
      }

      shared::CDataContainer CSQLiteRequester::getInformation()
      {
         shared::CDataContainer results;
         results.set("type", "SQLite");
         results.set("version", sqlite3_libversion());
         results.set("size", boost::filesystem::file_size(boost::filesystem::path(m_dbFile)));
         return results;
      }

      void isodate(sqlite3_context* context, int argc, sqlite3_value** argv)
      {
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

               const auto buf = static_cast<char *>(malloc(sizeof(char) * (sVal.size())));
               memcpy(buf, sVal.c_str(), sVal.size());
               sqlite3_result_text(context, buf, sVal.size(), free);
               break;
            }

         default:
            sqlite3_result_null(context);
            break;
         }
      }

      void CSQLiteRequester::registerExtendedFunctions() const
      {
         sqlite3_create_function(m_pDatabaseHandler,
                                 "isodate",
                                 1,
                                 SQLITE_UTF8,
                                 nullptr,
                                 &isodate,
                                 nullptr,
                                 nullptr);
      }


      boost::shared_ptr<common::CQuery> CSQLiteRequester::newQuery()
      {
         return boost::make_shared<CSQLiteQuery>();
      }


      //--------------------------------------------------------------
      /// \Brief		    query for entities (the result is a vector of typed objects, accessible by a call to pAdapter->GetResult())
      /// \param [in]	 adapter:  pointer to the adapter to use to map raw values to a new entity
      /// \param [in]	 querytoExecute: the sql query
      //--------------------------------------------------------------
      void CSQLiteRequester::queryEntities(common::adapters::IResultAdapter* adapter,
                                           const common::CQuery& querytoExecute)
      {
         BOOST_ASSERT(adapter != NULL);

         if (adapter != nullptr)
         {
            try
            {
               auto remainingTries = m_maxTries;
               bool retry;

               do
               {
                  //ensure retry is reset to false
                  retry = false;

                  sqlite3_stmt* stmt;
                  const auto rc = sqlite3_prepare_v2(m_pDatabaseHandler, querytoExecute.c_str(), -1, &stmt, nullptr);

                  if (rc == SQLITE_OK)
                  {
                     const auto handler(boost::make_shared<CSQLiteResultHandler>(stmt));
                     if (!adapter->adapt(handler))
                     {
                        YADOMS_LOG(error) << "Fail to adapt values";
                     }
                     sqlite3_finalize(stmt);
                  }
                  else
                  {
                     if (rc == SQLITE_LOCKED && remainingTries > 1)
                     {
                        //the database is locked (likely by another program)
                        //just sleep some time and wish it is not locked anymore
                        boost::this_thread::sleep(boost::posix_time::milliseconds(500));

                        //retry
                        retry = true;
                     }
                     else
                     {
                        YADOMS_LOG(error) << "Fail to execute query : " << sqlite3_errmsg(m_pDatabaseHandler);
                     }
                  }
               }
               while ((--remainingTries) > 0 && retry);
            }
            catch (std::exception& ex)
            {
               YADOMS_LOG(error) << "Exception: Fail to execute query : " << ex.what();
            }
            catch (...)
            {
               YADOMS_LOG(error) << "Unknown exception: Fail to execute query";
            }
         }
         else
         {
            //throw exception
            throw shared::exception::CNullReference("adapter");
         }
      }


      int CSQLiteRequester::queryStatement(const common::CQuery& querytoExecute,
                                           bool throwIfFails)
      {
         BOOST_ASSERT(querytoExecute.GetQueryType() != common::CQuery::kNotYetDefined);
         BOOST_ASSERT(querytoExecute.GetQueryType() != common::CQuery::kSelect);

         //execute the query
         char* zErrMsg = nullptr;
         int remainingTries = m_maxTries;
         bool retry;

         do
         {
            //execute query
            const auto rc = sqlite3_exec(m_pDatabaseHandler,
                                   querytoExecute.c_str(),
                                   nullptr,
                                   nullptr,
                                   &zErrMsg);

            //if an error occurred
            //then   log it
            //       if error is known like "database locked" then retry the query
            if (rc != SQLITE_OK)
            {
               //make a copy of the err message
               const std::string errMessage(zErrMsg);

               //log the message
               YADOMS_LOG(error) << "Query failed : " << std::endl << "Query: " << querytoExecute.str() << std::endl << "Error : " << zErrMsg;

               //free allocated memory by sqlite
               sqlite3_free(zErrMsg);

               //if it is a database locked error, just wait and retry
               if (rc == SQLITE_LOCKED && remainingTries > 1)
               {
                  YADOMS_LOG(debug) << "Query execution retry !!";

                  //the database is locked (likely by another program)
                  //just sleep some time and wish it is not locked anymore
                  boost::this_thread::sleep(boost::posix_time::milliseconds(500));

                  //retry
                  retry = true;
               }
               else
               {
                  if (throwIfFails)
                     throw CDatabaseException(errMessage, fromSQLiteReturnCode(rc));
                  return -1;
               }
            }
            else
            {
               //query is successfull
               retry = false;
            }
         }
         while ((--remainingTries) > 0 && retry);

         return sqlite3_changes(m_pDatabaseHandler);
      }


      int CSQLiteRequester::queryCount(const common::CQuery& querytoExecute)
      {
         BOOST_ASSERT(querytoExecute.GetQueryType() != common::CQuery::kNotYetDefined);
         BOOST_ASSERT(querytoExecute.GetQueryType() == common::CQuery::kSelect);

         common::adapters::CSingleValueAdapter<int> countAdapter;
         queryEntities(&countAdapter, querytoExecute);

         if (countAdapter.getResults().size() >= 1)
            return countAdapter.getResults()[0];
         return -1;
      }

      CSQLiteRequester::QueryRow CSQLiteRequester::querySingleLine(const common::CQuery& querytoExecute)
      {
         BOOST_ASSERT(querytoExecute.GetQueryType() != common::CQuery::kNotYetDefined);
         BOOST_ASSERT(querytoExecute.GetQueryType() == common::CQuery::kSelect);

         QueryResults results = query(querytoExecute);
         if (results.size() >= 1)
            return results[0];

         return QueryRow(); //returns empty data
      }


      CSQLiteRequester::QueryResults CSQLiteRequester::query(const common::CQuery& querytoExecute)
      {
         BOOST_ASSERT(querytoExecute.GetQueryType() != common::CQuery::kNotYetDefined);
         BOOST_ASSERT(querytoExecute.GetQueryType() == common::CQuery::kSelect);

         common::adapters::CGenericAdapter genericAdapter;
         queryEntities(&genericAdapter, querytoExecute);
         return genericAdapter.getResults();
      }

      bool CSQLiteRequester::transactionSupport()
      {
         return true;
      }

      void CSQLiteRequester::transactionBegin()
      {
         if (!m_bOneTransactionActive)
         {
            sqlite3_exec(m_pDatabaseHandler, "BEGIN", nullptr, nullptr, nullptr);
            m_bOneTransactionActive = true;
         }
      }

      void CSQLiteRequester::transactionCommit()
      {
         if (m_bOneTransactionActive)
         {
            sqlite3_exec(m_pDatabaseHandler, "COMMIT", nullptr, nullptr, nullptr);
            m_bOneTransactionActive = false;
         }
      }

      void CSQLiteRequester::transactionRollback()
      {
         if (m_bOneTransactionActive)
         {
            sqlite3_exec(m_pDatabaseHandler, "ROLLBACK", nullptr, nullptr, nullptr);
            m_bOneTransactionActive = false;
         }
      }

      bool CSQLiteRequester::transactionIsAlreadyCreated()
      {
         return m_bOneTransactionActive;
      }

      bool CSQLiteRequester::checkTableExists(const common::CDatabaseTable& tableName)
      {
         //check that table Configuration exists
         CSQLiteQuery sCheckForTableExists;
         sCheckForTableExists.SelectCount().
                             From(CSqliteMasterTable::getTableName()).
                             Where(CSqliteMasterTable::getTypeColumnName(), CQUERY_OP_EQUAL, SQLITEMASTER_TABLE).
                             And(CSqliteMasterTable::getNameColumnName(), CQUERY_OP_EQUAL, tableName.GetName());
         const auto count = queryCount(sCheckForTableExists);
         return (count == 1);
      }


      bool CSQLiteRequester::dropTableIfExists(const common::CDatabaseTable& tableName)
      {
         if (checkTableExists(tableName))
         {
            CSQLiteQuery drop;
            queryStatement(drop.DropTable(tableName));
            return !checkTableExists(tableName);
         }
         return true;
      }


      bool CSQLiteRequester::createTableIfNotExists(const common::CDatabaseTable& tableName,
                                                    const std::string& tableScript)
      {
         if (!checkTableExists(tableName))
         {
            queryStatement(common::CQuery::CustomQuery(tableScript, common::CQuery::kCreate));
            return checkTableExists(tableName);
         }
         return true;
      }

      bool CSQLiteRequester::addTableColumn(const common::CDatabaseTable& tableName, const std::string& columnDef)
      {
         if (checkTableExists(tableName))
         {
            CSQLiteQuery alter;
            queryStatement(alter.AddTableColumn(tableName, columnDef));
            return true;
         }
         return false;
      }

      void CSQLiteRequester::createIndex(const common::CDatabaseTable& tableName,
                                         const std::string& indexScript)
      {
         queryStatement(common::CQuery::CustomQuery(indexScript, common::CQuery::kCreate));
      }


      bool CSQLiteRequester::backupSupported() const
      {
         return true;
      }

      uintmax_t CSQLiteRequester::backupNeededSpace() const
      {
         return std::filesystem::file_size(m_dbFile);
      }

      void CSQLiteRequester::backupData(const std::string & backupFolder, ProgressFunc reporter) const
      {
         int rc;
         int currentTry = 0;

         do
         {
            rc = doBackup(backupFolder, reporter);
            if (rc == SQLITE_BUSY || rc == SQLITE_LOCKED)
            {
               YADOMS_LOG(warning) << "Database/table is locked. Waiting for 10 seconds and retry";
               //sleep for 10 sec and retry
               boost::this_thread::sleep(boost::posix_time::seconds(10)); 
            }
            currentTry++;
         }
         while(rc != SQLITE_OK && rc != SQLITE_DONE && currentTry<=3);

         if (rc != SQLITE_OK && rc != SQLITE_DONE)
         {
            if (reporter)
               reporter(0, 100, i18n::CClientStrings::DatabaseBackupFail, sqlite3_errstr(rc));
            throw shared::exception::CException(sqlite3_errstr(rc));
         }
      }

      int CSQLiteRequester::doBackup(const std::string & backupFolder, ProgressFunc reporter) const
      {
         sqlite3* pFile; /* Database connection opened on zFilename */

         auto backupfile = backupFolder + "/" + "yadoms.db3";

         //remove backup file if already exists
         if (boost::filesystem::exists(backupfile))
         {
            boost::filesystem::remove(backupfile);
         }

         // Open the database file identified by zFilename.
         auto rc = sqlite3_open(backupfile.c_str(), &pFile);
         if (rc == SQLITE_OK)
         {
            // Open the sqlite3_backup object used to accomplish the transfer
            const auto pBackup = sqlite3_backup_init(pFile, "main", m_pDatabaseHandler, "main");
            if (pBackup)
            {
               //do the backup
               (void)sqlite3_backup_step(pBackup, -1);
               (void)sqlite3_backup_finish(pBackup);
            }
            rc = sqlite3_errcode(pFile);

            //report
            if (reporter)
            {
               if (rc == SQLITE_OK || rc == SQLITE_DONE)
               {
                  reporter(sqlite3_backup_remaining(pBackup), sqlite3_backup_pagecount(pBackup), i18n::CClientStrings::DatabaseBackupSuccess, "");
               }
               else
               {
                  reporter(sqlite3_backup_remaining(pBackup), sqlite3_backup_pagecount(pBackup), i18n::CClientStrings::DatabaseBackupFail, sqlite3_errstr(rc));
               }
            }

         }

         // Close the database connection opened on database file zFilename
         // and return the result of this function.
         (void)sqlite3_close(pFile);

         return rc;
      }

      CDatabaseException::EDatabaseReturnCodes CSQLiteRequester::fromSQLiteReturnCode(int rc)
      {
         switch(rc)
         {
         case SQLITE_OK:
            return CDatabaseException::kOk;
         case SQLITE_CONSTRAINT:
            return CDatabaseException::kConstraintViolation;
         default:
            return CDatabaseException::kError;
         }
      }

      void CSQLiteRequester::vacuum()
      {
         //we ensure that no transaction is active
         //if a transaction is active, just wait for the transaction to end (with timetout)
         auto waitLoopCount = 0;
         const auto waitPeriodMs = 200;
         const auto maxLoopWait = 2 * 60 * 1000 / waitPeriodMs; // 2 minutes

         while (m_bOneTransactionActive && waitLoopCount < maxLoopWait)
         {
            boost::this_thread::sleep(boost::posix_time::milliseconds(waitPeriodMs));
            waitLoopCount++;
         }

         //if no timeout, execute vacuum
         if (waitLoopCount >= maxLoopWait || m_bOneTransactionActive)
         YADOMS_LOG(warning) << "Fail to execute vacuum, one transaction is still active";
         else
            queryStatement(CSQLiteQuery().Vacuum());
      }

      boost::shared_ptr<ITableCreationScriptProvider> CSQLiteRequester::getTableCreationScriptProvider()
      {
         return boost::make_shared<CSQLiteTableCreationScriptProvider>();
      }

      bool CSQLiteRequester::supportInsertOrUpdateStatement()
      {
         return true;
      }
   } //namespace sqlite
} //namespace database 


