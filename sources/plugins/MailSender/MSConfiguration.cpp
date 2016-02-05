#include "stdafx.h"
#include "MSConfiguration.h"
#include <shared/StringExtension.h>
#include <shared/Log.h>
#include <shared/encryption/Xor.h>

CMSConfiguration::CMSConfiguration()
   : m_SenderMail(""), m_ServerName(""), m_ServerPort(587), m_kSecurityMode(ESecurityMode::kTLS), m_bRequireAuthentication(false), m_UserAccount(""), m_Password("")
{}

CMSConfiguration::~CMSConfiguration()
{}

void CMSConfiguration::initializeWith(const shared::CDataContainer &data)
{
   try
   {
      m_data.initializeWith(data);

      m_SenderMail = m_data.get<std::string>("Sender");

      if (m_data.exists("account.content.gmail"))
      {
         m_ServerName = "smtp.gmail.com";
         m_ServerPort = 587;
         m_kSecurityMode = ESecurityMode::kTLS;
         m_bRequireAuthentication = true;
         m_UserAccount = m_SenderMail;
         m_Password = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("account.content.gmail.content.password"));
      }
      else if (m_data.exists("account.content.ovh"))
      {
         m_ServerName = "ns0.ovh.net";
         m_ServerPort = 587;
         m_kSecurityMode = ESecurityMode::kNone;
         m_bRequireAuthentication = true;
         m_UserAccount = m_SenderMail;
         m_Password = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("account.content.ovh.content.password"));
      }
      else if (m_data.exists("account.content.other"))
      {
         m_ServerName = m_data.get<std::string>("account.content.other.SMTPServer");
         m_ServerPort = m_data.get<Poco::UInt16>("account.content.other.SMTPPort");
         m_kSecurityMode = m_data.get<ESecurityMode>("account.content.other.Security");
         m_bRequireAuthentication = m_data.get<bool>("account.content.other.authentication.checkbox");
         m_UserAccount = m_data.get<std::string>("account.content.other.authentication.content.user");
         m_Password = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("account.content.other.content.authentication.content.password"));
      }
   }
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(error) << "ERROR : Plugin Mail Sender could not be loaded" << std::endl;
   }
}

std::string CMSConfiguration::getSenderEmail() const
{
   return m_SenderMail;
}
std::string CMSConfiguration::getHost() const
{
   return m_ServerName;
}

Poco::UInt16 CMSConfiguration::getPort() const
{
   return m_ServerPort;
}

std::string CMSConfiguration::getLogin() const
{
   return m_UserAccount;
}

std::string CMSConfiguration::getPassword() const
{
   return m_Password;
}

ESecurityMode CMSConfiguration::getSecurityMode() const
{
   return m_kSecurityMode;
}
bool CMSConfiguration::getAuthenticationRequired() const
{
   return m_bRequireAuthentication;
}

