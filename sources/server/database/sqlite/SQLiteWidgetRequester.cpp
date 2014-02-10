#include "stdafx.h"
#include "SQLiteWidgetRequester.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include <shared/Exceptions/EmptyResultException.hpp>
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/SQLiteDatabaseAdapters.h"
#include "SQLiteDatabaseTables.h"
#include "Query.h"

CSQLiteWidgetRequester::CSQLiteWidgetRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
   :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
{
}

CSQLiteWidgetRequester::~CSQLiteWidgetRequester()
{
}


// IWidgetRequester implementation
int CSQLiteWidgetRequester::addWidget(const CWidget & newWidget)
{
   CQuery qInsert;
   if(newWidget.getId() != 0)
   {
      qInsert. InsertInto(CWidgetTable::getTableName(), CWidgetTable::getIdColumnName(),CWidgetTable::getIdPageColumnName(), CWidgetTable::getNameColumnName(), CWidgetTable::getSizeXColumnName(), CWidgetTable::getSizeYColumnName(), CWidgetTable::getConfigurationColumnName()).
         Values(newWidget.getId(), newWidget.getIdPage(), newWidget.getName(), newWidget.getSizeX(), newWidget.getSizeY(), newWidget.getConfiguration());
   }
   else
   {
      qInsert. InsertInto(CWidgetTable::getTableName(), CWidgetTable::getIdPageColumnName(), CWidgetTable::getNameColumnName(), CWidgetTable::getSizeXColumnName(), CWidgetTable::getSizeYColumnName(), CWidgetTable::getConfigurationColumnName()).
         Values(newWidget.getIdPage(), newWidget.getName(), newWidget.getSizeX(), newWidget.getSizeY(), newWidget.getConfiguration());
   }
   if(m_databaseRequester->queryStatement(qInsert) <= 0)
      throw CEmptyResultException("No lines affected");

   CQuery qSelect;
   qSelect. Select(CWidgetTable::getIdColumnName()).
      From(CWidgetTable::getTableName()).
      Where(CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, newWidget.getIdPage()).
      And(CWidgetTable::getNameColumnName(), CQUERY_OP_EQUAL, newWidget.getName()).
      And(CWidgetTable::getSizeXColumnName(), CQUERY_OP_EQUAL, newWidget.getSizeX()).
      And(CWidgetTable::getSizeYColumnName(), CQUERY_OP_EQUAL, newWidget.getSizeY()).
      And(CWidgetTable::getConfigurationColumnName(), CQUERY_OP_EQUAL, newWidget.getConfiguration()).
      OrderBy(CWidgetTable::getIdColumnName(), CQUERY_ORDER_DESC);

   CSingleValueAdapter<int> adapter;
   m_databaseRequester->queryEntities<int>(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
      throw CEmptyResultException("Cannot retrieve inserted Widget");      
}

boost::shared_ptr<CWidget> CSQLiteWidgetRequester::getWidget(int widgetId)
{
   CQuery qSelect;
   qSelect. Select().
      From(CWidgetTable::getTableName()).
      Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

   CWidgetAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CWidget> >(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
   {
      std::string sEx = (boost::format("Cannot retrieve Widget Id=%1% in database") % widgetId).str(); 
      throw CEmptyResultException(sEx);
   }
}

std::vector<boost::shared_ptr<CWidget> > CSQLiteWidgetRequester::getWidgets()
{
   CQuery qSelect;
   qSelect. Select().
      From(CWidgetTable::getTableName());

   CWidgetAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CWidget> >(&adapter, qSelect);
   return adapter.getResults();
}

std::vector<boost::shared_ptr<CWidget> > CSQLiteWidgetRequester::getWidgetsForPage(int pageId)
{
   CQuery qSelect;
   qSelect. Select().
      From(CWidgetTable::getTableName()).
      Where(CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, pageId);

   CWidgetAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CWidget> >(&adapter, qSelect);
   return adapter.getResults();
}

void CSQLiteWidgetRequester::updateWidgetConfiguration(int widgetId, const std::string& newConfiguration)
{
   CQuery qUpdate;
   qUpdate. Update(CHardwareTable::getTableName()).
      Set(CWidgetTable::getConfigurationColumnName(), newConfiguration).
      Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

   if(m_databaseRequester->queryStatement(qUpdate) <= 0)
      throw CEmptyResultException("No lines affected");
}

void CSQLiteWidgetRequester::updateWidgetSize(int widgetId, int sizeX, int sizeY)
{
   CQuery qUpdate;
   qUpdate. Update(CHardwareTable::getTableName()).
      Set(CWidgetTable::getSizeXColumnName(), sizeX, CWidgetTable::getSizeYColumnName(), sizeY).
      Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

   if(m_databaseRequester->queryStatement(qUpdate) <= 0)
      throw CEmptyResultException("No lines affected");
}


void CSQLiteWidgetRequester::removeWidget(int widgetId)
{
   CQuery qDelete;
   qDelete. DeleteFrom(CWidgetTable::getTableName()).
      Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);
   if(m_databaseRequester->queryStatement(qDelete) <= 0)
      throw CEmptyResultException("No lines affected");
}

void CSQLiteWidgetRequester::removeWidgetsInPage(int pageId)
{
      CQuery qSelect;
   qSelect. Select().
      From(CWidgetTable::getTableName()).
      Where(CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, pageId);

   CQuery qDelete;
   qDelete. DeleteFrom(CWidgetTable::getTableName()).
            Where(CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, pageId);

   m_databaseRequester->queryStatement(qDelete);
}

void CSQLiteWidgetRequester::removeAllWidgets()
{
   CQuery qDelete;
   qDelete. DeleteFrom(CWidgetTable::getTableName());
   m_databaseRequester->queryStatement(qDelete);
}






// [END] IWidgetRequester implementation




