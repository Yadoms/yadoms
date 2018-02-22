#include "stdafx.h"
#include "Configuration2.h"
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
         CConfiguration2::CConfiguration2(boost::shared_ptr<IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CConfiguration2::~CConfiguration2()
         {
         }

         boost::shared_ptr<entities::CConfiguration2> CConfiguration2::getConfiguration(const std::string& section)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CConfiguration2Table::getTableName()).
                     Where(CConfiguration2Table::getSectionColumnName(), CQUERY_OP_LIKE, section);

            adapters::CConfiguration2Adapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            const auto sEx = (boost::format("Cannot retrieve Configuration Section=%1% in database") % section).str();
            throw shared::exception::CEmptyResult(sEx);
         }

         std::vector<boost::shared_ptr<entities::CConfiguration2>> CConfiguration2::getConfigurations()
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CConfiguration2Table::getTableName());

            adapters::CConfiguration2Adapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         void CConfiguration2::updateConfiguration(const std::string& section,
                                                   const std::string& value)
         {
            auto query = m_databaseRequester->newQuery();
            query->InsertOrReplaceInto(CConfiguration2Table::getTableName(),
                                       CConfiguration2Table::getSectionColumnName(),
                                       CConfiguration2Table::getValueColumnName(),
                                       CConfiguration2Table::getLastModificationDateColumnName()).
                   Values(section,
                          value,
                          shared::currentTime::Provider().now());

            if (m_databaseRequester->queryStatement(*query) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }
      } //namespace requesters
   } //namespace common
} //namespace database 
