#include "stdafx.h"
#include "SQLiteWidgetRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"
#include "database/DatabaseException.hpp"

namespace database { namespace sqlite { namespace requesters { 

   CSQLiteWidgetRequester::CSQLiteWidgetRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
   }

   CSQLiteWidgetRequester::~CSQLiteWidgetRequester()
   {
   }


   // IWidgetRequester implementation
   int CSQLiteWidgetRequester::addWidget(const database::entities::CWidget & newWidget)
   {
      CQuery qInsert;
      if(newWidget.getId() != 0)
      {
         qInsert. InsertInto(CWidgetTable::getTableName(), CWidgetTable::getIdColumnName(),CWidgetTable::getIdPageColumnName(), CWidgetTable::getNameColumnName(), CWidgetTable::getSizeXColumnName(), CWidgetTable::getSizeYColumnName(), CWidgetTable::getPositionXColumnName(), CWidgetTable::getPositionYColumnName(), CWidgetTable::getConfigurationColumnName()).
            Values(newWidget.getId(), newWidget.getIdPage(), newWidget.getName(), newWidget.getSizeX(), newWidget.getSizeY(), newWidget.getPositionX(), newWidget.getPositionY(), newWidget.getConfiguration());
      }
      else
      {
         qInsert. InsertInto(CWidgetTable::getTableName(), CWidgetTable::getIdPageColumnName(), CWidgetTable::getNameColumnName(), CWidgetTable::getSizeXColumnName(), CWidgetTable::getSizeYColumnName(), CWidgetTable::getPositionXColumnName(), CWidgetTable::getPositionYColumnName(), CWidgetTable::getConfigurationColumnName()).
            Values(newWidget.getIdPage(), newWidget.getName(), newWidget.getSizeX(), newWidget.getSizeY(), newWidget.getPositionX(), newWidget.getPositionY(), newWidget.getConfiguration());
      }
      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");

      CQuery qSelect;
      qSelect. Select(CWidgetTable::getIdColumnName()).
         From(CWidgetTable::getTableName()).
         Where(CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, newWidget.getIdPage()).
         And(CWidgetTable::getNameColumnName(), CQUERY_OP_EQUAL, newWidget.getName()).
         And(CWidgetTable::getSizeXColumnName(), CQUERY_OP_EQUAL, newWidget.getSizeX()).
         And(CWidgetTable::getSizeYColumnName(), CQUERY_OP_EQUAL, newWidget.getSizeY()).
         And(CWidgetTable::getPositionXColumnName(), CQUERY_OP_EQUAL, newWidget.getPositionX()).
         And(CWidgetTable::getPositionYColumnName(), CQUERY_OP_EQUAL, newWidget.getPositionY()).
         And(CWidgetTable::getConfigurationColumnName(), CQUERY_OP_EQUAL, newWidget.getConfiguration()).
         OrderBy(CWidgetTable::getIdColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
         throw shared::exception::CEmptyResult("Cannot retrieve inserted Widget");      
   }

   boost::shared_ptr<database::entities::CWidget> CSQLiteWidgetRequester::getWidget(int widgetId)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CWidgetTable::getTableName()).
         Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

      database::sqlite::adapters::CWidgetAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CWidget> >(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
      {
         std::string sEx = (boost::format("Cannot retrieve Widget Id=%1% in database") % widgetId).str(); 
         throw shared::exception::CEmptyResult(sEx);
      }
   }

   std::vector<boost::shared_ptr<database::entities::CWidget> > CSQLiteWidgetRequester::getWidgets()
   {
      CQuery qSelect;
      qSelect. Select().
         From(CWidgetTable::getTableName());

      database::sqlite::adapters::CWidgetAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CWidget> >(&adapter, qSelect);
      return adapter.getResults();
   }

   std::vector<boost::shared_ptr<database::entities::CWidget> > CSQLiteWidgetRequester::getWidgetsForPage(int pageId)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CWidgetTable::getTableName()).
         Where(CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, pageId);

      database::sqlite::adapters::CWidgetAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CWidget> >(&adapter, qSelect);
      return adapter.getResults();
   }

   void CSQLiteWidgetRequester::updateWidgetConfiguration(int widgetId, const std::string& newConfiguration)
   {
      CQuery qUpdate;
      qUpdate. Update(CPluginTable::getTableName()).
         Set(CWidgetTable::getConfigurationColumnName(), newConfiguration).
         Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CSQLiteWidgetRequester::updateWidgetSize(int widgetId, int sizeX, int sizeY)
   {
      CQuery qUpdate;
      qUpdate. Update(CPluginTable::getTableName()).
         Set(CWidgetTable::getSizeXColumnName(), sizeX, CWidgetTable::getSizeYColumnName(), sizeY).
         Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CSQLiteWidgetRequester::updateWidgetPosition(int widgetId, int positionX, int positionY)
   {
      CQuery qUpdate;
      qUpdate. Update(CWidgetTable::getTableName()).
         Set(CWidgetTable::getPositionXColumnName(), positionX, CWidgetTable::getPositionYColumnName(), positionY).
         Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CSQLiteWidgetRequester::updateWidget(const entities::CWidget & widgetToUpdate)
   {
      CQuery qUpdate;

      if(!widgetToUpdate.isIdFilled())
         throw database::CDatabaseException("Need an id to update");

      //update name
      if(widgetToUpdate.isNameFilled())
      {
         qUpdate.CLear().Update(CWidgetTable::getTableName()).
            Set(CWidgetTable::getNameColumnName(), widgetToUpdate.getName()).
            Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.getId());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update name");
      }

      //update IdPage
      if(widgetToUpdate.isIdPageFilled())
      {
         qUpdate.CLear().Update(CWidgetTable::getTableName()).
            Set(CWidgetTable::getIdPageColumnName(), widgetToUpdate.getIdPage()).
            Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.getId());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update id page");
      }

      //update configuration
      if(widgetToUpdate.isConfigurationFilled())
      {
         qUpdate.CLear().Update(CWidgetTable::getTableName()).
            Set(CWidgetTable::getConfigurationColumnName(), widgetToUpdate.getConfiguration()).
            Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.getId());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update configuration");
      }

      //update Position X
      if(widgetToUpdate.isPositionXFilled())
      {
         qUpdate.CLear().Update(CWidgetTable::getTableName()).
            Set(CWidgetTable::getPositionXColumnName(), widgetToUpdate.getPositionX()).
            Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.getId());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update Position X");
      }

      //update Position Y
      if(widgetToUpdate.isPositionYFilled())
      {
         qUpdate.CLear().Update(CWidgetTable::getTableName()).
            Set(CWidgetTable::getPositionYColumnName(), widgetToUpdate.getPositionY()).
            Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.getId());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update Position Y");
      }

      //update Size X
      if(widgetToUpdate.isSizeXFilled())
      {
         qUpdate.CLear().Update(CWidgetTable::getTableName()).
            Set(CWidgetTable::getSizeXColumnName(), widgetToUpdate.getSizeX()).
            Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.getId());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update Size X");
      }

      //update Size Y
      if(widgetToUpdate.isSizeYFilled())
      {
         qUpdate.CLear().Update(CWidgetTable::getTableName()).
            Set(CWidgetTable::getSizeYColumnName(), widgetToUpdate.getSizeY()).
            Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.getId());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update Size Y");
      }
   }


   void CSQLiteWidgetRequester::removeWidget(int widgetId)
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CWidgetTable::getTableName()).
         Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
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


} //namespace requesters
} //namespace sqlite
} //namespace database 



