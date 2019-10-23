#include "stdafx.h"
#include "SecureSession.h"
#include <Poco/Net/Context.h>
#include <Poco/URI.h>
#include <Poco/Net/HTTPSClientSession.h>

namespace shared
{
   CSecureSession::CSecureSession(const std::string& url)
      : m_url(url)
   {
      const Poco::URI uri(url);
      Poco::Crypto::OpenSSLInitializer::initialize();
      const Poco::Net::Context::Ptr context(new Poco::Net::Context(Poco::Net::Context::CLIENT_USE,
                                                                   "",
                                                                   "",
                                                                   "",
                                                                   Poco::Net::Context::VERIFY_NONE,
                                                                   9,
                                                                   false,
                                                                   "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"));

      m_session = boost::make_shared<Poco::Net::HTTPSClientSession>(uri.getHost(), uri.getPort(), context);
   }

   void CSecureSession::setTimeout(const boost::posix_time::time_duration& timeout)
   {
      m_session->setTimeout(Poco::Timespan(static_cast<long>(timeout.seconds()), 0));
   }

   std::ostream& CSecureSession::sendRequest(Poco::Net::HTTPRequest& request)
   {
      return m_session->sendRequest(request);
   }

   std::istream& CSecureSession::receiveResponse(Poco::Net::HTTPResponse& response)
   {
      return m_session->receiveResponse(response);
   }

   const std::string& CSecureSession::getUrl() const
   {
      return m_url;
   }
} // namespace shared
