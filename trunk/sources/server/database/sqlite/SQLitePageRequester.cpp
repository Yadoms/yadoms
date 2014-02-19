#include "stdafx.h"
#include "SQLitePageRequester.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include <shared/Exceptions/EmptyResultException.hpp>
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/SQLiteDatabaseAdapters.h"
#include "SQLiteDatabaseTables.h"
#include "Query.h"

CSQLitePageRequester::CSQLitePageRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
   :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
{
}

CSQLitePageRequester::~CSQLitePageRequester()
{
}


// IPageRequester implementation
int CSQLitePageRequester::addPage(const std::string& name, const int pageOrder)
{
   CQuery qInsert;
   qInsert. InsertInto(CPageTable::getTableName(), CPageTable::getNameColumnName(), CPageTable::getPageOrderColumnName()).
            Values(name, pageOrder);
   if(m_databaseRequester->queryStatement(qInsert) <= 0)
      throw CEmptyResultException("No lines affected");
      
 CQuery qSelect;
   qSelect. Select(CPageTable::getIdColumnName()).
            From(CPageTable::getTableName()).
            Where(CPageTable::getNameColumnName(), CQUERY_OP_EQUAL, name).
            OrderBy(CPageTable::getIdColumnName(), CQUERY_ORDER_DESC);

   CSingleValueAdapter<int> adapter;
   m_databaseRequester->queryEntities<int>(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
      throw CEmptyResultException("Cannot retrieve inserted Page");      
}

boost::shared_ptr<CPage> CSQLitePageRequester::getPage(int pageId)
{
   CQuery qSelect;
   qSelect. Select().
            From(CPageTable::getTableName()).
            Where(CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);

   CPageAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CPage> >(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
   {
      std::string sEx = (boost::format("Cannot retrieve Page Id=%1% in database") % pageId).str(); 
      throw CEmptyResultException(sEx);
   }
}

std::vector<boost::shared_ptr<CPage> > CSQLitePageRequester::getPages()
{
   CQuery qSelect;
   qSelect. Select().
            From(CPageTable::getTableName());

   CPageAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CPage> >(&adapter, qSelect);
   return adapter.getResults();
}

void CSQLitePageRequester::updatePage(int pageId, const std::string& name, const int pageOrder)
{
   CQuery qUpdate;
   qUpdate. Update(CHardwareTable::getTableName()).
            Set(CPageTable::getNameColumnName(), name, CPageTable::getPageOrderColumnName(), pageOrder).
            Where(CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);

   if(m_databaseRequester->queryStatement(qUpdate) <= 0)
      throw CEmptyResultException("No lines affected");
}

void CSQLitePageRequester::removePage(int pageId)
{
   CQuery qDelete;
   qDelete. DeleteFrom(CPageTable::getTableName()).
            Where(CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);
   if(m_databaseRequester->queryStatement(qDelete) <= 0)
      throw CEmptyResultException("No lines affected");
}

void CSQLitePageRequester::removeAllPages()
{
   CQuery qDelete;
   qDelete. DeleteFrom(CPageTable::getTableName());
   m_databaseRequester->queryStatement(qDelete);
}






// [END] IPageRequester implementation




