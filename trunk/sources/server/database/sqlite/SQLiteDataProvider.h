#pragma once

#include "server/database/IDataProvider.h"
#include "SQLiteHardwareRequester.h"
#include "SQLiteConfigurationRequester.h"
#include "SQLiteAcquisitionRequester.h"
#include "SQLiteDeviceRequester.h"
#include "SQLiteKeywordRequester.h"
#include "SQLitePageRequester.h"
#include "SQLiteWidgetRequester.h"
#include "SQLiteRequester.h"
#include "sqlite3.h"


//--------------------------------------------------------------
/// \Brief		   Database provider for SQLite
//--------------------------------------------------------------
class CSQLiteDataProvider : public IDataProvider
{
public:
	//--------------------------------------------------------------
	/// \Brief		   Constructor
	/// \param [in]	pConfig: startup configuration
   /// \throw        CNotSupportedException if the linked SQLIte library is compiled without ThreadSafe options
	//--------------------------------------------------------------
	CSQLiteDataProvider(const std::string & dbFile);
	
	//--------------------------------------------------------------
	/// \Brief		   Destructor
	//--------------------------------------------------------------
	virtual ~CSQLiteDataProvider();

   // IDatabaseProvider implementation
	bool load();
	void unload();
   boost::shared_ptr<IHardwareRequester> getHardwareRequester() { return m_hardwareRequester; }
   boost::shared_ptr<IConfigurationRequester> getConfigurationRequester() { return m_configurationRequester; }
   boost::shared_ptr<IDeviceRequester> getDeviceRequester() { return m_deviceRequester; }
   boost::shared_ptr<IAcquisitionRequester> getAcquisitionRequester() { return m_acquisitionRequester; }
   boost::shared_ptr<IKeywordRequester> getKeywordRequester() { return m_keywordRequester; }
   boost::shared_ptr<IPageRequester> getPageRequester() { return m_pageRequester; }
   boost::shared_ptr<IWidgetRequester> getWidgetRequester() { return m_widgetRequester; }


   // [END] IHardwarePlugin implementation

private:
	//--------------------------------------------------------------
	/// \Brief		Load all requesters
	//--------------------------------------------------------------
	void loadRequesters();
	
private:
	//--------------------------------------------------------------
	/// \Brief		Hardware requester
	//--------------------------------------------------------------
	boost::shared_ptr<CSQLiteHardwareRequester>		m_hardwareRequester;
	
	//--------------------------------------------------------------
	/// \Brief		Hardware requester
	//--------------------------------------------------------------
	boost::shared_ptr<CSQLiteConfigurationRequester>		m_configurationRequester;

	//--------------------------------------------------------------
	/// \Brief		Device requester
	//--------------------------------------------------------------
	boost::shared_ptr<CSQLiteDeviceRequester>		m_deviceRequester;

	//--------------------------------------------------------------
	/// \Brief		Acquisition requester
	//--------------------------------------------------------------
	boost::shared_ptr<CSQLiteAcquisitionRequester>		m_acquisitionRequester;

	//--------------------------------------------------------------
	/// \Brief		Keyword requester
	//--------------------------------------------------------------
	boost::shared_ptr<CSQLiteKeywordRequester>		m_keywordRequester;

	//--------------------------------------------------------------
	/// \Brief		Page requester
	//--------------------------------------------------------------
	boost::shared_ptr<CSQLitePageRequester>		m_pageRequester;

	//--------------------------------------------------------------
	/// \Brief		Widget requester
	//--------------------------------------------------------------
	boost::shared_ptr<CSQLiteWidgetRequester>		m_widgetRequester;

   //--------------------------------------------------------------
	/// \Brief		Startup options
	//--------------------------------------------------------------
	const std::string m_dbFile;

	//--------------------------------------------------------------
	/// \Brief		database handler
	//--------------------------------------------------------------
   sqlite3 * m_pDatabaseHandler; //not use shared_ptr because not alllocated by this library

	//--------------------------------------------------------------
	/// \Brief		database requester (for SQL queries)
	//--------------------------------------------------------------
   boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
};