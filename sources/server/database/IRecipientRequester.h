#pragma once

#include "entities/Entities.h"

namespace database { 

   class IRecipientRequester
   {
   public:
      //--------------------------------------------------------------
      /// \brief                    Add a new recipient
      /// \param [in] recipient     Recipient data
      /// \return                   Id of created recipient
      //--------------------------------------------------------------
      virtual int addRecipient(const entities::CRecipient & recipient) =0;
   
      //--------------------------------------------------------------
      /// \brief           List all recipients
      /// \return          List of registered recipients
      //--------------------------------------------------------------
      virtual std::vector<const boost::shared_ptr<const entities::CRecipient> > getRecipients() const = 0;

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
