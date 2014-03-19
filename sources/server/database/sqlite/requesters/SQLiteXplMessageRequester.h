#pragma once

#include "server/database/IXplMessageRequester.h"


namespace database { namespace sqlite { 

   class CSQLiteDataProvider;
   class CSQLiteRequester;

   namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   XplMessage requester for SQLite database
      //--------------------------------------------------------------
      class CSQLiteXplMessageRequester: public IXplMessageRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         CSQLiteXplMessageRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CSQLiteXplMessageRequester();

         // IAcquisitionRequester implementation
         virtual int addXplMessage(const database::entities::CXplMessage & newXplMessage, const std::vector<database::entities::CXplMessageContent> & messageContent);
         // [END] IAcquisitionRequester implementation

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

