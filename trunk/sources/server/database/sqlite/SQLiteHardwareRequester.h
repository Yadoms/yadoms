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
   bool createHardware(boost::scoped_ptr<CHardware> hardwareToRemove);
	boost::scoped_ptr<CHardware> getHardware(const int hardwareId);
	std::list<boost::scoped_ptr<CHardware>> getHardwares();
	bool updateHardware(boost::scoped_ptr<CHardware> hardwareToUpdate);
   bool removeHardware(boost::scoped_ptr<CHardware> hardwareToRemove);
   // [END] IHardwareRequester implementation

private:
	//--------------------------------------------------------------
	/// \Brief		   Reference to SQLiteDatabseHandler
	//--------------------------------------------------------------
	CSQLiteDatabaseProvider * m_pDatabaseHandler;
};
