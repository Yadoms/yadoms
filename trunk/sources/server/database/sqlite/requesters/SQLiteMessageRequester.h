#pragma once

#include "server/database/IMessageRequester.h"


namespace database { namespace sqlite { 

   class CSQLiteDataProvider;
   class CSQLiteRequester;

   namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   XplMessage requester for SQLite database
      //--------------------------------------------------------------
      class CSQLiteMessageRequester: public IMessageRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         CSQLiteMessageRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CSQLiteMessageRequester();

         // IAcquisitionRequester implementation
         virtual int insertMessage(const database::entities::CMessage & newMessage, const std::vector<database::entities::CMessageContent> & messageContent);
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

