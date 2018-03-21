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

         boost::shared_ptr<entities::CConfiguration> CConfiguration::getConfiguration(const std::string& section)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CConfigurationTable::getTableName()).
                     Where(CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, section);

            adapters::CConfigurationAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            const auto sEx = (boost::format("Cannot retrieve Configuration Section=%1% in database") % section).str();
            throw shared::exception::CEmptyResult(sEx);
         }

         std::vector<boost::shared_ptr<entities::CConfiguration>> CConfiguration::getConfigurations()
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CConfigurationTable::getTableName());

            adapters::CConfigurationAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         void CConfiguration::updateConfiguration(const std::string& section,
                                                  const std::string& value)
         {
            auto query = m_databaseRequester->newQuery();
            query->InsertOrReplaceInto(CConfigurationTable::getTableName(),
                                       CConfigurationTable::getSectionColumnName(),
                                       CConfigurationTable::getValueColumnName(),
                                       CConfigurationTable::getLastModificationDateColumnName()).
                   Values(section,
                          value,
                          shared::currentTime::Provider().now());

            if (m_databaseRequester->queryStatement(*query) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }
      } //namespace requesters
   } //namespace common
} //namespace database 
