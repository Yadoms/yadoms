#pragma once

#include "database/IPageRequester.h"
#include "database/IDatabaseRequester.h"


namespace database { namespace common { namespace requesters { 

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
         virtual int addPage(const entities::CPage & page);
         virtual int addPage(const std::string& name, const int pageOrder);
         virtual boost::shared_ptr<entities::CPage> getPage(int pageId);
         virtual std::vector<boost::shared_ptr<entities::CPage> > getPages();
         virtual void updatePage(int pageId, const std::string& name, const int pageOrder);
         virtual void removePage(int pageId);
         virtual void removeAllPages();

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

