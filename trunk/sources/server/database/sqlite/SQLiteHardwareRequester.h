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
   bool createHardware(boost::shared_ptr<CHardware> hardwareToCreate);
	boost::shared_ptr<CHardware> getHardware(const int hardwareId);
   boost::shared_ptr<CHardware> getHardware(const std::string& hardwareName);
	std::vector<boost::shared_ptr<CHardware> > getHardwares();
	bool updateHardware(boost::shared_ptr<CHardware> hardwareToUpdate);
   bool removeHardware(boost::shared_ptr<CHardware> hardwareToRemove);
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
