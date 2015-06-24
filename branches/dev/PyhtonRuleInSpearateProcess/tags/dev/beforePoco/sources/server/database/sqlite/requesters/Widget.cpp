#include "stdafx.h"
#include "Widget.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"
#include "database/DatabaseException.hpp"

namespace database { namespace sqlite { namespace requesters { 

   CWidget::CWidget(CSQLiteDataProvider * databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
   }

   CWidget::~CWidget()
   {
   }


   // IWidgetRequester implementation
   int CWidget::addWidget(const database::entities::CWidget & newWidget)
   {
      CQuery qInsert;
      if(newWidget.Id() != 0)
      {
         qInsert. InsertInto(CWidgetTable::getTableName(), CWidgetTable::getIdColumnName(),CWidgetTable::getIdPageColumnName(), CWidgetTable::getNameColumnName(), CWidgetTable::getSizeXColumnName(), CWidgetTable::getSizeYColumnName(), CWidgetTable::getPositionXColumnName(), CWidgetTable::getPositionYColumnName(), CWidgetTable::getConfigurationColumnName()).
            Values(newWidget.Id(), newWidget.IdPage(), newWidget.Name(), newWidget.SizeX(), newWidget.SizeY(), newWidget.PositionX(), newWidget.PositionY(), newWidget.Configuration());
      }
      else
      {
         qInsert. InsertInto(CWidgetTable::getTableName(), CWidgetTable::getIdPageColumnName(), CWidgetTable::getNameColumnName(), CWidgetTable::getSizeXColumnName(), CWidgetTable::getSizeYColumnName(), CWidgetTable::getPositionXColumnName(), CWidgetTable::getPositionYColumnName(), CWidgetTable::getConfigurationColumnName()).
            Values(newWidget.IdPage(), newWidget.Name(), newWidget.SizeX(), newWidget.SizeY(), newWidget.PositionX(), newWidget.PositionY(), newWidget.Configuration());
      }
      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");

      CQuery qSelect;
      qSelect. Select(CWidgetTable::getIdColumnName()).
         From(CWidgetTable::getTableName()).
         Where(CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, newWidget.IdPage()).
         And(CWidgetTable::getNameColumnName(), CQUERY_OP_EQUAL, newWidget.Name()).
         And(CWidgetTable::getSizeXColumnName(), CQUERY_OP_EQUAL, newWidget.SizeX()).
         And(CWidgetTable::getSizeYColumnName(), CQUERY_OP_EQUAL, newWidget.SizeY()).
         And(CWidgetTable::getPositionXColumnName(), CQUERY_OP_EQUAL, newWidget.PositionX()).
         And(CWidgetTable::getPositionYColumnName(), CQUERY_OP_EQUAL, newWidget.PositionY()).
         And(CWidgetTable::getConfigurationColumnName(), CQUERY_OP_EQUAL, newWidget.Configuration()).
         OrderBy(CWidgetTable::getIdColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
         throw shared::exception::CEmptyResult("Cannot retrieve inserted Widget");      
   }

   boost::shared_ptr<database::entities::CWidget> CWidget::getWidget(int widgetId)
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

   std::vector<boost::shared_ptr<database::entities::CWidget> > CWidget::getWidgets()
   {
      CQuery qSelect;
      qSelect. Select().
         From(CWidgetTable::getTableName());

      database::sqlite::adapters::CWidgetAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CWidget> >(&adapter, qSelect);
      return adapter.getResults();
   }

   std::vector<boost::shared_ptr<database::entities::CWidget> > CWidget::getWidgetsForPage(int pageId)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CWidgetTable::getTableName()).
         Where(CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, pageId);

      database::sqlite::adapters::CWidgetAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CWidget> >(&adapter, qSelect);
      return adapter.getResults();
   }

