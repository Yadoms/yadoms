#pragma once

#include "sqlite3.h"
#include <shared/exception/NullReference.hpp>
#include <shared/Log.h>
#include "database/common/Query.h"
#include "database/IDatabaseRequester.h"
#include "SQLiteQueryFunctions.h"

namespace database { 
namespace sqlite { 

   class CSQLiteRequester : public IDatabaseRequester
   {
   public:
      explicit CSQLiteRequester(const std::string &dbFile);
      virtual ~CSQLiteRequester();

      // IDatabaseEngine implementation
      virtual void initialize();
      virtual void finalize();
      // [END] IDatabaseEngine implementation

      // IDatabaseRequester implementation
      virtual database::common::CQuery newQuery();
      virtual database::common::CQueryFunctions & queryFunc();
      virtual void queryEntities(database::common::adapters::IResultAdapter * pAdapter, const database::common::CQuery & querytoExecute);
      virtual int queryStatement(const database::common::CQuery & querytoExecute);
      virtual int queryCount(const database::common::CQuery & querytoExecute);
      virtual QueryRow querySingleLine(const database::common::CQuery & querytoExecute);
      virtual QueryResults query(const database::common::CQuery & querytoExecute);
      virtual bool checkTableExists(const std::string & tableName);
      virtual bool dropTableIfExists(const std::string & tableName);
      virtual bool createTableIfNotExists(const std::string & tableName, const std::string & tableScript);
      virtual void createIndex(const std::string & tableName, const std::string & indexScript);
      virtual std::string generateSqlIsoDateFormat(const std::string &columnName);
      virtual std::string coalesce(const std::string & columnName, std::string defaultValue);
      virtual void vacuum();
      virtual boost::shared_ptr<ITableCreationScriptProvider> getTableCreationScriptProvider();
      // [END] IDatabaseRequester implementation

      // ITransactionalProvider implementation
      virtual bool transactionSupport();
      virtual void transactionBegin();
      virtual void transactionCommit();
      virtual void transactionRollback();
      virtual bool transactionIsAlreadyCreated();
      // [END] ITransactionalProvider implementation

      // IDataBackup implementation
      virtual bool backupSupported(); 
      virtual void backupData(const std::string & backupLocation, IDataBackup::ProgressFunc reporter);
      // [END] IDataBackup implementation

   private:
      //--------------------------------------------------------------
      /// \Brief		Inject C functions in sqlite engine
      //--------------------------------------------------------------
      void registerExtendedFunctions();

      //--------------------------------------------------------------
      /// \Brief		database handler
      //--------------------------------------------------------------
      sqlite3 * m_pDatabaseHandler;

      //--------------------------------------------------------------
      /// \Brief		Startup options
      //--------------------------------------------------------------
      const std::string m_dbFile;

      //--------------------------------------------------------------
      /// \Brief		true if a transaction is already begin
      //--------------------------------------------------------------
      bool m_bOneTransactionActive;

      //--------------------------------------------------------------
      /// \Brief		In case of some errors, (database locked,...) the query may be retried
      //--------------------------------------------------------------
      static int m_maxTries;

      //--------------------------------------------------------------
      /// \Brief		The sql functions helpers for SQLite database
      //--------------------------------------------------------------
      CSQLiteQueryFunctions m_functionsHelper;
   };

} //namespace sqlite
} //namespace database 

