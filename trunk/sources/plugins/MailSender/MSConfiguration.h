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
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual std::string getSenderEmail() const;
   virtual std::string getHost() const;
   virtual Poco::UInt16 getPort() const;
   virtual ESecurityMode getSecurityMode() const;
   virtual bool getAuthenticationRequired() const;
   virtual std::string getLogin() const;
   virtual std::string getPassword() const;

   // [END] ISIConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;

   //--------------------------------------------------------------
   /// \brief	    Sender Mail
   //--------------------------------------------------------------
   std::string m_SenderMail;

   //--------------------------------------------------------------
   /// \brief	    Name of the server
   //--------------------------------------------------------------
   std::string m_ServerName;

   //--------------------------------------------------------------
   /// \brief	    The port of the server
   //--------------------------------------------------------------
   Poco::UInt16 m_ServerPort;

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
   std::string m_UserAccount;

   //--------------------------------------------------------------
   /// \brief	    Password
   //--------------------------------------------------------------
   std::string m_Password;
};
