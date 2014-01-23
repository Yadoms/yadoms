#pragma once

#include "server/database/IPageRequester.h"
class CSQLiteDataProvider;
class CSQLiteRequester;

//--------------------------------------------------------------
/// \Brief		   Page requester for SQLite database
//--------------------------------------------------------------
class CSQLitePageRequester: public IPageRequester
{
public:
	//--------------------------------------------------------------
	/// \Brief		   Constructor
	/// \param [in]	pDatabaseHandler: the database handler
	//--------------------------------------------------------------
	CSQLitePageRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

	//--------------------------------------------------------------
	/// \Brief		   Destructor
	//--------------------------------------------------------------
	virtual ~CSQLitePageRequester();

   // IPageRequester implementation
   virtual int addPage(const std::string& name);
   virtual boost::shared_ptr<CPage> getPage(int pageId);
   virtual std::vector<boost::shared_ptr<CPage> > getPages();
   virtual void updatePage(int pageId, const std::string& name);
   virtual void removePage(int pageId);
   // [END] IPageRequester implementation

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
