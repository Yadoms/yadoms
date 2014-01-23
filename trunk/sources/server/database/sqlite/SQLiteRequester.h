#pragma once

#include "sqlite3.h"
#include "adapters/ISQLiteResultAdapter.h"
#include <shared/Exceptions/NullReferenceException.hpp>
#include <shared/Log.h>
#include "Query.h"

class CSQLiteRequester
{
public:
   CSQLiteRequester(sqlite3 * pDatabaseHandler);
   ~CSQLiteRequester();

   //--------------------------------------------------------------
	/// \brief	Use do define one row from database (column name, column value)
	//--------------------------------------------------------------
   typedef std::map<std::string, std::string>  QueryRow;

   //--------------------------------------------------------------
	/// \brief	Use do define a list of rows from database
	//--------------------------------------------------------------
   typedef std::vector<QueryRow>  QueryResults;

	//--------------------------------------------------------------
	/// \brief		      execute a single statement (create, update, delete) which returns the number of affected lines
	/// \param [in]	   the query (with vaargs)
	/// \return 	      the number of affected lines
	//--------------------------------------------------------------  
   int queryStatement(const CQuery & querytoExecute);

	//--------------------------------------------------------------
	/// \brief		      execute a count statement
	/// \param [in]	   the query (with vaargs)
	/// \return 	      the count result
	//--------------------------------------------------------------  
   int queryCount(const CQuery & querytoExecute);

	//--------------------------------------------------------------
	/// \brief		      query only one databse row
	/// \param [in]	   the query (with vaargs)
	/// \return 	      the row values
	//--------------------------------------------------------------  
   QueryRow querySingleLine(const CQuery & querytoExecute);

	//--------------------------------------------------------------
	/// \brief		      query databse
	/// \param [in]	   the query (with vaargs)
	/// \return 	      a list of rows
	//--------------------------------------------------------------  
   QueryResults query(const CQuery & querytoExecute);


   //--------------------------------------------------------------
	/// \Brief		    query for entities (the result is a vector of typed objects)
	/// \param [in]	 adapter:  pointer to the adapter to use to map raw values to a new entity
	/// \param [in]	 queryFormat: the sql query
	/// \param [in]	 ...: args of printf like format string for query
	/// \return	       the vector of entities
	//--------------------------------------------------------------
   template<class TEntity>
   void queryEntities(ISQLiteResultAdapter<TEntity> * pAdapter, const CQuery & querytoExecute)
   {
      BOOST_ASSERT(pAdapter != NULL);

      if(pAdapter != NULL)
      {
         //execute the query
         char *zErrMsg = NULL;
         sqlite3_exec(m_pDatabaseHandler, querytoExecute.c_str(),  &CSQLiteRequester::handleRowEntityAdapter<TEntity>, pAdapter, &zErrMsg);
         if( zErrMsg )
         {
            YADOMS_LOG(error) << "Query failed : " << std::endl << "Query: " << querytoExecute.str() << std::endl << "Error : " << zErrMsg;
         }

         //free allocated memory by sqlite
         if(zErrMsg)
            sqlite3_free(zErrMsg);
      }
      else
      {
         //throw exception
         throw new CNullReferenceException("pAdapter");
      }
   }

   //--------------------------------------------------------------
	/// \Brief	Start a transaction
	//--------------------------------------------------------------
   void transactionBegin();

   //--------------------------------------------------------------
	/// \Brief	Commit a transaction
	//--------------------------------------------------------------
   void transactionCommit();

   //--------------------------------------------------------------
	/// \Brief	Rollback a transaction
	//--------------------------------------------------------------
   void transactionRollback();

   //--------------------------------------------------------------
	/// \Brief	Check if a table already exists in database
	/// \param [in]	 tableName:  the table name to check
	/// \return	       true if table exists
	//--------------------------------------------------------------
   bool checkTableExists(const std::string & tableName);

   //--------------------------------------------------------------
	/// \Brief	      Drop a table if exists
	/// \param [in]   tableName:  the table name to delete
	/// \return	      true if table no more exists
	//--------------------------------------------------------------
   bool dropTableIfExists(const std::string & tableName);

   //--------------------------------------------------------------
	/// \Brief	      Create a table if not exists
	/// \param [in]   tableName:  the table name to create
	/// \param [in]   tableScript:  the sql script to create the table
	/// \return	      true if table exists
	//--------------------------------------------------------------
   bool createTableIfNotExists(const std::string & tableName, const std::string & tableScript);

private:
   //--------------------------------------------------------------
	/// \Brief		    wrapper for adapting a result row to an entity
	/// \param [in]	 params:  pointer to adapter
	/// \param [in]	 columnCount: number of columns in result set
	/// \param [in]	 columnValues: list of column values
	/// \param [in]	 columnNames: list of column names
	/// \return	       SQLITE_OK for success, else SQLITE_ABORT (abort, cancel and abort the full query)
	//--------------------------------------------------------------
   template<class TEntity>
   static int handleRowEntityAdapter(void* params ,int columnCount,char** columnValues,char** columnNames)
   {
      try
      {
         ISQLiteResultAdapter<TEntity> * pAdatper = (ISQLiteResultAdapter<TEntity> *)params;
         if(pAdatper->adapt(columnCount, columnValues, columnNames))
            return SQLITE_OK;
      }
      catch(...)
      {
         YADOMS_LOG(error) << "Fail to adapt row values. Query aborted.";
      }
      return SQLITE_ABORT;
   }

   
   int handleOneRow(void* params ,int columnCount,char** columnValues,char** columnNames);

private:
	//--------------------------------------------------------------
	/// \Brief		database handler
	//--------------------------------------------------------------
   sqlite3 * m_pDatabaseHandler;

};
