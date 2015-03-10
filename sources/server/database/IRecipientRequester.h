#pragma once

#include "entities/Entities.h"

namespace database { 

   class IRecipientRequester
   {
   public:
      //--------------------------------------------------------------
      /// \brief                    Add a new recipient
      /// \param [in] recipient     Recipient data
      /// \return                   The created recipient
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CRecipient> addRecipient(const entities::CRecipient & recipient) = 0;

      //--------------------------------------------------------------
      /// \brief                    Udpate a recipient
      /// \param [in] recipient     Recipient data
      /// \return                   The updated recipient
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CRecipient> updateRecipient(const entities::CRecipient & recipient) = 0;
   
      //--------------------------------------------------------------
      /// \brief           List all recipients
      /// \return          List of registered recipients
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRecipient> > getRecipients() = 0;

      //--------------------------------------------------------------
      /// \brief                    Get one recipient
      /// \param [in] recipientId   Recipient id
      /// \return                   The specified recipient
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CRecipient> getRecipient(const int recipientId) = 0;

      //--------------------------------------------------------------
      /// \brief                    Get one recipient from first and last name
      /// \param [in] firstName     Recipient first name
      /// \param [in] lastName      Recipient last name
      /// \return                   The specified recipient
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CRecipient> getRecipient(const std::string & firstName, const std::string & lastName) = 0;
      
      //--------------------------------------------------------------
      ///\brief                           Find a recipient from a specific field value
      ///\param [in] fieldName            The field name ("phone", "email", etc...) where searching
      ///\param [in] expectedFieldValue   The expected field value
      ///\return                          The found recipients containing this field with this value
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRecipient> > findRecipientsFromField(const std::string& fieldName, const std::string& expectedFieldValue) = 0;//TODO doit plutôt retourner une liste de récipients

      //--------------------------------------------------------------
      /// \brief                    Check is a recipient exists
      /// \param [in] firstName     Recipient first name
      /// \param [in] lastName      Recipient last name
      /// \return                   true if recipient exists
      //--------------------------------------------------------------
      virtual bool exists(const std::string & firstName, const std::string & lastName) = 0;

      //--------------------------------------------------------------
      /// \brief                    Check is a recipient exists
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
      /// \brief           Remove all recipients
      //--------------------------------------------------------------
      virtual void removeAllRecipients() = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IRecipientRequester()
      {
      }
   };

 
} //namespace database 
