#include "stdafx.h"
#include "Page.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         CPage::CPage(boost::shared_ptr<IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CPage::~CPage()
         {
         }

         // IPageRequester implementation
         int CPage::addPage(const entities::CPage& page)
         {
            auto qInsert = m_databaseRequester->newQuery();
            if (page.Id.isDefined())
            {
               qInsert->InsertInto(CPageTable::getTableName(), CPageTable::getIdColumnName(), CPageTable::getNameColumnName(), CPageTable::getPageOrderColumnName()).
                  Values(page.Id(), page.Name(), page.PageOrder());
            }
            else
            {
               qInsert->InsertInto(CPageTable::getTableName(), CPageTable::getNameColumnName(), CPageTable::getPageOrderColumnName()).
                  Values(page.Name(), page.PageOrder());
            }

            if (m_databaseRequester->queryStatement(*qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");

            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CPageTable::getIdColumnName()).
               From(CPageTable::getTableName()).
               Where(CPageTable::getNameColumnName(), CQUERY_OP_EQUAL, page.Name()).
               OrderBy(CPageTable::getIdColumnName(), CQuery::kDesc);

            adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];
            throw shared::exception::CEmptyResult("Cannot retrieve inserted Page");
         }


         int CPage::addPage(const std::string& name, const int pageOrder)
         {
            auto qInsert = m_databaseRequester->newQuery();
            qInsert->InsertInto(CPageTable::getTableName(), CPageTable::getNameColumnName(), CPageTable::getPageOrderColumnName()).
               Values(name, pageOrder);
            if (m_databaseRequester->queryStatement(*qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");

            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CPageTable::getIdColumnName()).
               From(CPageTable::getTableName()).
               Where(CPageTable::getNameColumnName(), CQUERY_OP_EQUAL, name).
               OrderBy(CPageTable::getIdColumnName(), CQuery::kDesc);

            adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];
            throw shared::exception::CEmptyResult("Cannot retrieve inserted Page");
         }


         boost::shared_ptr<entities::CPage> CPage::getPage(int pageId)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CPageTable::getTableName()).
               Where(CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);

            adapters::CPageAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            std::string sEx = (boost::format("Cannot retrieve Page Id=%1% in database") % pageId).str();
            throw shared::exception::CEmptyResult(sEx);
         }

         std::vector<boost::shared_ptr<entities::CPage>> CPage::getPages()
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CPageTable::getTableName()).
               OrderBy(CPageTable::getPageOrderColumnName());

            adapters::CPageAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         void CPage::updatePage(int pageId, const std::string& name, const int pageOrder)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->Update(CPageTable::getTableName()).
               Set(CPageTable::getNameColumnName(), name, CPageTable::getPageOrderColumnName(), pageOrder).
               Where(CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CPage::removePage(int pageId)
         {
            auto qDelete = m_databaseRequester->newQuery();
            qDelete->DeleteFrom(CPageTable::getTableName()).
               Where(CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);
            if (m_databaseRequester->queryStatement(*qDelete) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CPage::removeAllPages()
         {
            auto qDelete = m_databaseRequester->newQuery();
            qDelete->DeleteFrom(CPageTable::getTableName());
            m_databaseRequester->queryStatement(*qDelete);
         }

         // [END] IPageRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


