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
         class CRecipient final : public IRecipientRequester
         {
         public:
            explicit CRecipient(boost::shared_ptr<IDatabaseRequester> databaseRequester);
            ~CRecipient() override = default;

            // IRecipientRequester implementation
            boost::shared_ptr<entities::CRecipient> addRecipient(const entities::CRecipient& recipient) override;
            int createUser(const entities::CRecipient& user) override;
            boost::shared_ptr<entities::CRecipient> updateRecipient(const entities::CRecipient& recipient) override;
            void updateUser(const entities::CRecipient& user) override;
            std::vector<boost::shared_ptr<entities::CRecipient>> getRecipients() override;
            std::vector<boost::shared_ptr<entities::CRecipient>> getUsers(const boost::optional<int>& userId,
                                                                          const boost::optional<std::string>& firstName,
                                                                          const boost::optional<std::string>& lastName) override;
            boost::shared_ptr<entities::CRecipient> getRecipient(int recipientId) override;
            boost::shared_ptr<entities::CRecipient> getRecipient(const std::string& firstName,
                                                                 const std::string& lastName) override;
            bool exists(const std::string& firstName, const std::string& lastName) override;
            bool exists(const int id) override;
            void removeRecipient(int recipientId) override;
            void removeUser(int userId) override;
            void removeAllRecipients() override;
            bool fieldExists(const std::string& fieldName, const std::string& pluginName) const override;
            void createField(const entities::CRecipientField& field) override;
            std::vector<boost::shared_ptr<entities::CRecipient>> findRecipientsFromField(const std::string& fieldName,
                                                                                         const std::string& expectedFieldValue) override;
            std::vector<boost::shared_ptr<entities::CRecipientField>> getFields() override;
            std::vector<boost::shared_ptr<entities::CRecipientField>> getFields(const boost::optional<int>& userId,
                                                                                const boost::optional<std::string>& pluginType,
                                                                                const boost::optional<std::string>& fieldName) override;
            std::vector<boost::shared_ptr<entities::CRecipientField>> getFieldsByName(const std::string& fieldName) override;
            // [END] IRecipientRequester implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   Read the recipient fields
            /// \param [in] recipientToComplete the recipient entity to complete
            //--------------------------------------------------------------
            void readRecipientFields(const boost::shared_ptr<entities::CRecipient>& recipientToComplete) const;

            //--------------------------------------------------------------
            /// \Brief		   Write the recipient fields
            /// \param [in] recipientId   the recipient id
            /// \param [in] fields        the recipient fields
            //--------------------------------------------------------------
            void writeRecipientFields(int recipientId,
                                      const std::vector<boost::shared_ptr<entities::CRecipientField>>& fields) const;

            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 
