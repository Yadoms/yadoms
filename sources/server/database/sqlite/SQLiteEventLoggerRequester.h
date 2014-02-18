#pragma once

#include "server/database/IEventLoggerRequester.h"

class CSQLiteDataProvider;
class CSQLiteRequester;

//--------------------------------------------------------------
/// \Brief		   EventLogger requester for SQLite database
//--------------------------------------------------------------
class CSQLiteEventLoggerRequester: public IEventLoggerRequester
{
public:
	//--------------------------------------------------------------
	/// \Brief		   Constructor
	/// \param [in]	pDatabaseHandler: the database handler
	//--------------------------------------------------------------
	CSQLiteEventLoggerRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

	//--------------------------------------------------------------
	/// \Brief		   Destructor
	//--------------------------------------------------------------
	virtual ~CSQLiteEventLoggerRequester();

   // IEventLoggerRequester implementation
   virtual int addEvent(const std::string & eventType, const std::string & optionalData);
   virtual int addEvent(const CEventLogger & logEntry);
   virtual std::vector<boost::shared_ptr<CEventLogger> > getEvents(); 
   // [END] IEventLoggerRequester implementation

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
