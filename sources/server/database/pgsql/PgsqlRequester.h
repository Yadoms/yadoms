#pragma once

#include "database/IDatabaseRequester.h"
#include <Poco/Nullable.h>
#include "PgsqlLibrary.h"

namespace database
{
   namespace pgsql
   {
      class CPgsqlRequester : public IDatabaseRequester
      {
      public:
         explicit CPgsqlRequester(boost::shared_ptr<CPgsqlLibrary> pgsqlLibrary);
         virtual ~CPgsqlRequester();

         // IDatabaseEngine implementation
         void initialize() override;
         void finalize() override;
         shared::CDataContainer getInformation() override;
         // [END] IDatabaseEngine implementation

         // IDatabaseRequester implementation
         boost::shared_ptr<common::CQuery> newQuery() override;
         void queryEntities(common::adapters::IResultAdapter* pAdapter, const common::CQuery& querytoExecute) override;
         int queryStatement(const common::CQuery& querytoExecute, bool throwIfFails = true) override;
         int queryCount(const common::CQuery& querytoExecute) override;
         QueryRow querySingleLine(const common::CQuery& querytoExecute) override;
         QueryResults query(const common::CQuery& querytoExecute) override;
         bool checkTableExists(const common::CDatabaseTable& tableName) override;
         bool dropTableIfExists(const common::CDatabaseTable& tableName) override;
         bool createTableIfNotExists(const common::CDatabaseTable& tableName, const std::string& tableScript) override;
         bool addTableColumn(const common::CDatabaseTable& tableName, const std::string& columnDef) override;
         void createIndex(const common::CDatabaseTable& tableName, const std::string& indexScript) override;
         void vacuum() override;
         boost::shared_ptr<ITableCreationScriptProvider> getTableCreationScriptProvider() override;
         bool supportInsertOrUpdateStatement() override;
         // [END] IDatabaseRequester implementation

         // ITransactionalProvider implementation
         bool transactionSupport() override;
         void transactionBegin() override;
         void transactionCommit() override;
         void transactionRollback() override;
         bool transactionIsAlreadyCreated() override;
         // [END] ITransactionalProvider implementation

         // IDataBackup implementation
         bool backupSupported() override;
         void backupData(ProgressFunc reporter) override;
         boost::filesystem::path lastBackupData() override;
         // [END] IDataBackup implementation

      private:
         void queryEntities(common::adapters::IResultAdapter* pAdapter, const common::CQuery& querytoExecute, PGconn* pConnection);
         int queryStatement(const common::CQuery& querytoExecute, bool throwIfFails, PGconn* pConnection);
         int queryCount(const common::CQuery& querytoExecute, PGconn* pConnection);
         void transactionBegin(PGconn* pConnection);
         void transactionCommit(PGconn* pConnection);
         void transactionRollback(PGconn* pConnection);
         bool transactionIsAlreadyCreated(PGconn* pConnection);

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
         PGresult* executeQuery(PGconn* pConnection, const std::string& querytoExecute, ExecStatusType expectedResultCode, bool throwIfFails);

         //--------------------------------------------------------------
         /// \Brief		Try to ping PostgreSQL server
         /// \throws    CDatabaseException If ping failed
         //--------------------------------------------------------------
         void pingServer();

         typedef enum
         {
            kNormal, /// full connection string
            kPing, ///ping connection string
            kMasterDb //light connection string, with targetting postgres database
         } EConnectionStringMode;

         //--------------------------------------------------------------
         /// \Brief		         Create the connection string
         /// \param [in]	mode  The connection string mode.
         /// \return             The connection string
         //--------------------------------------------------------------
         std::string createConnectionString(const EConnectionStringMode mode = kNormal);

         //--------------------------------------------------------------
         /// \Brief		         Append (or not) an optional parameter to the connection string
         /// \param [in/out]	   cnx         The connection string to complete.
         /// \param [in]	      value       The optional parameter
         /// \param [in]	      paramName   The parameter name (to add in connection string)
         //--------------------------------------------------------------
         template <class T>
         void connectionStringAppendOptionalParameter(std::string& cnx, Poco::Nullable<T> value, const std::string& paramName)
         {
            if (!value.isNull())
            {
               std::stringstream ss;
               ss << " " << paramName << "=" << value.value();
               cnx += ss.str();
            }
         }

         //--------------------------------------------------------------
         /// \Brief		         Create a new connection (one for each thread; testing one for each request)
         /// \return             A connection pointer
         //--------------------------------------------------------------
         PGconn* createNewConnection();

         //--------------------------------------------------------------
         /// \Brief		         Get the current thread connection (create it if necessary)
         /// \return             A connection pointer
         //--------------------------------------------------------------
         PGconn* getConnection();

         //--------------------------------------------------------------
         /// \Brief		         Terminate a connection (one for each thread; testing one for each request)
         /// \param [in] pConnection   The connection pointer to close
         //--------------------------------------------------------------
         void terminateConnection(PGconn* pConnection) const;

         //--------------------------------------------------------------
         /// \Brief		         Obtain the last error message
         /// \param [in] pConnection   The connection pointer to retreive the last error message
         /// \return                   The last error message above the specified connection
         //--------------------------------------------------------------
         std::string getLastErrorMessage(PGconn* pConnection) const;

         //--------------------------------------------------------------
         /// \Brief		Dynamic loaded library instance
         //--------------------------------------------------------------
         boost::shared_ptr<CPgsqlLibrary> m_pgsqlLibrary;

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


