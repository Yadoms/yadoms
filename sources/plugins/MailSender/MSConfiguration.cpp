#include "stdafx.h"
#include "MSConfiguration.h"
#include <shared/encryption/Xor.h>
#include <shared/Log.h>

CMSConfiguration::CMSConfiguration()
   : m_senderMail(""),
   m_serverName(""),
   m_serverPort(587),
   m_kSecurityMode(ESecurityMode::kTLS),
   m_bRequireAuthentication(false),
   m_userAccount(""),
   m_password("")
{
}

CMSConfiguration::~CMSConfiguration()
{
}

void CMSConfiguration::initializeWith(const shared::CDataContainer& data)
{
   try
   {
      m_data.initializeWith(data);

      m_senderMail = m_data.get<std::string>("Sender");

      if (m_data.exists("account.content.gmail.radio") && m_data.get<bool>("account.content.gmail.radio"))
      {
         m_serverName = "smtp.gmail.com";
         m_serverPort = 587;
         m_kSecurityMode = ESecurityMode::kTLS;
         m_bRequireAuthentication = true;
         m_userAccount = m_senderMail;
         m_password = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("account.content.gmail.content.password"));
      }
      else if (m_data.exists("account.content.outlook.radio") && m_data.get<bool>("account.content.outlook.radio"))
      {
         m_serverName = "smtp-mail.outlook.com";
         m_serverPort = 587;
         m_kSecurityMode = ESecurityMode::kTLS;
         m_bRequireAuthentication = true;
         m_userAccount = m_senderMail;
         m_password = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("account.content.outlook.content.password"));
      }
      else if (m_data.exists("account.content.ovh.radio") && m_data.get<bool>("account.content.ovh.radio"))
      {
         m_serverName = "ns0.ovh.net";
         m_serverPort = 587;
         m_kSecurityMode = ESecurityMode::kNone;
         m_bRequireAuthentication = true;
         m_userAccount = m_senderMail;
         m_password = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("account.content.ovh.content.password"));
      }
      else if (m_data.exists("account.content.yahoo.radio") && m_data.get<bool>("account.content.yahoo.radio"))
      {
         m_serverName = "smtp.mail.yahoo.com";
         m_serverPort = 587;
         m_kSecurityMode = ESecurityMode::kTLS;
         m_bRequireAuthentication = true;
         m_userAccount = m_senderMail;
         m_password = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("account.content.yahoo.content.password"));
      }
      else
      {
         m_serverName = m_data.get<std::string>("account.content.other.content.SMTPServer");
         m_serverPort = m_data.get<Poco::UInt16>("account.content.other.content.SMTPPort");
         m_kSecurityMode = m_data.get<ESecurityMode>("account.content.other.content.Security");
         m_bRequireAuthentication = m_data.get<bool>("account.content.other.content.authentication.checkbox");
         m_userAccount = m_data.get<std::string>("account.content.other.content.authentication.content.user");
         m_password = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("account.content.other.content.authentication.content.password"));
      }
   }
   catch (shared::exception::CInvalidParameter& ip)
   {
      YADOMS_LOG(error) << "Invalid configuration : " << ip.what() ;
   }
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(error) << "ERROR : Plugin Mail Sender could not be loaded" ;
   }
}

std::string CMSConfiguration::getSenderEmail() const
{
   return m_senderMail;
}

std::string CMSConfiguration::getHost() const
{
   return m_serverName;
}

Poco::UInt16 CMSConfiguration::getPort() const
{
   return m_serverPort;
}

std::string CMSConfiguration::getLogin() const
{
   return m_userAccount;
}

std::string CMSConfiguration::getPassword() const
{
   return m_password;
}

ESecurityMode CMSConfiguration::getSecurityMode() const
{
   return m_kSecurityMode;
}

bool CMSConfiguration::getAuthenticationRequired() const
{
   return m_bRequireAuthentication;
}

