#pragma once

#include "server/database/IHardwareRequester.h"
class CSQLiteDataProvider;
class CSQLiteRequester;

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
	CSQLiteHardwareRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

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
	const CSQLiteDataProvider & m_databaseHandler;

	//--------------------------------------------------------------
	/// \Brief		   Reference to SQLiteRequester
	//--------------------------------------------------------------
   boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
};
