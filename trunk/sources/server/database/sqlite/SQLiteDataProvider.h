#pragma once

#include "server/database/IDataProvider.h"
#include "SQLiteHardwareRequester.h"
#include "SQLiteRequester.h"
#include "sqlite3.h"


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
   boost::shared_ptr<IHardwareRequester> getHardwareRequester() { return m_hardwareRequester; }
   // [END] IHardwarePlugin implementation

private:
	//--------------------------------------------------------------
	/// \Brief		Load all requesters
	//--------------------------------------------------------------
	void loadRequesters();
	
private:
	//--------------------------------------------------------------
	/// \Brief		Hardware requester
	//--------------------------------------------------------------
	boost::shared_ptr<CSQLiteHardwareRequester>		m_hardwareRequester;
	
	//--------------------------------------------------------------
	/// \Brief		Startup options
	//--------------------------------------------------------------
	const std::string m_dbFile;

	//--------------------------------------------------------------
	/// \Brief		database handler
	//--------------------------------------------------------------
   sqlite3 * m_pDatabaseHandler; //not use shared_ptr because not alllocated by this library

	//--------------------------------------------------------------
	/// \Brief		database requester (for SQL queries)
	//--------------------------------------------------------------
   boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
};