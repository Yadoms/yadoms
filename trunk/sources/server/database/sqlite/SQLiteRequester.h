#pragma once

#include "sqlite3.h"
#include "adapters/ISQLiteResultAdapter.h"
#include <shared/exception/NullReference.hpp>
#include <shared/Log.h>
#include "Query.h"
#include "database/ITransactionalProvider.h"


namespace database { 
namespace sqlite { 

   class CSQLiteRequester : public ITransactionalProvider
   {
   public:
      CSQLiteRequester(sqlite3 * pDatabaseHandler);
      virtual ~CSQLiteRequester();

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
      /// \Brief		    query for entities (the result is a vector of typed objects, accessible by a call to pAdapter->GetResult())
      /// \param [in]	 adapter:  pointer to the adapter to use to map raw values to a new entity
      /// \param [in]	 querytoExecute: the sql query
      //--------------------------------------------------------------
      template<class TEntity>
      void queryEntities(adapters::ISQLiteResultAdapter<TEntity> * pAdapter, const CQuery & querytoExecute)
      {
         BOOST_ASSERT(pAdapter != NULL);

         if(pAdapter != NULL)
         {

            try
            {
               int remainingTries = m_maxTries;
               bool retry;

               do
               {
                  //ensure retry is reset to false
                  retry = false;

                  sqlite3_stmt *stmt;
                  int rc = sqlite3_prepare_v2(m_pDatabaseHandler, querytoExecute.c_str(), -1, &stmt, 0);
                  if (rc == SQLITE_OK)
                  {
                     if(!pAdapter->adapt(stmt))
                     {
                        YADOMS_LOG(error) << "Fail to adapt values";
                     }
                     sqlite3_finalize(stmt);
                  }
                  else
                  {
                     if(rc == SQLITE_LOCKED && remainingTries>1)
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
               while( (--remainingTries) > 0 && retry);

            }
            catch(std::exception &ex)
            {
               YADOMS_LOG(error) << "Exception: Fail to execute query : " << ex.what();
            }
            catch(...)
            {
               YADOMS_LOG(error) << "Unknown exception: Fail to execute query";
            }
         }
         else
         {
            //throw exception
            throw shared::exception::CNullReference("pAdapter");
         }
      }


      // ITransactionalProvider implementation
      virtual void transactionBegin();
      virtual void transactionCommit();
      virtual void transactionRollback();
      // [END] ITransactionalProvider implementation

      //--------------------------------------------------------------
      /// \Brief	       Check if a transaction is already created
      /// \return	       true if a transaction is already created
      //--------------------------------------------------------------
      bool transactionIsAlreadyCreated();

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
      
      //--------------------------------------------------------------
      /// \Brief	      Create an index if not exists
      /// \param [in]   tableName:  the table name to create
      /// \param [in]   indexScript:  the sql script to create the index
      //--------------------------------------------------------------
      void createIndex(const std::string & tableName, const std::string & indexScript);

      //--------------------------------------------------------------
      /// \Brief	      Vacuum the database (compact it)
      //--------------------------------------------------------------
      void vacuum();

   private:
      //--------------------------------------------------------------
      /// \Brief		database handler
      //--------------------------------------------------------------
      sqlite3 * m_pDatabaseHandler;

      //--------------------------------------------------------------
      /// \Brief		true if a transaction is already begin
      //--------------------------------------------------------------
      bool m_bOneTransactionActive;

      //--------------------------------------------------------------
      /// \Brief		In case of some errors, (database locked,...) the query may be retried
      //--------------------------------------------------------------
      static int m_maxTries;
   };

} //namespace sqlite
} //namespace database 

