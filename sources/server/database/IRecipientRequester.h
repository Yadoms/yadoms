#pragma once

#include "entities/Entities.h"

namespace database
{
   class IRecipientRequester //TODO renommer partout "Recipient" en "User"
   {
   public:
      virtual ~IRecipientRequester() = default;

      //--------------------------------------------------------------
      /// \brief                    Add a new recipient
      /// \param [in] recipient     Recipient data
      /// \return                   The created recipient
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CRecipient> addRecipient(const entities::CRecipient& recipient) = 0;

      //--------------------------------------------------------------
      /// \brief                    Create a new user
      /// \param [in] user          User data
      /// \return                   The created user ID
      //--------------------------------------------------------------
      virtual int createUser(const entities::CRecipient& user) = 0;

      //--------------------------------------------------------------
      /// \brief                    Update a recipient
      /// \param [in] recipient     Recipient data
      /// \return                   The updated recipient
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CRecipient> updateRecipient(const entities::CRecipient& recipient) = 0;

      //--------------------------------------------------------------
      /// \brief                    Update a user
      /// \param [in] user          User data
      //--------------------------------------------------------------
      virtual void updateUser(const entities::CRecipient& user) = 0;

      //--------------------------------------------------------------
      /// \brief           List all recipients
      /// \return          List of registered recipients
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRecipient>> getRecipients() = 0;

      //--------------------------------------------------------------
      ///\brief                           Find a recipient from a specific field value
      ///\param [in] userId               The user ID
      ///\param [in] firstName            The first name to search user
      ///\param [in] lastName             The last name to search user
      ///\return                          Found users
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRecipient>> getUsers(const boost::optional<int>& userId,
                                                                            const boost::optional<std::string>& firstName,
                                                                            const boost::optional<std::string>& lastName) = 0;

      //--------------------------------------------------------------
      /// \brief                    Get one recipient
      /// \param [in] recipientId   Recipient id
      /// \return                   The specified recipient
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CRecipient> getRecipient(int recipientId) = 0;

      //--------------------------------------------------------------
      /// \brief                    Get one recipient from first and last name
      /// \param [in] firstName     Recipient first name
      /// \param [in] lastName      Recipient last name
      /// \return                   The specified recipient
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CRecipient> getRecipient(const std::string& firstName,
                                                                   const std::string& lastName) = 0;

      //--------------------------------------------------------------
      /// \brief                    Check is a recipient exists
      /// \param [in] firstName     Recipient first name
      /// \param [in] lastName      Recipient last name
      /// \return                   true if recipient exists
      //--------------------------------------------------------------
      virtual bool exists(const std::string& firstName, const std::string& lastName) = 0;

      //--------------------------------------------------------------
      /// \brief                    Check if a recipient exists
      /// \param [in] id            Recipient id
      /// \return                   true if recipient exists
      //--------------------------------------------------------------
      virtual bool exists(const int id) = 0;

      //--------------------------------------------------------------
      /// \brief                    Remove recipient 
      /// \param [in] recipientId   Recipient  Id
      /// \throw                    shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeRecipient(int recipientId) = 0;

      //--------------------------------------------------------------
      /// \brief                    Remove user 
      /// \param [in] userId        User Id
      /// \throw                    shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeUser(int userId) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove all recipients
      //--------------------------------------------------------------
      virtual void removeAllRecipients() = 0;

      //--------------------------------------------------------------
      /// \brief                    Check if a field exists
      /// \param [in] fieldName     The field name
      /// \param [in] pluginName    The plugin name (user/creator of the field)
      /// \return                   true if field exists
      //--------------------------------------------------------------
      virtual bool fieldExists(const std::string& fieldName, const std::string& pluginName) const = 0;

      //--------------------------------------------------------------
      /// \brief                    Create a new field
      /// \param [in] field         The field data
      /// \throw                    shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void createField(const entities::CRecipientField& field) = 0;

      //--------------------------------------------------------------
      /// \brief                    Update field
      /// \param [in] field         The field data
      /// \throw                    shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateField(const entities::CRecipientField& field) = 0;

      //--------------------------------------------------------------
      ///\brief                           Find a recipient from a specific field value
      ///\param [in] fieldName            The field name ("mobile", "email", etc...) where searching
      ///\param [in] expectedFieldValue   The expected field value
      ///\return                          The found recipients containing this field with this value
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRecipient>> findRecipientsFromField(const std::string& fieldName,
                                                                                           const std::string& expectedFieldValue) = 0;

      //--------------------------------------------------------------
      ///\brief                           Get all existing fields
      ///\return                          The fields list
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRecipientField>> getFields() = 0;

      //--------------------------------------------------------------
      ///\brief                           Get all existing fields
      ///\param [in] userId               Filter by user ID
      ///\param [in] pluginType           Filter by plugin type
      ///\param [in] fieldName            Filter by field name
      ///\return                          The fields list
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRecipientField>> getFields(const boost::optional<int>& userId,
                                                                                  const boost::optional<std::string>& pluginType,
                                                                                  const boost::optional<std::string>& fieldName) = 0;

      //--------------------------------------------------------------
      ///\brief                           Get all the fields with specific name
      ///\param [in] fieldName            The field name ("mobile", "email", etc...)
      ///\return                          A fields list
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRecipientField>> getFieldsByName(const std::string& fieldName) = 0;
   };
} //namespace database 
