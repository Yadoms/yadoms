#include "stdafx.h"
#include "SmtpUnsecureServiceProvider.h"
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/NetException.h>
#include <shared/Log.h>

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
         YADOMS_LOG(error) << "Fail to send email : SMTPException :" ;
         YADOMS_LOG(error) << e.displayText() ;
         session.close();
      }
   }
   catch (Poco::Net::NetException& e)
   {
      YADOMS_LOG(error) << "Fail to send email : NetException :" ;
      YADOMS_LOG(error) << e.displayText();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send email : unknown error" ;
   }
   return false;
}

