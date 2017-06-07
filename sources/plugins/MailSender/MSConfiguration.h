#pragma once

#include "IMSConfiguration.h"
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CMSConfiguration : public IMSConfiguration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CMSConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CMSConfiguration();

   // IMSConfiguration implementation
   void initializeWith(const shared::CDataContainer& data) override;
   std::string getSenderEmail() const override;
   std::string getHost() const override;
   Poco::UInt16 getPort() const override;
   ESecurityMode getSecurityMode() const override;
   bool getAuthenticationRequired() const override;
   std::string getLogin() const override;
   std::string getPassword() const override;
   std::string getCertificatePassphrase() const override;
   // [END] ISIConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;

   //--------------------------------------------------------------
   /// \brief	    Sender Mail
   //--------------------------------------------------------------
   std::string m_senderMail;

   //--------------------------------------------------------------
   /// \brief	    Name of the server
   //--------------------------------------------------------------
   std::string m_serverName;

   //--------------------------------------------------------------
   /// \brief	    The port of the server
   //--------------------------------------------------------------
   Poco::UInt16 m_serverPort;

   //--------------------------------------------------------------
   /// \brief	    The server securoty mode
   //--------------------------------------------------------------
   ESecurityMode m_kSecurityMode;

   //--------------------------------------------------------------
   /// \brief	    Server authentication required
   //--------------------------------------------------------------
   bool m_bRequireAuthentication;

   //--------------------------------------------------------------
   /// \brief	    User account
   //--------------------------------------------------------------
   std::string m_userAccount;

   //--------------------------------------------------------------
   /// \brief	    Password
   //--------------------------------------------------------------
   std::string m_password;

   //--------------------------------------------------------------
   /// \brief	    Certificate Passphrase
   //--------------------------------------------------------------
   std::string m_certificatePassphrase;
};

