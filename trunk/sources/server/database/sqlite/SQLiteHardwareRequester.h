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
   virtual int addHardware(boost::shared_ptr<CHardware> newHardware);
	virtual boost::shared_ptr<CHardware> getHardware(int hardwareId);
	virtual std::vector<boost::shared_ptr<CHardware> > getHardwares();
	virtual bool updateHardware(boost::shared_ptr<CHardware> hardware);
   virtual bool removeHardware(int hardwareId);
   virtual void enableInstance(int hardwareId, bool enable);
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

   //--------------------------------------------------------------
   /// \Brief		   The table name
   //--------------------------------------------------------------
   static const std::string m_tableName;
};
