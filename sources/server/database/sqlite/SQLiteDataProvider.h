#pragma once

#include "server/StartupOptions.h"
#include "server/database/IDataProvider.h"
#include "tools/Exceptions/NullReferenceException.h"


class CSQLiteHardwareRequester;

#include "sqlite3.h"

#include "adapters/HardwareAdapter.h"


//--------------------------------------------------------------
/// \Brief		   Database provider for SQLite
//--------------------------------------------------------------
class CSQLiteDataProvider : public IDataProvider
{
public:
	//--------------------------------------------------------------
	/// \Brief		   Constructor
	/// \param [in]	pConfig: startup configuration
	//--------------------------------------------------------------
	CSQLiteDataProvider(const std::string & dbFile);
	
	//--------------------------------------------------------------
	/// \Brief		   Destructor
	//--------------------------------------------------------------
	virtual ~CSQLiteDataProvider();

   // IDatabaseProvider implementation
	bool load();
	void unload();
   // [END] IHardwarePlugin implementation

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
   int queryStatement(const std::string & queryFormat, ...);

	//--------------------------------------------------------------
	/// \brief		      query only one databse row
	/// \param [in]	   the query (with vaargs)
	/// \return 	      the row values
	//--------------------------------------------------------------  
   QueryRow querySingleLine(const std::string & queryFormat, ...);

	//--------------------------------------------------------------
	/// \brief		      query databse
	/// \param [in]	   the query (with vaargs)
	/// \return 	      a list of rows
	//--------------------------------------------------------------  
   QueryResults query(const std::string & queryFormat, ...);


   //--------------------------------------------------------------
	/// \Brief		Get the hardware requester
	/// \return	   The hardware requester
	//--------------------------------------------------------------
	IHardwareRequester * getHardwareRequester() { return (IHardwareRequester*)m_hardwareRequester.get(); }

   
   //--------------------------------------------------------------
	/// \Brief		    query for entities (the result is a vector of typed objects)
	/// \param [in]	 adapter:  pointer to the adapter to use to map raw values to a new entity
	/// \param [in]	 queryFormat: the sql query
	/// \param [in]	 ...: args of printf like format string for query
	/// \return	       the vector of entities
	//--------------------------------------------------------------
   template<class TEntity>
   void queryEntities(ISQLiteResultAdapter<TEntity> * pAdapter, const std::string & queryFormat, ...)
   {
      BOOST_ASSERT(pAdapter != NULL);

      if(pAdapter != NULL)
      {
         //create query
         char *zSql;
         va_list ap;
         va_start(ap, queryFormat);
         zSql = sqlite3_vmprintf(queryFormat.c_str(), ap);
         va_end(ap);

         //execute the query
         char *zErrMsg = NULL;
         sqlite3_exec(m_pDatabaseHandler, zSql,  &CSQLiteDataProvider::handleRowEntityAdapter<TEntity>, pAdapter, &zErrMsg);
         if( zErrMsg )
         {
            BOOST_LOG_TRIVIAL(error) << "Query failed : " << std::endl << "Query: " << zSql << std::endl << "Error : " << zErrMsg;
         }

         //free allocated memory by sqlite
         if(zErrMsg)
            sqlite3_free(zErrMsg);

         if(zSql)
            sqlite3_free(zSql);
      }
      else
      {
         //throw exception
         throw new CNullReferenceException("pAdapter");
      }
   }

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
         BOOST_LOG_TRIVIAL(error) << "Fail to adapt row values. Query aborted.";
      }
      return SQLITE_ABORT;
   }


private:
	//--------------------------------------------------------------
	/// \Brief		Load all requesters
	//--------------------------------------------------------------
	void loadRequesters();
	

   int handleOneRow(void* params ,int columnCount,char** columnValues,char** columnNames);

private:
	//--------------------------------------------------------------
	/// \Brief		Hardware requester
	//--------------------------------------------------------------
	boost::scoped_ptr<CSQLiteHardwareRequester>		m_hardwareRequester;
	
	//--------------------------------------------------------------
	/// \Brief		Startup options
	//--------------------------------------------------------------
	const std::string m_dbFile;

	//--------------------------------------------------------------
	/// \Brief		database handler
	//--------------------------------------------------------------
	sqlite3 * m_pDatabaseHandler;
};