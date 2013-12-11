#include "stdafx.h"
#include "SQLiteConfigurationRequester.h"
#include "tools/Exceptions/NotImplementedException.h"
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/ConfigurationAdapter.h"

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
   throw CNotImplementedException();
}

boost::shared_ptr<CConfiguration> CSQLiteConfigurationRequester::getConfiguration(const std::string & section, const std::string & name)
{
   CConfigurationAdapter adapter;
   std::ostringstream os;
   os << "SELECT * FROM Configuration WHERE section=\"" << section << "\" AND name=\"" << name << "\"";
   m_databaseRequester->queryEntities<boost::shared_ptr<CConfiguration> >(&adapter, os.str());
   // TODO : gérer exception si non trouvé
   return adapter.getResults().at(0);
}


bool CSQLiteConfigurationRequester::updateConfiguration(boost::shared_ptr<CConfiguration> configurationToUpdate)
{
   throw CNotImplementedException();
}

bool CSQLiteConfigurationRequester::removeConfiguration(boost::shared_ptr<CConfiguration> configurationToRemove)
{
   throw CNotImplementedException();
}
// [END] IConfigurationRequester implementation




