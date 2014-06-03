#pragma once

#include "server/database/IWidgetRequester.h"


namespace database { namespace sqlite { 

   class CSQLiteDataProvider;
   class CSQLiteRequester;

   namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   Widget requester for SQLite database
      //--------------------------------------------------------------
      class CWidget: public IWidgetRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         CWidget(CSQLiteDataProvider * databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CWidget();

         // IWidgetRequester implementation
         virtual int addWidget(const database::entities::CWidget & newWidget);
         virtual boost::shared_ptr<database::entities::CWidget> getWidget(int widgetId);
         virtual std::vector<boost::shared_ptr<database::entities::CWidget> > getWidgets();
         virtual std::vector<boost::shared_ptr<database::entities::CWidget> > getWidgetsForPage(int pageId);
         virtual void updateWidgetConfiguration(int widgetId, const std::string& newConfiguration);
         virtual void updateWidgetSize(int widgetId, int sizeX, int sizeY);
         virtual void updateWidgetPosition(int widgetId, int positionX, int positionY);
         virtual void updateWidget(const entities::CWidget & widget, bool createIfNotExists);
         virtual void removeWidget(int widgetId);
         virtual void removeWidgetsInPage(int pageId);
         virtual void removeAllWidgets();
         // [END] IWidgetRequester implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		   Pointer to SQLiteDatabseHandler
         //--------------------------------------------------------------
         CSQLiteDataProvider * m_databaseHandler;

         //--------------------------------------------------------------
         /// \Brief		   Reference to SQLiteRequester
         //--------------------------------------------------------------
         boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
      };

   } //namespace requesters
} //namespace sqlite
} //namespace database 

