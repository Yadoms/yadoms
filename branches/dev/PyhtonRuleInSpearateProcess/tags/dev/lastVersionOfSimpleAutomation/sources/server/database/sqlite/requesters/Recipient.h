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
         virtual boost::shared_ptr<entities::CRecipient> addRecipient(const entities::CRecipient & recipient);
         virtual boost::shared_ptr<entities::CRecipient> updateRecipient(const entities::CRecipient & recipient);
         virtual std::vector<boost::shared_ptr<entities::CRecipient> > getRecipients();
         virtual boost::shared_ptr<entities::CRecipient> getRecipient(const int recipientId);
         virtual boost::shared_ptr<entities::CRecipient> getRecipient(const std::string & firstName, const std::string & lastName);
         virtual bool exists(const std::string & firstName, const std::string & lastName);
         virtual bool exists(const int id);
         virtual void removeRecipient(int recipientId);
         virtual void removeAllRecipients();
         // [END] IRecipientRequester implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		   Read the recipient fields
         /// \param [in]	recipientToComplete: the recipient entity to complete
         //--------------------------------------------------------------
         void ReadRecipientFields(boost::shared_ptr<database::entities::CRecipient> & recipientToComplete);

         //--------------------------------------------------------------
         /// \Brief		   Write the recipient fields
         /// \param [in]	recipientId: the recipient id
         /// \param [in]	fields: the recipient fields
         //--------------------------------------------------------------
         void WriteRecipientFields(const int recipientId, const std::vector< boost::shared_ptr<database::entities::CRecipientField> > & fields);

         //--------------------------------------------------------------
         /// \Brief		   Reference to SQLiteRequester
         //--------------------------------------------------------------
         boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
      };

   } //namespace requesters
} //namespace sqlite
} //namespace database 

