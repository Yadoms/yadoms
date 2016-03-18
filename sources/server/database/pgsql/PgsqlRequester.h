#pragma once

#include "libpq-fe.h"
#include <shared/exception/NullReference.hpp>
#include <shared/Log.h>
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
      virtual database::common::CQuery newQuery();
      virtual void queryEntities(database::common::adapters::IResultAdapter * pAdapter, const database::common::CQuery & querytoExecute);
      virtual int queryStatement(const database::common::CQuery & querytoExecute, bool throwIfFails = true);
      virtual int queryCount(const database::common::CQuery & querytoExecute);
      virtual QueryRow querySingleLine(const database::common::CQuery & querytoExecute);
      virtual QueryResults query(const database::common::CQuery & querytoExecute);
      virtual bool checkTableExists(const database::common::CDatabaseTable & tableName);
      virtual bool dropTableIfExists(const database::common::CDatabaseTable & tableName);
      virtual bool createTableIfNotExists(const database::common::CDatabaseTable & tableName, const std::string & tableScript);
      virtual void createIndex(const database::common::CDatabaseTable & tableName, const std::string & indexScript);
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
      virtual void queryEntities(database::common::adapters::IResultAdapter * pAdapter, const database::common::CQuery & querytoExecute, PGconn * pConnection);
      virtual int queryStatement(const database::common::CQuery & querytoExecute, bool throwIfFails, PGconn * pConnection);
      virtual int queryCount(const database::common::CQuery & querytoExecute, PGconn * pConnection);
      virtual void transactionBegin(PGconn * pConnection);
      virtual void transactionCommit(PGconn * pConnection);
      virtual void transactionRollback(PGconn * pConnection);
      virtual bool transactionIsAlreadyCreated(PGconn * pConnection);

      //--------------------------------------------------------------
      /// \Brief		Close all active connections
      //--------------------------------------------------------------
      void closeAllConnections();

      //--------------------------------------------------------------
      /// \Brief		Execute a query
      /// \param [in] pConnection         The connection to use
      /// \param [in] querytoExecute      The query to execute
      /// \param [in] expectedResultCode  The expected result code (might differ depending on the statement type)
      /// \param [in] throwIfFails        If true an exception is thrown on error, else the PGresult is returned
      /// \return    The PGresult data (must be cleared)
      /// \throws    CDatabaseException If failed. In case of disconnection, close any active connection
      //--------------------------------------------------------------
      PGresult * executeQuery(PGconn * pConnection, const std::string & querytoExecute, ExecStatusType expectedResultCode, bool throwIfFails);

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

      //--------------------------------------------------------------
      /// \Brief		         Create a new connection (one for each thread; testing one for each request)
      /// \return             A connection pointer
      //--------------------------------------------------------------
      PGconn * createNewConnection();  
      
      //--------------------------------------------------------------
      /// \Brief		         Get the current thread connection (create it if necessary)
      /// \return             A connection pointer
      //--------------------------------------------------------------
      PGconn * getConnection();
       
      //--------------------------------------------------------------
      /// \Brief		         Terminate a connection (one for each thread; testing one for each request)
      /// \param [in] pConnection   The connection pointer to close
      //--------------------------------------------------------------
      void terminateConnection(PGconn * pConnection);
      
      //--------------------------------------------------------------
      /// \Brief		         Obtain the last error message
      /// \param [in] pConnection   The connection pointer to retreive the last error message
      /// \return                   The last error message above the specified connection
      //--------------------------------------------------------------
      std::string getLastErrorMessage(PGconn * pConnection);

   private:
      const std::string m_host;
      const unsigned int m_port;
      const std::string m_dbName;
      const std::string m_login;
      const std::string m_password;

      //--------------------------------------------------------------
      /// \Brief		List all connections (one per thread)
      //--------------------------------------------------------------
      std::map<unsigned long, PGconn*> m_connectionList;

      //--------------------------------------------------------------
      /// \Brief		List all transactions (one per connection)
      //--------------------------------------------------------------
      std::map<PGconn*, bool> m_activeTransactionsList;

   };

} //namespace pgsql
} //namespace database 

