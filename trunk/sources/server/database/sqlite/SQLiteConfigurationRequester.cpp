#include "stdafx.h"
#include "SQLiteConfigurationRequester.h"
#include "tools/Exceptions/NotImplementedException.hpp"
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/SQLiteDatabaseAdapters.h"

CSQLiteConfigurationRequester::CSQLiteConfigurationRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
   :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
{
}

CSQLiteConfigurationRequester::~CSQLiteConfigurationRequester()
{
}


// IConfigurationRequester implementation
bool CSQLiteConfigurationRequester::create(boost::shared_ptr<CConfiguration> configurationToCreate)
{
   NOT_IMPLEMENTED;
}

boost::shared_ptr<CConfiguration> CSQLiteConfigurationRequester::getConfiguration(const std::string & section, const std::string & name)
{
   CConfigurationAdapter adapter;
   std::ostringstream os;
   os << "SELECT * FROM Configuration WHERE section=\"" << section << "\" AND name=\"" << name << "\"";
   m_databaseRequester->queryEntities<boost::shared_ptr<CConfiguration> >(&adapter, os.str());
   // TODO : g�rer exception si non trouv�
   return adapter.getResults().at(0);
}


bool CSQLiteConfigurationRequester::updateConfiguration(boost::shared_ptr<CConfiguration> configurationToUpdate)
{
   NOT_IMPLEMENTED;
}

bool CSQLiteConfigurationRequester::removeConfiguration(boost::shared_ptr<CConfiguration> configurationToRemove)
{
   NOT_IMPLEMENTED;
}
// [END] IConfigurationRequester implementation




