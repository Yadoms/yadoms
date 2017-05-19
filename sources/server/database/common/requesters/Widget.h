#pragma once

#include "database/IWidgetRequester.h"
#include "database/IDatabaseRequester.h"

namespace database { namespace common { namespace requesters {

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
         explicit CWidget(boost::shared_ptr<IDatabaseRequester> databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CWidget();

         // IWidgetRequester implementation
         virtual int addWidget(const entities::CWidget & newWidget);
         virtual boost::shared_ptr<entities::CWidget> getWidget(int widgetId);
         virtual std::vector<boost::shared_ptr<entities::CWidget> > getWidgets();
         virtual std::vector<boost::shared_ptr<entities::CWidget> > getWidgetsForPage(int pageId);
         virtual void updateWidgetConfiguration(int widgetId, const std::string& newConfiguration);
         virtual void updateWidgetSize(int widgetId, int sizeX, int sizeY);
         virtual void updateWidgetPosition(int widgetId, int position);
         virtual void updateWidget(const entities::CWidget & widget, bool createIfNotExists);
         virtual void removeWidget(int widgetId);
         virtual void removeWidgetsInPage(int pageId);
         virtual void removeAllWidgets();
         // [END] IWidgetRequester implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		   Reference to IDatabaseRequester
         //--------------------------------------------------------------
         boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
      };

} //namespace requesters
} //namespace common
} //namespace database 

