#include "stdafx.h"
#include "SQLiteConfigurationRequester.h"
#include "tools/Exceptions/NotImplementedException.hpp"
#include "tools/Exceptions/EmptyResultException.hpp"
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/SQLiteDatabaseAdapters.h"
#include "SQLiteDatabaseTables.h"
#include "Query.h"

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
   CQuery qSelect;
   qSelect. Select().
      From(CConfigurationTable::getTableName()).
      Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_EQUAL, section).
      And(CConfigurationTable::getNameColumnName(), CQUERY_OP_EQUAL, name);

   CConfigurationAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CConfiguration> >(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
      throw new CEmptyResultException("Cannot retrieve inserted Hardware");
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




