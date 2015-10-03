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
   virtual std::string SMTPSenderMail   (void) const;
   virtual std::string SMTPServerName   (void) const;
   virtual int         SMTPServerPort   (void) const;
   virtual std::string SMTPUserAccount  (void) const;
   virtual std::string SMTPUserPassword (void) const;
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
   int m_ServerPort;

   //--------------------------------------------------------------
   /// \brief	    User account
   //--------------------------------------------------------------
   std::string m_UserAccount;

   //--------------------------------------------------------------
   /// \brief	    Password
   //--------------------------------------------------------------
   std::string m_Password;
};
