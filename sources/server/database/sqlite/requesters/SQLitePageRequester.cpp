#include "stdafx.h"
#include "SQLitePageRequester.h"
#include <shared/exceptions/NotImplemented.hpp>
#include <shared/exceptions/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"

namespace server {    namespace database {       namespace sqlite {          namespace requesters { 

   CSQLitePageRequester::CSQLitePageRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
   }

   CSQLitePageRequester::~CSQLitePageRequester()
   {
   }


   // IPageRequester implementation
   int CSQLitePageRequester::addPage(const server::database::entities::CPage & page)
   {

      CQuery qInsert;
      if(page.isIdFilled())
      {
         qInsert. InsertInto(CPageTable::getTableName(), CPageTable::getIdColumnName(), CPageTable::getNameColumnName(), CPageTable::getPageOrderColumnName()).
            Values(page.getId(), page.getName(), page.getPageOrder());
      }
      else
      {
         qInsert. InsertInto(CPageTable::getTableName(), CPageTable::getNameColumnName(), CPageTable::getPageOrderColumnName()).
            Values(page.getName(), page.getPageOrder());
      }

      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");

      CQuery qSelect;
      qSelect. Select(CPageTable::getIdColumnName()).
         From(CPageTable::getTableName()).
         Where(CPageTable::getNameColumnName(), CQUERY_OP_EQUAL, page.getName()).
         OrderBy(CPageTable::getIdColumnName(), CQUERY_ORDER_DESC);

      server::database::sqlite::adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
         throw shared::exception::CEmptyResult("Cannot retrieve inserted Page");      
   }
               

   int CSQLitePageRequester::addPage(const std::string& name, const int pageOrder)
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

      server::database::sqlite::adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
         throw shared::exception::CEmptyResult("Cannot retrieve inserted Page");      
   }



   boost::shared_ptr<server::database::entities::CPage> CSQLitePageRequester::getPage(int pageId)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CPageTable::getTableName()).
         Where(CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);

      server::database::sqlite::adapters::CPageAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<server::database::entities::CPage> >(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
      {
         std::string sEx = (boost::format("Cannot retrieve Page Id=%1% in database") % pageId).str(); 
         throw shared::exception::CEmptyResult(sEx);
      }
   }

   std::vector<boost::shared_ptr<server::database::entities::CPage> > CSQLitePageRequester::getPages()
   {
      CQuery qSelect;
      qSelect. Select().
         From(CPageTable::getTableName());

      server::database::sqlite::adapters::CPageAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<server::database::entities::CPage> >(&adapter, qSelect);
      return adapter.getResults();
   }

   void CSQLitePageRequester::updatePage(int pageId, const std::string& name, const int pageOrder)
   {
      CQuery qUpdate;
      qUpdate.Update(CPageTable::getTableName()).
         Set(CPageTable::getNameColumnName(), name, CPageTable::getPageOrderColumnName(), pageOrder).
         Where(CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CSQLitePageRequester::removePage(int pageId)
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CPageTable::getTableName()).
         Where(CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CSQLitePageRequester::removeAllPages()
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CPageTable::getTableName());
      m_databaseRequester->queryStatement(qDelete);
   }
   // [END] IPageRequester implementation


} //namespace requesters
} //namespace sqlite
} //namespace database 
} //namespace server


