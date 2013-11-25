#pragma once

#include "server/StartupOptions.h"
#include "server/database/IDatabaseProvider.h"
#include "boost/scoped_ptr.hpp"

class CSQLiteHardwareRequester;
struct sqlite3;

//--------------------------------------------------------------
/// \Brief		   Database provider for SQLite
//--------------------------------------------------------------
class CSQLiteDatabaseProvider : public IDatabaseProvider
{
public:
	//--------------------------------------------------------------
	/// \Brief		   Constructor
	/// \param [in]	pConfig: startup configuration
	//--------------------------------------------------------------
	CSQLiteDatabaseProvider(const std::string & dbFile);
	
	//--------------------------------------------------------------
	/// \Brief		   Destructor
	//--------------------------------------------------------------
	virtual ~CSQLiteDatabaseProvider();

   // IDatabaseProvider implementation
	bool load();
	void unload();
   // [END] IHardwarePlugin implementation

//--------------------------------------------------------------
	/// \brief	Use do define one row from database
	//--------------------------------------------------------------
   typedef std::vector<std::string>  QueryRow;

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