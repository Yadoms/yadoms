#pragma once

#include "server/database/IHardwareRequester.h"
class CSQLiteDatabaseProvider;

//--------------------------------------------------------------
/// \Brief		   Hardware requester for SQLite database
//--------------------------------------------------------------
class CSQLiteHardwareRequester: public IHardwareRequester
{
public:
	//--------------------------------------------------------------
	/// \Brief		   Constructor
	/// \param [in]	pDatabaseHandler: the database handler
	//--------------------------------------------------------------
	CSQLiteHardwareRequester(CSQLiteDatabaseProvider * pDatabaseHandler);

	//--------------------------------------------------------------
	/// \Brief		   Destructor
	//--------------------------------------------------------------
	virtual ~CSQLiteHardwareRequester();

   // IHardwareRequester implementation
   bool createHardware(CHardware & hardwareToRemove);
	CHardware getHardware(const int hardwareId);
	std::vector<CHardware> getHardwares();
	bool updateHardware(CHardware & hardwareToUpdate);
   bool removeHardware(CHardware & hardwareToRemove);
   // [END] IHardwareRequester implementation

private:
	//--------------------------------------------------------------
	/// \Brief		   Reference to SQLiteDatabseHandler
	//--------------------------------------------------------------
	CSQLiteDatabaseProvider * m_pDatabaseHandler;
};
