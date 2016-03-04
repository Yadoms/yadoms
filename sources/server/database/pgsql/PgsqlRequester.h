#pragma once

#include "libpq-fe.h"
#include <shared/exception/NullReference.hpp>
#include <shared/Log.h>
#include "Query.h"
#include "database/IDatabaseRequester.h"

namespace database { 
namespace pgsql { 

   class CPgsqlRequester : public IDatabaseRequester
   {
   public:
      CPgsqlRequester(const std::string &host, const unsigned int port, const std::string &dbName, const std::string &login, const std::string &password);
      virtual ~CPgsqlRequester();

      // IDatabaseEngine implementation
      virtual void initialize();
      virtual void finalize();
      // [END] IDatabaseEngine implementation

      // IDatabaseRequester implementation
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
      /// \Brief		Try to ping PostgreSQL server
      /// \throws    CDatabaseException If ping failed
      //--------------------------------------------------------------
      void pingServer();

      typedef enum
      {
         kNormal, /// full connection string
         kPing,   ///ping connection string
         kMasterDb   //light connection string, with targetting postgres database
      }EConnectionStringMode;

      //--------------------------------------------------------------
      /// \Brief		         Create the connection string
      /// \param [in]	mode  The connection string mode.
      /// \return             The connection string
      //--------------------------------------------------------------
      const std::string createConnectionString(const EConnectionStringMode mode = kNormal);

      void terminateConnection();
      std::string getLastErrorMessage();

   private:
      const std::string m_host;
      const unsigned int m_port;
      const std::string m_dbName;
      const std::string m_login;
      const std::string m_password;
      const std::string m_schema;
      //--------------------------------------------------------------
      /// \Brief		database handler
      //--------------------------------------------------------------
      PGconn     *m_pConnection;

      //--------------------------------------------------------------
      /// \Brief		true if a transaction is already begin
      //--------------------------------------------------------------
      bool m_bOneTransactionActive;
   };

} //namespace pgsql
} //namespace database 

