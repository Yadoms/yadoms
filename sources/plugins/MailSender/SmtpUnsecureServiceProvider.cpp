#include "stdafx.h"
#include "SmtpUnsecureServiceProvider.h"
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/NetException.h>

CSmtpUnsecureServiceProvider::CSmtpUnsecureServiceProvider(boost::shared_ptr<IMSConfiguration>& smtpConfiguration)
   : m_smtpConfiguration(smtpConfiguration)
{
}

CSmtpUnsecureServiceProvider::~CSmtpUnsecureServiceProvider()
{
}

bool CSmtpUnsecureServiceProvider::sendMail(const Poco::Net::MailMessage& message) const
{
   try
   {
      Poco::Net::SMTPClientSession session(m_smtpConfiguration->getHost(),
                                           m_smtpConfiguration->getPort());
      session.open();
      try
      {
         if (m_smtpConfiguration->getAuthenticationRequired())
            session.login(Poco::Net::SMTPClientSession::AUTH_LOGIN,
                          m_smtpConfiguration->getLogin(),
                          m_smtpConfiguration->getPassword());
         else
            session.login();
         session.sendMessage(message);
         session.close();
         return true;
      }
      catch (Poco::Net::SMTPException& e)
      {
         std::cerr << "Fail to send email : SMTPException :" << std::endl;
         std::cerr << e.displayText() << std::endl;
         session.close();
      }
   }
   catch (Poco::Net::NetException& e)
   {
      std::cerr << "Fail to send email : NetException :" << std::endl;
      std::cerr << e.displayText();
   }
   catch (...)
   {
      std::cerr << "Fail to send email : unknown error" << std::endl;
   }
   return false;
}

