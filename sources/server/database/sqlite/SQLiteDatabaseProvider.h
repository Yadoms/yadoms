#pragma once

#include "server/StartupOptions.h"
#include "server/database/IDatabaseProvider.h"

class CSQLiteHardwareRequester;
class sqlite3;

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
	CSQLiteDatabaseProvider(const IStartupOptions& startupOptions);
	
	//--------------------------------------------------------------
	/// \Brief		   Destructor
	//--------------------------------------------------------------
	virtual ~CSQLiteDatabaseProvider();

   // IDatabaseProvider implementation
	bool load();
	void unload();
   int queryStatement(const std::string & queryFormat, ...);
   QueryRow querySingleLine(const std::string & queryFormat, ...);
   QueryResults query(const std::string & queryFormat, ...);
   // [END] IHardwarePlugin implementation

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
	const IStartupOptions& m_startupOptions;

	//--------------------------------------------------------------
	/// \Brief		database handler
	//--------------------------------------------------------------
	sqlite3 * m_pDatabaseHandler;
};