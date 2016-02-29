#pragma once
#include "ISmtpServiceProvider.h"
#include "IMSConfiguration.h"

//--------------------------------------------------------------
/// \brief	Interface of SMTP server
//--------------------------------------------------------------
class CSmtpTlsServiceProvider : public ISmtpServiceProvider
{
public:
   //--------------------------------------------------------------
   /// \brief	    Create the smtp server connection
   /// \param [in]   smtpConfiguration  The smtp server configuration
   //--------------------------------------------------------------
   explicit CSmtpTlsServiceProvider(boost::shared_ptr<IMSConfiguration> & smtpConfiguration);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSmtpTlsServiceProvider();
   
   // ISmtpServiceProvider Implementation 
   virtual bool sendMail(const Poco::Net::MailMessage & message) const;
   // [END] - ISmtpServiceProvider Implementation 
   
private:
   boost::shared_ptr<IMSConfiguration> m_smtpConfiguration;
};
