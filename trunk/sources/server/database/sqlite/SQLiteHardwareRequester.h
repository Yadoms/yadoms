#pragma once

#include "server/database/IHardwareRequester.h"
class CSQLiteDataProvider;

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
	CSQLiteHardwareRequester(CSQLiteDataProvider * pDatabaseHandler);

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

   //test
   std::vector<std::string> getHardwareNameList();

private:
	//--------------------------------------------------------------
	/// \Brief		   Reference to SQLiteDatabseHandler
	//--------------------------------------------------------------
	CSQLiteDataProvider * m_pDatabaseHandler;
};
