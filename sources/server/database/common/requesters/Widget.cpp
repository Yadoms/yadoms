#include "stdafx.h"
#include "Widget.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"
#include "database/DatabaseException.hpp"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         CWidget::CWidget(boost::shared_ptr<IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CWidget::~CWidget()
         {
         }


         // IWidgetRequester implementation
         int CWidget::addWidget(const entities::CWidget& newWidget)
         {
            auto qInsert = m_databaseRequester->newQuery();
            if (newWidget.Id() != 0)
            {
               qInsert->InsertInto(CWidgetTable::getTableName(), CWidgetTable::getIdColumnName(), CWidgetTable::getIdPageColumnName(), CWidgetTable::getTypeColumnName(), CWidgetTable::getSizeXColumnName(), CWidgetTable::getSizeYColumnName(), CWidgetTable::getPositionColumnName(), CWidgetTable::getTitleColumnName(), CWidgetTable::getConfigurationColumnName()).
                  Values(newWidget.Id(), newWidget.IdPage(), newWidget.Type(), newWidget.SizeX(), newWidget.SizeY(), newWidget.Position(), newWidget.Title(), newWidget.Configuration());
            }
            else
            {
               qInsert->InsertInto(CWidgetTable::getTableName(), CWidgetTable::getIdPageColumnName(), CWidgetTable::getTypeColumnName(), CWidgetTable::getSizeXColumnName(), CWidgetTable::getSizeYColumnName(), CWidgetTable::getPositionColumnName(), CWidgetTable::getTitleColumnName(), CWidgetTable::getConfigurationColumnName()).
                  Values(newWidget.IdPage(), newWidget.Type(), newWidget.SizeX(), newWidget.SizeY(), newWidget.Position(), newWidget.Title(), newWidget.Configuration());
            }
            if (m_databaseRequester->queryStatement(*qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");

            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CWidgetTable::getIdColumnName()).
               From(CWidgetTable::getTableName()).
               Where(CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, newWidget.IdPage()).
               And(CWidgetTable::getTypeColumnName(), CQUERY_OP_EQUAL, newWidget.Type()).
               And(CWidgetTable::getSizeXColumnName(), CQUERY_OP_EQUAL, newWidget.SizeX()).
               And(CWidgetTable::getSizeYColumnName(), CQUERY_OP_EQUAL, newWidget.SizeY()).
               And(CWidgetTable::getPositionColumnName(), CQUERY_OP_EQUAL, newWidget.Position()).
               And(CWidgetTable::getTitleColumnName(), CQUERY_OP_EQUAL, newWidget.Title()).
               And(CWidgetTable::getConfigurationColumnName(), CQUERY_OP_EQUAL, newWidget.Configuration()).
               OrderBy(CWidgetTable::getIdColumnName(), CQuery::kDesc);

            adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];
            throw shared::exception::CEmptyResult("Cannot retrieve inserted Widget");
         }

         boost::shared_ptr<entities::CWidget> CWidget::getWidget(int widgetId)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CWidgetTable::getTableName()).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

            adapters::CWidgetAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, * qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            std::string sEx = (boost::format("Cannot retrieve Widget Id=%1% in database") % widgetId).str();
            throw shared::exception::CEmptyResult(sEx);
         }

         std::vector<boost::shared_ptr<entities::CWidget>> CWidget::getWidgets()
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CWidgetTable::getTableName()).
               OrderBy(CWidgetTable::getPositionColumnName());

            adapters::CWidgetAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, * qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CWidget>> CWidget::getWidgetsForPage(int pageId)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CWidgetTable::getTableName()).
               Where(CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, pageId).
               OrderBy(CWidgetTable::getPositionColumnName());

            adapters::CWidgetAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, * qSelect);
            return adapter.getResults();
         }

         void CWidget::updateWidgetConfiguration(int widgetId, const std::string& newConfiguration)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->Update(CPluginTable::getTableName()).
               Set(CWidgetTable::getConfigurationColumnName(), newConfiguration).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CWidget::updateWidgetSize(int widgetId, int sizeX, int sizeY)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->Update(CPluginTable::getTableName()).
               Set(CWidgetTable::getSizeXColumnName(), sizeX, CWidgetTable::getSizeYColumnName(), sizeY).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CWidget::updateWidgetPosition(int widgetId, int position) const
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->Update(CWidgetTable::getTableName()).
               Set(CWidgetTable::getPositionColumnName(), position).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CWidget::updateWidget(const entities::CWidget& widgetToUpdate, bool createIfNotExists)
         {
            auto qUpdate = m_databaseRequester->newQuery();

            //in all cases the Id must be filled
            if (!widgetToUpdate.Id.isDefined())
               throw CDatabaseException("Need an id to update");

            //search for the widget
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CWidgetTable::getTableName()).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

            adapters::CWidgetAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().size() == 0)
            {
               //do not exist, just add it
               addWidget(widgetToUpdate);
            }
            else
            {
               //already exists, just update fields


               //update type
               if (widgetToUpdate.Type.isDefined())
               {
                  qUpdate->Clear().Update(CWidgetTable::getTableName()).
                     Set(CWidgetTable::getTypeColumnName(), widgetToUpdate.Type()).
                     Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update name");
               }

               //update IdPage
               if (widgetToUpdate.IdPage.isDefined())
               {
                  qUpdate->Clear().Update(CWidgetTable::getTableName()).
                     Set(CWidgetTable::getIdPageColumnName(), widgetToUpdate.IdPage()).
                     Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update id page");
               }

               //update configuration
               if (widgetToUpdate.Configuration.isDefined())
               {
                  qUpdate->Clear().Update(CWidgetTable::getTableName()).
                     Set(CWidgetTable::getConfigurationColumnName(), widgetToUpdate.Configuration()).
                     Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update configuration");
               }

               //update Position
               if (widgetToUpdate.Position.isDefined())
               {
                  qUpdate->Clear().Update(CWidgetTable::getTableName()).
                     Set(CWidgetTable::getPositionColumnName(), widgetToUpdate.Position()).
                     Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update Position");
               }

               //update Size X
               if (widgetToUpdate.SizeX.isDefined())
               {
                  qUpdate->Clear().Update(CWidgetTable::getTableName()).
                     Set(CWidgetTable::getSizeXColumnName(), widgetToUpdate.SizeX()).
                     Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update Size X");
               }

               //update Size Y
               if (widgetToUpdate.SizeY.isDefined())
               {
                  qUpdate->Clear().Update(CWidgetTable::getTableName()).
                     Set(CWidgetTable::getSizeYColumnName(), widgetToUpdate.SizeY()).
                     Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update Size Y");
               }

               //update Title
               if (widgetToUpdate.Title.isDefined())
               {
                  qUpdate->Clear().Update(CWidgetTable::getTableName()).
                     Set(CWidgetTable::getTitleColumnName(), widgetToUpdate.Title()).
                     Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update Title");
               }
            }
         }


         void CWidget::removeWidget(int widgetId)
         {
            auto qDelete = m_databaseRequester->newQuery();
            qDelete->DeleteFrom(CWidgetTable::getTableName()).
               Where(CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);
            if (m_databaseRequester->queryStatement(*qDelete) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CWidget::removeWidgetsInPage(int pageId)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CWidgetTable::getTableName()).
               Where(CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, pageId);

            auto qDelete = m_databaseRequester->newQuery();
            qDelete->DeleteFrom(CWidgetTable::getTableName()).
               Where(CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, pageId);

            m_databaseRequester->queryStatement(*qDelete);
         }

         void CWidget::removeAllWidgets()
         {
            auto qDelete = m_databaseRequester->newQuery();
            qDelete->DeleteFrom(CWidgetTable::getTableName());
            m_databaseRequester->queryStatement(*qDelete);
         }

         // [END] IWidgetRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


