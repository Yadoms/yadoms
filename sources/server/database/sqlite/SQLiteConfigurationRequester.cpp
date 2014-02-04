#include "stdafx.h"
#include "SQLiteConfigurationRequester.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include <shared/Exceptions/EmptyResultException.hpp>
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
void CSQLiteConfigurationRequester::create(boost::shared_ptr<CConfiguration> configurationToCreate)
{
   CQuery qInsert;
   qInsert. InsertInto(CConfigurationTable::getTableName(), CConfigurationTable::getSectionColumnName(), CConfigurationTable::getNameColumnName(), CConfigurationTable::getValueColumnName(), CConfigurationTable::getDescriptionColumnName(), CConfigurationTable::getDefaultValueColumnName(), CConfigurationTable::getLastModificationDateColumnName()).
            Values(configurationToCreate->getSection(), configurationToCreate->getName(), configurationToCreate->getValue(), configurationToCreate->getDescription(), configurationToCreate->getDefaultValue(), boost::gregorian::day_clock::local_day());
   if(m_databaseRequester->queryStatement(qInsert) <= 0)
      throw CEmptyResultException("No lines affected");
}

boost::shared_ptr<CConfiguration> CSQLiteConfigurationRequester::getConfiguration(const std::string & section, const std::string & name)
{
   CQuery qSelect;
   qSelect. Select().
            From(CConfigurationTable::getTableName()).
            Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, section).
            And(CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, name);

   CConfigurationAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CConfiguration> >(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
   {
      std::string sEx = (boost::format("Cannot retrieve Configuration Section=%1% and Name=%2% in database") % section % name).str(); 
      throw CEmptyResultException(sEx);
   }
}

std::vector<boost::shared_ptr<CConfiguration> > CSQLiteConfigurationRequester::getConfigurations(const std::string & section)
{
   CQuery qSelect;
   qSelect. Select().
            From(CConfigurationTable::getTableName()).
            Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, section).
            OrderBy(CConfigurationTable::getNameColumnName(), CQUERY_ORDER_ASC);

   CConfigurationAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CConfiguration> >(&adapter, qSelect);
   return adapter.getResults();
}

std::vector<boost::shared_ptr<CConfiguration> > CSQLiteConfigurationRequester::getConfigurations()
{
   CQuery qSelect;
   qSelect. Select().
            From(CConfigurationTable::getTableName()).
            OrderBy(CConfigurationTable::getSectionColumnName(), CQUERY_ORDER_ASC, CConfigurationTable::getNameColumnName(), CQUERY_ORDER_ASC);

   CConfigurationAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CConfiguration> >(&adapter, qSelect);
   return adapter.getResults();
}


void CSQLiteConfigurationRequester::updateConfiguration(boost::shared_ptr<CConfiguration> configurationToUpdate)
{
   CQuery qUpdate;
   qUpdate. Update(CConfigurationTable::getTableName()).
            Set(CConfigurationTable::getValueColumnName(), configurationToUpdate->getName(),
                CConfigurationTable::getLastModificationDateColumnName(), boost::gregorian::day_clock::local_day()).
            Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, configurationToUpdate->getSection()).
            And(CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, configurationToUpdate->getName());

   if(m_databaseRequester->queryStatement(qUpdate) <= 0)
      throw CEmptyResultException("No lines affected");
}

void CSQLiteConfigurationRequester::removeConfiguration(boost::shared_ptr<CConfiguration> configurationToRemove)
{
   CQuery qDelete;
   qDelete. DeleteFrom(CConfigurationTable::getTableName()).
            Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, configurationToRemove->getSection()).
            And(CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, configurationToRemove->getName());
   if(m_databaseRequester->queryStatement(qDelete) <= 0)
      throw new CEmptyResultException("No lines affected");
}
// [END] IConfigurationRequester implementation




