#include "stdafx.h"
#include "SmtpSslServiceProvider.h"
#include <Poco/Net/NetException.h>
#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/AutoPtr.h>

#include <shared/Log.h>

CSmtpSslServiceProvider::CSmtpSslServiceProvider(boost::shared_ptr<IMSConfiguration> & smtpConfiguration)
   :m_smtpConfiguration(smtpConfiguration)
{
}

CSmtpSslServiceProvider::~CSmtpSslServiceProvider()
{
}

bool CSmtpSslServiceProvider::sendMail(const Poco::Net::MailMessage & message) const
{
   try
   {
      Poco::Net::initializeSSL();
      Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrHandler = new Poco::Net::AcceptCertificateHandler(false);
      Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
      Poco::Net::SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);

      Poco::Net::SocketAddress sa(m_smtpConfiguration->getHost(), m_smtpConfiguration->getPort());
      Poco::Net::SecureStreamSocket socket(sa);
      Poco::Net::SMTPClientSession session(socket);

      try
      {
         if (m_smtpConfiguration->getAuthenticationRequired())
            session.login(Poco::Net::SMTPClientSession::AUTH_LOGIN, m_smtpConfiguration->getLogin(), m_smtpConfiguration->getPassword());
         else
            session.login();

         session.sendMessage(message);
         session.close();
         Poco::Net::uninitializeSSL();
         return true;
      }
      catch (Poco::Net::SMTPException &e)
      {
         YADOMS_LOG(error) << "Fail to send email with SSL : SMTPException :";
         YADOMS_LOG(error) << e.displayText();
         session.close();
         Poco::Net::uninitializeSSL();
      }
   }
   catch (Poco::Net::NetException &e)
   {
      YADOMS_LOG(error) << "Fail to send email with SSL : NetException :";
      YADOMS_LOG(error) << e.displayText();
   }
   catch (Poco::Exception &e)
   {
      YADOMS_LOG(error) << "Fail to send email with SSL : Exception";
      YADOMS_LOG(error) << e.displayText();
   }
   catch (std::exception &e)
   {
      YADOMS_LOG(error) << "Fail to send email with SSL : std::exception";
      YADOMS_LOG(error) << e.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send email with SSL : unknown error";
   }
   return false;
}