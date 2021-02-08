#pragma once

#include "database/IRecipientRequester.h"
#include "database/IDatabaseRequester.h"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   Recipient requester for SQLite database
         //--------------------------------------------------------------
         class CRecipient: public IRecipientRequester
         {
         public:
            explicit CRecipient(boost::shared_ptr<IDatabaseRequester> databaseRequester);
            virtual ~CRecipient() = default;

            // IRecipientRequester implementation
            boost::shared_ptr<entities::CRecipient> addRecipient(const entities::CRecipient& recipient) override;
            boost::shared_ptr<entities::CRecipient> updateRecipient(const entities::CRecipient& recipient) override;
            std::vector<boost::shared_ptr<entities::CRecipient>> getRecipients() override;
            boost::shared_ptr<entities::CRecipient> getRecipient(const int recipientId) override;
            boost::shared_ptr<entities::CRecipient> getRecipient(const std::string& firstName, const std::string& lastName) override;
            bool exists(const std::string& firstName, const std::string& lastName) override;
            bool exists(const int id) override;
            void removeRecipient(int recipientId) override;
            void removeAllRecipients() override;
            bool fieldExists(const std::string& fieldName, const std::string& pluginName) const override;
            boost::shared_ptr<entities::CRecipientField> createField(const entities::CRecipientField& field) override;
            std::vector<boost::shared_ptr<entities::CRecipient>> findRecipientsFromField(const std::string& fieldName, const std::string& expectedFieldValue) override;
            std::vector<boost::shared_ptr<entities::CRecipientField>> getFields() override;
            std::vector<boost::shared_ptr<entities::CRecipientField>> getFieldsByName(const std::string& fieldName) override;
            // [END] IRecipientRequester implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   Read the recipient fields
            /// \param [in]	recipientToComplete: the recipient entity to complete
            //--------------------------------------------------------------
            void ReadRecipientFields(boost::shared_ptr<database::entities::CRecipient>& recipientToComplete) const;

            //--------------------------------------------------------------
            /// \Brief		   Write the recipient fields
            /// \param [in]	recipientId: the recipient id
            /// \param [in]	fields: the recipient fields
            //--------------------------------------------------------------
            void WriteRecipientFields(const int recipientId, const std::vector<boost::shared_ptr<database::entities::CRecipientField>>& fields) const;

            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 


