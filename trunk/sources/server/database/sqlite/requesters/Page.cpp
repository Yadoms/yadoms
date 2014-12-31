#include "stdafx.h"
#include "Page.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"

namespace database {  namespace sqlite { namespace requesters { 

   CPage::CPage(boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseRequester(databaseRequester)
   {
   }

   CPage::~CPage()
   {
   }


   // IPageRequester implementation
   int CPage::addPage(const entities::CPage & page)
   {

      CQuery qInsert;
      if(page.Id.isDefined())
      {
         qInsert. InsertInto(CPageTable::getTableName(), CPageTable::getIdColumnName(), CPageTable::getNameColumnName(), CPageTable::getPageOrderColumnName()).
            Values(page.Id(), page.Name(), page.PageOrder());
      }
      else
      {
         qInsert. InsertInto(CPageTable::getTableName(), CPageTable::getNameColumnName(), CPageTable::getPageOrderColumnName()).
            Values(page.Name(), page.PageOrder());
      }

      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");

      CQuery qSelect;
      qSelect. Select(CPageTable::getIdColumnName()).
         From(CPageTable::getTableName()).
         Where(CPageTable::getNameColumnName(), CQUERY_OP_EQUAL, page.Name()).
         OrderBy(CPageTable::getIdColumnName(), CQUERY_ORDER_DESC);

      adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      throw shared::exception::CEmptyResult("Cannot retrieve inserted Page");
   }


   int CPage::addPage(const std::string& name, const int pageOrder)
   {
      CQuery qInsert;
      qInsert. InsertInto(CPageTable::getTableName(), CPageTable::getNameColumnName(), CPageTable::getPageOrderColumnName()).
         Values(name, pageOrder);
      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");

      CQuery qSelect;
      qSelect. Select(CPageTable::getIdColumnName()).
         From(CPageTable::getTableName()).
         Where(CPageTable::getNameColumnName(), CQUERY_OP_EQUAL, name).
         OrderBy(CPageTable::getIdColumnName(), CQUERY_ORDER_DESC);

      adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      throw shared::exception::CEmptyResult("Cannot retrieve inserted Page");
   }



   boost::shared_ptr<entities::CPage> CPage::getPage(int pageId)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CPageTable::getTableName()).
         Where(CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);

      adapters::CPageAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<entities::CPage> >(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      
      std::string sEx = (boost::format("Cannot retrieve Page Id=%1% in database") % pageId).str(); 
      throw shared::exception::CEmptyResult(sEx);
   }

   std::vector<boost::shared_ptr<entities::CPage> > CPage::getPages()
   {
      CQuery qSelect;
      qSelect. Select().
         From(CPageTable::getTableName()).
         OrderBy(CPageTable::getPageOrderColumnName());

      adapters::CPageAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<entities::CPage> >(&adapter, qSelect);
      return adapter.getResults();
   }

   void CPage::updatePage(int pageId, const std::string& name, const int pageOrder)
   {
      CQuery qUpdate;
      qUpdate.Update(CPageTable::getTableName()).
         Set(CPageTable::getNameColumnName(), name, CPageTable::getPageOrderColumnName(), pageOrder).
         Where(CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CPage::removePage(int pageId)
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CPageTable::getTableName()).
         Where(CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CPage::removeAllPages()
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CPageTable::getTableName());
      m_databaseRequester->queryStatement(qDelete);
   }
   // [END] IPageRequester implementation


} //namespace requesters
} //namespace sqlite
} //namespace database 



