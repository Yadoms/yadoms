#pragma once

#include "database/IPageRequester.h"
#include "database/IDatabaseRequester.h"


namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   Page requester for SQLite database
         //--------------------------------------------------------------
         class CPage: public IPageRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	pDatabaseHandler: the database handler
            //--------------------------------------------------------------
            explicit CPage(boost::shared_ptr<IDatabaseRequester> databaseRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CPage();

            // IPageRequester implementation
            int addPage(const entities::CPage& page) override;
            int addPage(const std::string& name, const int pageOrder) override;
            boost::shared_ptr<entities::CPage> getPage(int pageId) override;
            std::vector<boost::shared_ptr<entities::CPage>> getPages() override;
            void updatePage(int pageId, const std::string& name, const int pageOrder) override;
            void removePage(int pageId) override;
            void removeAllPages() override;
            // [END] IPageRequester implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 


