#pragma once
#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include "IMSConfiguration.h"

//--------------------------------------------------------------
/// \brief	Interface of SMTP server
//--------------------------------------------------------------
class CCertificatePassphraseProvider : public Poco::Net::PrivateKeyPassphraseHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	    Create the certificate passphrase provider
   /// \param [in]   pluginConfiguration  The plugin configuration
   //--------------------------------------------------------------
   explicit CCertificatePassphraseProvider(boost::shared_ptr<IMSConfiguration>& pluginConfiguration);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CCertificatePassphraseProvider();

   // Poco::Net::PrivateKeyPassphraseHandler Implementation 
   void onPrivateKeyRequested(const void* pSender, std::string& privateKey) override;
   // [END] - Poco::Net::PrivateKeyPassphraseHandler Implementation 

private:
   boost::shared_ptr<IMSConfiguration> m_pluginConfiguration;
};

