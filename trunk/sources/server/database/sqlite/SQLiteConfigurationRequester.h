#pragma once

#include "server/database/IConfigurationRequester.h"
class CSQLiteDataProvider;
class CSQLiteRequester;

//--------------------------------------------------------------
/// \Brief		   Configuration requester for SQLite database
//--------------------------------------------------------------
class CSQLiteConfigurationRequester: public IConfigurationRequester
{
public:
	//--------------------------------------------------------------
	/// \Brief		   Constructor
	/// \param [in]	pDatabaseHandler: the database handler
	//--------------------------------------------------------------
	CSQLiteConfigurationRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

	//--------------------------------------------------------------
	/// \Brief		   Destructor
	//--------------------------------------------------------------
	virtual ~CSQLiteConfigurationRequester();

   // IConfigurationRequester implementation
   virtual void create(CConfiguration& configurationToCreate);
	virtual boost::shared_ptr<CConfiguration> getConfiguration(const std::string & section, const std::string & name);
	virtual std::vector<boost::shared_ptr<CConfiguration> > getConfigurations(const std::string & section);
	virtual std::vector<boost::shared_ptr<CConfiguration> > getConfigurations();
	virtual void updateConfiguration(CConfiguration& configurationToUpdate);
   virtual void removeConfiguration(CConfiguration& configurationToRemove);
   // [END] IConfigurationRequester implementation

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
