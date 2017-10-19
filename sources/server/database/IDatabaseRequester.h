#pragma once

#include "common/Query.h"
#include "common/adapters/IResultAdapter.h"
#include "ITransactionalProvider.h"
#include "IDataBackup.h"
#include "IDatabaseEngine.h"
#include "ITableCreationScriptProvider.h"

namespace database
{
   //---------------------------------
   ///\brief Interface for database requester
   //---------------------------------
   class IDatabaseRequester : public IDatabaseEngine,
                              public ITransactionalProvider,
                              public IDataBackup
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Use do define one row from database (column name, column value)
      //--------------------------------------------------------------
      typedef std::map<std::string, std::string> QueryRow;

      //--------------------------------------------------------------
      /// \brief	Use do define a list of rows from database
      //--------------------------------------------------------------
      typedef std::vector<QueryRow> QueryResults;

      //--------------------------------------------------------------
      /// \Brief  Create a new CQuery object
      /// \return The created CQuery
      //--------------------------------------------------------------
      virtual boost::shared_ptr<common::CQuery> newQuery() = 0;

      //--------------------------------------------------------------
      /// \Brief		    query for entities (the result is a vector of typed objects, accessible by a call to pAdapter->GetResult())
      /// \param [in]	 adapter:  pointer to the adapter to use to map raw values to a new entity
      /// \param [in]	 querytoExecute: the sql query
      //--------------------------------------------------------------
      virtual void queryEntities(common::adapters::IResultAdapter* pAdapter,
                                 const common::CQuery& querytoExecute) = 0;


      //--------------------------------------------------------------
      /// \brief		      execute a single statement (create, update, delete) which returns the number of affected lines
      /// \param [in]	   the query (with vaargs)
      /// \param [in]	   throwIfFails If true, generate an exception when it fails; else return -1 (= number of afected rows)
      /// \return 	      the number of affected lines
      //--------------------------------------------------------------  
      virtual int queryStatement(const common::CQuery& querytoExecute,
                                 bool throwIfFails = true) = 0;

      //--------------------------------------------------------------
      /// \brief		      execute a count statement
      /// \param [in]	   the query (with vaargs)
      /// \return 	      the count result
      //--------------------------------------------------------------  
      virtual int queryCount(const common::CQuery& querytoExecute) = 0;

      //--------------------------------------------------------------
      /// \brief		      query only one databse row
      /// \param [in]	   the query (with vaargs)
      /// \return 	      the row values
      //--------------------------------------------------------------  
      virtual QueryRow querySingleLine(const common::CQuery& querytoExecute) = 0;

      //--------------------------------------------------------------
      /// \brief		      query databse
      /// \param [in]	   the query (with vaargs)
      /// \return 	      a list of rows
      //--------------------------------------------------------------  
      virtual QueryResults query(const common::CQuery& querytoExecute) = 0;


      //--------------------------------------------------------------
      /// \Brief	Check if a table already exists in database
      /// \param [in]	 tableName:  the table name to check
      /// \return	       true if table exists
      //--------------------------------------------------------------
      virtual bool checkTableExists(const common::CDatabaseTable& tableName) = 0;

      //--------------------------------------------------------------
      /// \Brief	      Drop a table if exists
      /// \param [in]   tableName:  the table name to delete
      /// \return	      true if table no more exists
      //--------------------------------------------------------------
      virtual bool dropTableIfExists(const common::CDatabaseTable& tableName) = 0;

      //--------------------------------------------------------------
      /// \Brief	      Create a table if not exists
      /// \param [in]   tableName:  the table name to create
      /// \param [in]   tableScript:  the sql script to create the table
      /// \return	      true if table exists
      //--------------------------------------------------------------
      virtual bool createTableIfNotExists(const common::CDatabaseTable& tableName,
                                          const std::string& tableScript) = 0;

      //--------------------------------------------------------------
      /// \Brief	      Add a new column to a table
      /// \param [in]   tableName:  the table name to alter
      /// \param [in]   columnDef:  the column definition
      /// \return	      true if column created with success
      //--------------------------------------------------------------
      virtual bool addTableColumn(const common::CDatabaseTable& tableName,
                                 const std::string& columnDef) = 0;

      //--------------------------------------------------------------
      /// \Brief	      Create an index if not exists
      /// \param [in]   tableName:  the table name to create
      /// \param [in]   indexScript:  the sql script to create the index
      //--------------------------------------------------------------
      virtual void createIndex(const common::CDatabaseTable& tableName,
                               const std::string& indexScript) = 0;

      //--------------------------------------------------------------
      /// \Brief	      Vacuum the database (compact it)
      //--------------------------------------------------------------
      virtual void vacuum() = 0;

      //--------------------------------------------------------------
      /// \Brief	      Tells if database support insert or update statement
      //--------------------------------------------------------------
      virtual bool supportInsertOrUpdateStatement() = 0;

      // ITransactionalProvider implementation
      bool transactionSupport() override = 0;
      void transactionBegin() override = 0;
      void transactionCommit() override = 0;
      void transactionRollback() override = 0;
      bool transactionIsAlreadyCreated() override = 0;
      // [END] ITransactionalProvider implementation

      // IDataBackup implementation
      void backupData(const std::string & backupFolder, ProgressFunc reporter) override = 0;
      // [END] IDataBackup implementation

      // IDatabaseEngine implementation
      void initialize() override = 0;
      void finalize() override = 0;
      // [END] IDatabaseEngine implementation

      //--------------------------------------------------------------
      /// \Brief		get the table creation script provider
      /// \return 	the table creation script provider
      /// \throws 		
      //--------------------------------------------------------------  
      virtual boost::shared_ptr<ITableCreationScriptProvider> getTableCreationScriptProvider() = 0;


      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IDatabaseRequester()
      {
      }
   };
} //namespace database 