   void CWidget::updateWidgetConfiguration(int widgetId, const std::string& newConfiguration)
   {
      CQuery qUpdate;
      qUpdate. Update(CPluginTable::getTableName()).
         Set(CWidgetTable::getConfigurationColumnName(), newConfiguration).
         Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CWidget::updateWidgetSize(int widgetId, int sizeX, int sizeY)
   {
      CQuery qUpdate;
      qUpdate. Update(CPluginTable::getTableName()).
         Set(CWidgetTable::getSizeXColumnName(), sizeX, CWidgetTable::getSizeYColumnName(), sizeY).
         Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CWidget::updateWidgetPosition(int widgetId, int positionX, int positionY)
   {
      CQuery qUpdate;
      qUpdate. Update(CWidgetTable::getTableName()).
         Set(CWidgetTable::getPositionXColumnName(), positionX, CWidgetTable::getPositionYColumnName(), positionY).
         Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CWidget::updateWidget(const entities::CWidget & widgetToUpdate, bool createIfNotExists)
   {
      CQuery qUpdate;

      //in all cases the Id must be filled
      if(!widgetToUpdate.Id.isDefined())
         throw database::CDatabaseException("Need an id to update");

      //search for the widget
      CQuery qSelect;
      qSelect. Select().
         From(CWidgetTable::getTableName()).
         Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

      database::sqlite::adapters::CWidgetAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CWidget> >(&adapter, qSelect);
      if(adapter.getResults().size() == 0)
      {
         //do not exist, just add it
         addWidget(widgetToUpdate);
      }
      else
      {
         //already exists, just update fields


         //update name
         if(widgetToUpdate.Name.isDefined())
         {
            qUpdate.Clear().Update(CWidgetTable::getTableName()).
               Set(CWidgetTable::getNameColumnName(), widgetToUpdate.Name()).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

            if(m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw database::CDatabaseException("Failed to update name");
         }

         //update IdPage
         if(widgetToUpdate.IdPage.isDefined())
         {
            qUpdate.Clear().Update(CWidgetTable::getTableName()).
               Set(CWidgetTable::getIdPageColumnName(), widgetToUpdate.IdPage()).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

            if(m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw database::CDatabaseException("Failed to update id page");
         }

         //update configuration
         if(widgetToUpdate.Configuration.isDefined())
         {
            qUpdate.Clear().Update(CWidgetTable::getTableName()).
               Set(CWidgetTable::getConfigurationColumnName(), widgetToUpdate.Configuration()).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

            if(m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw database::CDatabaseException("Failed to update configuration");
         }

         //update Position X
         if(widgetToUpdate.PositionX.isDefined())
         {
            qUpdate.Clear().Update(CWidgetTable::getTableName()).
               Set(CWidgetTable::getPositionXColumnName(), widgetToUpdate.PositionX()).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

            if(m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw database::CDatabaseException("Failed to update Position X");
         }

         //update Position Y
         if(widgetToUpdate.PositionY.isDefined())
         {
            qUpdate.Clear().Update(CWidgetTable::getTableName()).
               Set(CWidgetTable::getPositionYColumnName(), widgetToUpdate.PositionY()).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

            if(m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw database::CDatabaseException("Failed to update Position Y");
         }

         //update Size X
         if(widgetToUpdate.SizeX.isDefined())
         {
            qUpdate.Clear().Update(CWidgetTable::getTableName()).
               Set(CWidgetTable::getSizeXColumnName(), widgetToUpdate.SizeX()).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

            if(m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw database::CDatabaseException("Failed to update Size X");
         }

         //update Size Y
         if(widgetToUpdate.SizeY.isDefined())
         {
            qUpdate.Clear().Update(CWidgetTable::getTableName()).
               Set(CWidgetTable::getSizeYColumnName(), widgetToUpdate.SizeY()).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

            if(m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw database::CDatabaseException("Failed to update Size Y");
         }
      }
   }


   void CWidget::removeWidget(int widgetId)
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CWidgetTable::getTableName()).
         Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CWidget::removeWidgetsInPage(int pageId)
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

   void CWidget::removeAllWidgets()
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CWidgetTable::getTableName());
      m_databaseRequester->queryStatement(qDelete);
   }
   // [END] IWidgetRequester implementation


} //namespace requesters
} //namespace sqlite
} //namespace database 



