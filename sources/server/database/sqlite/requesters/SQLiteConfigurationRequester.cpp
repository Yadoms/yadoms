#include "stdafx.h"
#include "SQLiteConfigurationRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"


namespace database { 
namespace sqlite { 
namespace requesters { 

   CSQLiteConfigurationRequester::CSQLiteConfigurationRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
   }

   CSQLiteConfigurationRequester::~CSQLiteConfigurationRequester()
   {
   }


   // IConfigurationRequester implementation
   void CSQLiteConfigurationRequester::create(database::entities::CConfiguration& configurationToCreate)
   {
      CQuery qInsert;
      qInsert. InsertInto(CConfigurationTable::getTableName(), CConfigurationTable::getSectionColumnName(), CConfigurationTable::getNameColumnName(), CConfigurationTable::getValueColumnName(), CConfigurationTable::getDescriptionColumnName(), CConfigurationTable::getDefaultValueColumnName(), CConfigurationTable::getLastModificationDateColumnName()).
               Values(configurationToCreate.getSection(), configurationToCreate.getName(), configurationToCreate.getValue(), configurationToCreate.getDescription(), configurationToCreate.getDefaultValue(), boost::gregorian::day_clock::universal_day());
      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   boost::shared_ptr<database::entities::CConfiguration> CSQLiteConfigurationRequester::getConfiguration(const std::string & section, const std::string & name)
   {
      CQuery qSelect;
      qSelect. Select().
               From(CConfigurationTable::getTableName()).
               Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, section).
               And(CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, name);

      database::sqlite::adapters::CConfigurationAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CConfiguration> >(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
      {
         std::string sEx = (boost::format("Cannot retrieve Configuration Section=%1% and Name=%2% in database") % section % name).str(); 
         throw shared::exception::CEmptyResult(sEx);
      }
   }

   std::vector<boost::shared_ptr<database::entities::CConfiguration> > CSQLiteConfigurationRequester::getConfigurations(const std::string & section)
   {
      CQuery qSelect;
      qSelect. Select().
               From(CConfigurationTable::getTableName()).
               Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, section).
               OrderBy(CConfigurationTable::getNameColumnName(), CQUERY_ORDER_ASC);

      database::sqlite::adapters::CConfigurationAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CConfiguration> >(&adapter, qSelect);
      return adapter.getResults();
   }

   std::vector<boost::shared_ptr<database::entities::CConfiguration> > CSQLiteConfigurationRequester::getConfigurations()
   {
      CQuery qSelect;
      qSelect. Select().
               From(CConfigurationTable::getTableName()).
               OrderBy(CConfigurationTable::getSectionColumnName(), CQUERY_ORDER_ASC, CConfigurationTable::getNameColumnName(), CQUERY_ORDER_ASC);

      database::sqlite::adapters::CConfigurationAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CConfiguration> >(&adapter, qSelect);
      return adapter.getResults();
   }


   void CSQLiteConfigurationRequester::updateConfiguration(database::entities::CConfiguration& configurationToUpdate)
   {
      CQuery qUpdate;
      qUpdate. Update(CConfigurationTable::getTableName()).
               Set(CConfigurationTable::getValueColumnName(), configurationToUpdate.getValue(),
                   CConfigurationTable::getLastModificationDateColumnName(), configurationToUpdate.getLastModificationDate()).
               Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, configurationToUpdate.getSection()).
               And(CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, configurationToUpdate.getName());

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CSQLiteConfigurationRequester::removeConfiguration(database::entities::CConfiguration& configurationToRemove)
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CConfigurationTable::getTableName()).
               Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, configurationToRemove.getSection()).
               And(CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, configurationToRemove.getName());
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw new shared::exception::CEmptyResult("No lines affected");
   }
   // [END] IConfigurationRequester implementation

} //namespace requesters
} //namespace sqlite
} //namespace database 





