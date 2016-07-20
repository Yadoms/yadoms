#pragma once
#include <Poco/Net/MailMessage.h>

//--------------------------------------------------------------
/// \brief	Interface of SMTP server
//--------------------------------------------------------------
class ISmtpServiceProvider
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~ISmtpServiceProvider() {}

   //--------------------------------------------------------------
   /// \brief	    Send an email
   /// \param [in] message  The mail to send
   //--------------------------------------------------------------
   virtual bool sendMail(const Poco::Net::MailMessage & message) const = 0;
};
