#pragma once

#include "server/database/IDeviceRequester.h"
class CSQLiteDataProvider;
class CSQLiteRequester;

//--------------------------------------------------------------
/// \Brief		   Device requester for SQLite database
//--------------------------------------------------------------
class CSQLiteDeviceRequester: public IDeviceRequester
{
public:
	//--------------------------------------------------------------
	/// \Brief		   Constructor
	/// \param [in]	pDatabaseHandler: the database handler
	//--------------------------------------------------------------
	CSQLiteDeviceRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

	//--------------------------------------------------------------
	/// \Brief		   Destructor
	//--------------------------------------------------------------
	virtual ~CSQLiteDeviceRequester();

   // IDeviceRequester implementation
   virtual int addDevice(boost::shared_ptr<CDevice> newDevice);
   virtual boost::shared_ptr<CDevice> getDevice(int deviceId);
   virtual std::vector<boost::shared_ptr<CDevice> > getDevices();
   virtual void updateDeviceConfiguration(int deviceId, const std::string& newConfiguration);
   virtual void removeDevice(int deviceId);
   // [END] IDeviceRequester implementation

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
