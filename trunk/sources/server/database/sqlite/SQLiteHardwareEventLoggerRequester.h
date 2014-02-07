#pragma once

#include "server/database/IHardwareEventLoggerRequester.h"
class CSQLiteDataProvider;
class CSQLiteRequester;

//--------------------------------------------------------------
/// \Brief		   HardwareEventLogger requester for SQLite database
//--------------------------------------------------------------
class CSQLiteHardwareEventLoggerRequester: public IHardwareEventLoggerRequester
{
public:
	//--------------------------------------------------------------
	/// \Brief		   Constructor
	/// \param [in]	pDatabaseHandler: the database handler
	//--------------------------------------------------------------
	CSQLiteHardwareEventLoggerRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

	//--------------------------------------------------------------
	/// \Brief		   Destructor
	//--------------------------------------------------------------
	virtual ~CSQLiteHardwareEventLoggerRequester();

   // IHardwareEventLoggerRequester implementation
   virtual int addEvent(const std::string & pluginName, const std::string & pluginVersion, const IHardwarePluginInformation::EReleaseType pluginReleaseType, const CHardwareEventLogger::EEventType eventType, const std::string & message = CStringExtension::EmptyString);
   virtual int addEvent(const CHardwareEventLogger & hardwareLogEntry);
   virtual std::vector<boost::shared_ptr<CHardwareEventLogger> > getHardwareEvents(const std::string & pluginName, const std::string & pluginVersion, const IHardwarePluginInformation::EReleaseType pluginReleaseType, const boost::posix_time::ptime & fromDate);
   // [END] IHardwareEventLoggerRequester implementation

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
