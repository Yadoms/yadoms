#include "stdafx.h"
#include "SmtpSslServiceProvider.h"
#include <Poco/Net/NetException.h>
#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/AutoPtr.h>

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
         std::cerr << "Fail to send email with SSL : SMTPException :" << std::endl;
         std::cerr << e.displayText() << std::endl;
         session.close();
         Poco::Net::uninitializeSSL();
      }
   }
   catch (Poco::Net::NetException &e)
   {
      std::cerr << "Fail to send email with SSL : NetException :" << std::endl;
      std::cerr << e.displayText() << std::endl;
   }
   catch (Poco::Exception &e)
   {
      std::cerr << "Fail to send email with SSL : Exception" << std::endl;
      std::cerr << e.displayText() << std::endl;
   }
   catch (std::exception &e)
   {
      std::cerr << "Fail to send email with SSL : std::exception" << std::endl;
      std::cerr << e.what() << std::endl;
   }
   catch (...)
   {
      std::cerr << "Fail to send email with SSL : unknown error" << std::endl;
   }
   return false;
}