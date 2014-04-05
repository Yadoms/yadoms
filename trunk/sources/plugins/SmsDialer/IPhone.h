#pragma once
#include "ISms.h"

//--------------------------------------------------------------
/// \brief	This class has nothing to do with the famous smartphone...
///         This class represent a phone, used to send/receive SMS
//--------------------------------------------------------------
class IPhone
{  
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IPhone() {}

   //--------------------------------------------------------------
   /// \brief	                  Send a SMS
   /// \param [in] sms           The SMS to send
   //--------------------------------------------------------------
   virtual void send(boost::shared_ptr<ISms> sms) = 0;

   //--------------------------------------------------------------
   /// \brief	                  Get the next incoming message
   /// \return                   The list of incoming messages, NULL if none
   //--------------------------------------------------------------
   virtual boost::shared_ptr<std::vector<ISms> > getIncomingSMS() = 0;
};



