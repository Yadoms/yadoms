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
      virtual int addRecipient(const database::entities::CRecipient & recipient) =0;
   
      //--------------------------------------------------------------
      /// \brief           List all recipients
      /// \return          List of registered recipients
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRecipient> > getRecipients() = 0;

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
