#pragma once
#include "IHttpSession.h"
#include <Poco/Net/HTTPClientSession.h>

namespace shared
{
   class CStandardSession : public IHttpSession
   {
   public:
      virtual ~CStandardSession() = default;

      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in]  url             the url where to send the request
      //--------------------------------------------------------------
      explicit CStandardSession(const std::string& url);

      // IHttpSession implementation
      void setTimeout(const boost::posix_time::time_duration& timeout) override;
      std::ostream& sendRequest(Poco::Net::HTTPRequest& request) override;
      std::istream& receiveResponse(Poco::Net::HTTPResponse& response) override;
      const std::string& getUrl() const override;
      // [END] IHttpSession implementation

   private:
      boost::shared_ptr<Poco::Net::HTTPClientSession> m_session;
      const std::string m_url;
   };
} // namespace shared
