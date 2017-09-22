#include "stdafx.h"
#include "Configuration.h"
#include <shared/currentTime/Provider.h>
#include <shared/exception/EmptyResult.hpp>
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"


namespace database
{
   namespace common
   {
      namespace requesters
      {
         CConfiguration::CConfiguration(boost::shared_ptr<IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CConfiguration::~CConfiguration()
         {
         }


         // IConfigurationRequester implementation
         void CConfiguration::create(entities::CConfiguration& configurationToCreate)
         {
            boost::posix_time::ptime insertDate = shared::currentTime::Provider().now();
            auto qInsert = m_databaseRequester->newQuery();
            qInsert->InsertInto(CConfigurationTable::getTableName(), CConfigurationTable::getSectionColumnName(), CConfigurationTable::getNameColumnName(), CConfigurationTable::getValueColumnName(), CConfigurationTable::getDescriptionColumnName(), CConfigurationTable::getDefaultValueColumnName(), CConfigurationTable::getLastModificationDateColumnName()).
               Values(configurationToCreate.Section(), configurationToCreate.Name(), configurationToCreate.Value(), configurationToCreate.Description(), configurationToCreate.DefaultValue(), insertDate);
            if (m_databaseRequester->queryStatement(*qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         bool CConfiguration::exists(const std::string& section, const std::string& name)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->SelectCount().
               From(CConfigurationTable::getTableName()).
               Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, section).
               And(CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, name);
            return (m_databaseRequester->queryCount(*qSelect) == 1);
         }


         boost::shared_ptr<entities::CConfiguration> CConfiguration::getConfiguration(const std::string& section, const std::string& name)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CConfigurationTable::getTableName()).
               Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, section).
               And(CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, name);

            adapters::CConfigurationAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            std::string sEx = (boost::format("Cannot retrieve Configuration Section=%1% and Name=%2% in database") % section % name).str();
            throw shared::exception::CEmptyResult(sEx);
         }

         std::vector<boost::shared_ptr<entities::CConfiguration>> CConfiguration::getConfigurations(const std::string& section)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CConfigurationTable::getTableName()).
               Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, section).
               OrderBy(CConfigurationTable::getNameColumnName());

            adapters::CConfigurationAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CConfiguration>> CConfiguration::getConfigurations()
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CConfigurationTable::getTableName()).
               OrderBy(CConfigurationTable::getSectionColumnName(), CQuery::kAsc, CConfigurationTable::getNameColumnName(), CQuery::kAsc);

            adapters::CConfigurationAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }


         void CConfiguration::updateConfiguration(entities::CConfiguration& configurationToUpdate)
         {
            auto updateDate = shared::currentTime::Provider().now();

            if (exists(configurationToUpdate.Section(), configurationToUpdate.Name()))
            {
               auto qUpdate = m_databaseRequester->newQuery();
               qUpdate->Update(CConfigurationTable::getTableName()).
                  Set(CConfigurationTable::getValueColumnName(), configurationToUpdate.Value(),
                      CConfigurationTable::getLastModificationDateColumnName(), updateDate).
                  Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, configurationToUpdate.Section()).
                  And(CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, configurationToUpdate.Name());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw shared::exception::CEmptyResult("No lines affected");
            }
            else
            {
               return create(configurationToUpdate);
            }
         }

         void CConfiguration::removeConfiguration(entities::CConfiguration& configurationToRemove)
         {
            auto qDelete = m_databaseRequester->newQuery();
            qDelete->DeleteFrom(CConfigurationTable::getTableName()).
               Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, configurationToRemove.Section()).
               And(CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, configurationToRemove.Name());
            if (m_databaseRequester->queryStatement(*qDelete) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         // [END] IConfigurationRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


