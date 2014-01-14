#pragma once

#include "server/database/IAcquisitionRequester.h"
class CSQLiteDataProvider;
class CSQLiteRequester;

//--------------------------------------------------------------
/// \Brief		   Acquisition requester for SQLite database
//--------------------------------------------------------------
class CSQLiteAcquisitionRequester: public IAcquisitionRequester
{
public:
	//--------------------------------------------------------------
	/// \Brief		   Constructor
	/// \param [in]	pDatabaseHandler: the database handler
	//--------------------------------------------------------------
	CSQLiteAcquisitionRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

	//--------------------------------------------------------------
	/// \Brief		   Destructor
	//--------------------------------------------------------------
	virtual ~CSQLiteAcquisitionRequester();

   // IAcquisitionRequester implementation
   virtual void addAcquisition(const CAcquisition & newAcquisition);
   virtual boost::shared_ptr<CAcquisition> getAcquisition(int acquisitionId);
   virtual std::vector<boost::shared_ptr<CAcquisition> > getAcquisitions(const std::string & source, const std::string & keyword);
   virtual void removeAcquisition(int acquisitionId);   
   // [END] IAcquisitionRequester implementation

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
