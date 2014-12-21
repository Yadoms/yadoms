#pragma once

#include "server/database/IRecipientRequester.h"


namespace database { namespace sqlite { 

   class CSQLiteDataProvider;
   class CSQLiteRequester;

   namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   Recipient requester for SQLite database
      //--------------------------------------------------------------
      class CRecipient: public IRecipientRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         CRecipient(boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CRecipient();

         // IRecipientRequester implementation
         virtual int addRecipient(const database::entities::CRecipient & recipient);
         virtual std::vector<boost::shared_ptr<database::entities::CRecipient> > getRecipients();
         virtual void removeRecipient(int recipientId);
         virtual void removeAllRecipients();
         // [END] IRecipientRequester implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		   Reference to SQLiteRequester
         //--------------------------------------------------------------
         boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
      };

   } //namespace requesters
} //namespace sqlite
} //namespace database 

