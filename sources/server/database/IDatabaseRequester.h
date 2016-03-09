#pragma once

#include "entities/Entities.h"
#include "common/Query.h"
#include "common/QueryFunctions.h"
#include "common/adapters/IResultAdapter.h"
#include "ITransactionalProvider.h"
#include "IDataBackup.h"
#include "IDatabaseEngine.h"
#include "ITableCreationScriptProvider.h"

namespace database { 

   //---------------------------------
   ///\brief Interface for database requester
   //---------------------------------
   class IDatabaseRequester : public IDatabaseEngine, public ITransactionalProvider, public IDataBackup
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Use do define one row from database (column name, column value)
      //--------------------------------------------------------------
      typedef std::map<std::string, std::string>  QueryRow;

      //--------------------------------------------------------------
      /// \brief	Use do define a list of rows from database
      //--------------------------------------------------------------
      typedef std::vector<QueryRow>  QueryResults;

      //--------------------------------------------------------------
      /// \Brief  Create a new CQuery object
      /// \return The created CQuery
      //--------------------------------------------------------------
      virtual database::common::CQuery newQuery() = 0;

      //--------------------------------------------------------------
      /// \Brief  Create a new CQueryFunctions function object
      /// \return The created CQueryFunctions function 
      //--------------------------------------------------------------
      virtual database::common::CQueryFunctions & queryFunc() = 0;
      

      //--------------------------------------------------------------
      /// \Brief		    query for entities (the result is a vector of typed objects, accessible by a call to pAdapter->GetResult())
      /// \param [in]	 adapter:  pointer to the adapter to use to map raw values to a new entity
      /// \param [in]	 querytoExecute: the sql query
      //--------------------------------------------------------------
      virtual void queryEntities(database::common::adapters::IResultAdapter * pAdapter, const database::common::CQuery & querytoExecute) = 0;


      //--------------------------------------------------------------
      /// \brief		      execute a single statement (create, update, delete) which returns the number of affected lines
      /// \param [in]	   the query (with vaargs)
      /// \return 	      the number of affected lines
      //--------------------------------------------------------------  
      virtual int queryStatement(const database::common::CQuery & querytoExecute) = 0;

      //--------------------------------------------------------------
      /// \brief		      execute a count statement
      /// \param [in]	   the query (with vaargs)
      /// \return 	      the count result
      //--------------------------------------------------------------  
      virtual int queryCount(const database::common::CQuery & querytoExecute) = 0;

      //--------------------------------------------------------------
      /// \brief		      query only one databse row
      /// \param [in]	   the query (with vaargs)
      /// \return 	      the row values
      //--------------------------------------------------------------  
      virtual QueryRow querySingleLine(const database::common::CQuery & querytoExecute) = 0;

      //--------------------------------------------------------------
      /// \brief		      query databse
      /// \param [in]	   the query (with vaargs)
      /// \return 	      a list of rows
      //--------------------------------------------------------------  
      virtual QueryResults query(const database::common::CQuery & querytoExecute) = 0;


      //--------------------------------------------------------------
      /// \Brief	Check if a table already exists in database
      /// \param [in]	 tableName:  the table name to check
      /// \return	       true if table exists
      //--------------------------------------------------------------
      virtual bool checkTableExists(const std::string & tableName) = 0;

      //--------------------------------------------------------------
      /// \Brief	      Drop a table if exists
      /// \param [in]   tableName:  the table name to delete
      /// \return	      true if table no more exists
      //--------------------------------------------------------------
      virtual bool dropTableIfExists(const std::string & tableName) = 0;

      //--------------------------------------------------------------
      /// \Brief	      Create a table if not exists
      /// \param [in]   tableName:  the table name to create
      /// \param [in]   tableScript:  the sql script to create the table
      /// \return	      true if table exists
      //--------------------------------------------------------------
      virtual bool createTableIfNotExists(const std::string & tableName, const std::string & tableScript) = 0;

      //--------------------------------------------------------------
      /// \Brief	      Create an index if not exists
      /// \param [in]   tableName:  the table name to create
      /// \param [in]   indexScript:  the sql script to create the index
      //--------------------------------------------------------------
      virtual void createIndex(const std::string & tableName, const std::string & indexScript) = 0;

      //--------------------------------------------------------------
      /// \Brief	      Vacuum the database (compact it)
      //--------------------------------------------------------------
      virtual void vacuum() = 0;

      //--------------------------------------------------------------
      /// \Brief	      Generate SQL query part to format date as ISO string
      /// \param [in]   columnName:  the column name
      /// \return       the sql part which formats date in ISO format
      //--------------------------------------------------------------
      virtual std::string generateSqlIsoDateFormat(const std::string &columnName) = 0;

      //--------------------------------------------------------------
      /// \Brief	      Generate SQL coalesce function
      /// \param [in]   columnName:  the column name
      /// \param [in]   columnName:  the default value if columnName is null
      /// \return       the sql function
      //--------------------------------------------------------------
      virtual std::string coalesce(const std::string & columnName, std::string defaultValue) = 0;
      
      
      // ITransactionalProvider implementation
      virtual bool transactionSupport() = 0;
      virtual void transactionBegin() = 0;
      virtual void transactionCommit() = 0;
      virtual void transactionRollback() = 0;
      virtual bool transactionIsAlreadyCreated() = 0;
      // [END] ITransactionalProvider implementation

      // IDataBackup implementation
      virtual void backupData(const std::string & backupLocation, IDataBackup::ProgressFunc reporter) = 0;
      // [END] IDataBackup implementation

      // IDatabaseEngine implementation
      virtual void initialize() = 0;
      virtual void finalize() = 0;
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
