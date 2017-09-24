#pragma once

#include "database/IWidgetRequester.h"
#include "database/IDatabaseRequester.h"

namespace database
{
   namespace common
   {
      namespace requesters
      {
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
            int addWidget(const entities::CWidget& newWidget) override;
            boost::shared_ptr<entities::CWidget> getWidget(int widgetId) override;
            std::vector<boost::shared_ptr<entities::CWidget>> getWidgets() override;
            std::vector<boost::shared_ptr<entities::CWidget>> getWidgetsForPage(int pageId) override;
            void updateWidgetConfiguration(int widgetId, const std::string& newConfiguration) override;
            void updateWidgetSize(int widgetId, int sizeX, int sizeY) override;
            void updateWidget(const entities::CWidget& widget, bool createIfNotExists) override;
            void removeWidget(int widgetId) override;
            void removeWidgetsInPage(int pageId) override;
            void removeAllWidgets() override;
            // [END] IWidgetRequester implementation

         private:
            void updateWidgetPosition(int widgetId, int position) const;

            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 


