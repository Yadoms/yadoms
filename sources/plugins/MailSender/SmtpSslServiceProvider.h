#pragma once
#include "ISmtpServiceProvider.h"
#include "IMSConfiguration.h"
#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/SharedPtr.h>

//--------------------------------------------------------------
/// \brief	Interface of SMTP server
//--------------------------------------------------------------
class CSmtpSslServiceProvider : public ISmtpServiceProvider
{
public:
   //--------------------------------------------------------------
   /// \brief	    Create the smtp server connection
   /// \param [in]   smtpConfiguration  The smtp server configuration
   //--------------------------------------------------------------
   explicit CSmtpSslServiceProvider(boost::shared_ptr<IMSConfiguration>& smtpConfiguration, Poco::SharedPtr<Poco::Net::PrivateKeyPassphraseHandler> certificatePassphraseHandler);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSmtpSslServiceProvider();

   // ISmtpServiceProvider Implementation 
   bool sendMail(const Poco::Net::MailMessage& message) const override;
   // [END] - ISmtpServiceProvider Implementation 

private:
   boost::shared_ptr<IMSConfiguration> m_smtpConfiguration;
   Poco::SharedPtr<Poco::Net::PrivateKeyPassphraseHandler> m_certificatePassphraseHandler;
};

