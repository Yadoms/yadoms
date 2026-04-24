#include "stdafx.h"
#include "MSConfiguration.h"
#include <shared/encryption/Xor.h>
#include <shared/Log.h>

CMSConfiguration::CMSConfiguration()
   : m_serverPort(587),
   m_kSecurityMode(ESecurityMode::kTLS),
   m_bRequireAuthentication(false)
{
}

void CMSConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
{
   try
   {
      m_data.initializeWith(data);

      m_senderMail = m_data.get<std::string>("Sender");
      m_mailSubject = m_data.getWithDefault<std::string>("mailSubject", "");

      switch (getSelectedProvider(m_data.getChild("account")))
      {
      case kGmail:
         m_serverName = "smtp.gmail.com";
         m_serverPort = 587;
         m_kSecurityMode = ESecurityMode::kTLS;
         m_bRequireAuthentication = true;
         m_userAccount = m_senderMail;
         m_password = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("account.content.gmail.content.password"));
         break;

      case kOutlook:
         m_serverName = "smtp-mail.outlook.com";
         m_serverPort = 587;
         m_kSecurityMode = ESecurityMode::kTLS;
         m_bRequireAuthentication = true;
         m_userAccount = m_senderMail;
         m_password = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("account.content.outlook.content.password"));
         break;

      case kOvh:
         m_serverName = "ns0.ovh.net";
         m_serverPort = 587;
         m_kSecurityMode = ESecurityMode::kNone;
         m_bRequireAuthentication = true;
         m_userAccount = m_senderMail;
         m_password = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("account.content.ovh.content.password"));
         break;

      case kYahoo:
         m_serverName = "smtp.mail.yahoo.com";
         m_serverPort = 587;
         m_kSecurityMode = ESecurityMode::kTLS;
         m_bRequireAuthentication = true;
         m_userAccount = m_senderMail;
         m_password = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("account.content.yahoo.content.password"));
         break;

      case kOther:
      default:  // NOLINT(clang-diagnostic-covered-switch-default)
         m_serverName = m_data.get<std::string>("account.content.other.content.SMTPServer");
         m_serverPort = m_data.get<std::uint16_t>("account.content.other.content.SMTPPort");
         m_kSecurityMode = m_data.get<ESecurityMode>("account.content.other.content.Security");
         m_bRequireAuthentication = m_data.get<bool>("account.content.other.content.authentication.checkbox");
         m_userAccount = m_data.get<std::string>("account.content.other.content.authentication.content.user");
         m_password = shared::encryption::CXor::decryptBase64(
            m_data.get<std::string>("account.content.other.content.authentication.content.password"));
         break;
      }
   }
   catch (shared::exception::CInvalidParameter& ip)
   {
      YADOMS_LOG(error) << "Invalid configuration : " << ip.what();
   }
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(error) << "ERROR : Plugin Mail Sender could not be loaded";
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

std::uint16_t CMSConfiguration::getPort() const
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

std::string CMSConfiguration::getMailSubject() const
{
   return m_mailSubject;
}

CMSConfiguration::Provider CMSConfiguration::getSelectedProvider(const boost::shared_ptr<shared::CDataContainer>& accountNode)
{
   // New radio section format
   if (accountNode->exists("activeSection"))
   {
      const std::string activeSection = accountNode->get<std::string>("activeSection");
      if (activeSection == "gmail")
         return kGmail;
      if (activeSection == "outlook")
         return kOutlook;
      if (activeSection == "ovh")
         return kOvh;
      if (activeSection == "yahoo")
         return kYahoo;
      return kOther;
   }

   // Obsolete method (Old radio section format)
   if (accountNode->exists("content.gmail.radio") && accountNode->get<bool>("content.gmail.radio"))
      return kGmail;
   if (accountNode->exists("content.outlook.radio") && accountNode->get<bool>("content.outlook.radio"))
      return kOutlook;
   if (accountNode->exists("content.ovh.radio") && accountNode->get<bool>("content.ovh.radio"))
      return kOvh;
   if (accountNode->exists("content.yahoo.radio") && accountNode->get<bool>("content.yahoo.radio"))
      return kYahoo;
   return kOther;
}
