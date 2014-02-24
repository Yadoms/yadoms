#pragma once

#include "server/database/IPageRequester.h"


   namespace database { 
      namespace sqlite { 

         class CSQLiteDataProvider;
         class CSQLiteRequester;

         namespace requesters { 

            //--------------------------------------------------------------
            /// \Brief		   Page requester for SQLite database
            //--------------------------------------------------------------
            class CSQLitePageRequester: public IPageRequester
            {
            public:
               //--------------------------------------------------------------
               /// \Brief		   Constructor
               /// \param [in]	pDatabaseHandler: the database handler
               //--------------------------------------------------------------
               CSQLitePageRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

               //--------------------------------------------------------------
               /// \Brief		   Destructor
               //--------------------------------------------------------------
               virtual ~CSQLitePageRequester();

               // IPageRequester implementation
               virtual int addPage(const database::entities::CPage & page);
               virtual int addPage(const std::string& name, const int pageOrder);
               virtual boost::shared_ptr<database::entities::CPage> getPage(int pageId);
               virtual std::vector<boost::shared_ptr<database::entities::CPage> > getPages();
               virtual void updatePage(int pageId, const std::string& name, const int pageOrder);
               virtual void removePage(int pageId);
               virtual void removeAllPages();

               // [END] IPageRequester implementation

            private:
               //--------------------------------------------------------------
               /// \Brief		   Reference to SQLiteDatabseHandler
               //--------------------------------------------------------------
               const CSQLiteDataProvider & m_databaseHandler;

               //--------------------------------------------------------------
               /// \Brief		   Reference to SQLiteRequester
               //--------------------------------------------------------------
               boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
            };

         } //namespace requesters
      } //namespace sqlite
   } //namespace database 

