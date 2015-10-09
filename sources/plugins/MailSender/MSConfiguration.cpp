#include "stdafx.h"
#include "MSConfiguration.h"
#include <shared/StringExtension.h>
#include <shared/Log.h>
#include <shared/encryption/Xor.h>

CMSConfiguration::CMSConfiguration(): m_SenderMail(""), m_ServerName (""), m_ServerPort(587), m_UserAccount(""), m_Password("")
{}

CMSConfiguration::~CMSConfiguration()
{}

void CMSConfiguration::initializeWith(const shared::CDataContainer &data)
{
   try
   {
       m_data.initializeWith(data);

       //TODO : Ce premier dans JSON doit avoir un contrôle de format mail.
       m_SenderMail    = m_data.get<std::string>("Sender");
       m_ServerName    = m_data.get<std::string>("SMTPServer");
       m_ServerPort    = m_data.get<int>("SMTPPort");
       m_UserAccount   = m_data.get<std::string>("User");
       m_Password      = shared::encryption::CXor::decryptBase64(m_data.get<std::string>("Password"));

   }
   catch (boost::thread_interrupted&)
   {
	YADOMS_LOG(error) << "ERROR : Plugin Mail Sender could not be loaded"  << std::endl;
   }
}

std::string CMSConfiguration::SMTPSenderMail   (void) const {return m_SenderMail;}
std::string CMSConfiguration::SMTPServerName   (void) const {return m_ServerName;}
int         CMSConfiguration::SMTPServerPort   (void) const {return m_ServerPort;}
std::string CMSConfiguration::SMTPUserAccount  (void) const {return m_UserAccount;}
std::string CMSConfiguration::SMTPUserPassword (void) const {return m_Password;}
