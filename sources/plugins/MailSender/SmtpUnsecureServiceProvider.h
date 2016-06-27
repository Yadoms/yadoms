#pragma once
#include "ISmtpServiceProvider.h"
#include "IMSConfiguration.h"

//--------------------------------------------------------------
/// \brief	Interface of SMTP server
//--------------------------------------------------------------
class CSmtpUnsecureServiceProvider : public ISmtpServiceProvider
{
public:
   //--------------------------------------------------------------
   /// \brief	    Create the smtp server connection
   /// \param [in]   smtpConfiguration  The smtp server configuration
   //--------------------------------------------------------------
   explicit CSmtpUnsecureServiceProvider(boost::shared_ptr<IMSConfiguration>& smtpConfiguration);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSmtpUnsecureServiceProvider();

   // ISmtpServiceProvider Implementation 
   bool sendMail(const Poco::Net::MailMessage& message) const override;
   // [END] - ISmtpServiceProvider Implementation 

private:
   boost::shared_ptr<IMSConfiguration> m_smtpConfiguration;
};

