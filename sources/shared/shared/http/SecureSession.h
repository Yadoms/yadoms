#pragma once
#include "IHttpSession.h"
#include <boost/shared_ptr.hpp>

namespace shared
{
	class CSecureSession : public IHTTPSession
	{
	public:
      virtual ~CSecureSession() = default;

      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in]  url             the url where to send the request
      //--------------------------------------------------------------
		explicit CSecureSession(std::string url);

      // IHTTPSession implementation
		void setTimeout(const boost::posix_time::time_duration& timeout) override;
		std::ostream& sendRequest(Poco::Net::HTTPRequest& request) override;
		std::istream& receiveResponse(Poco::Net::HTTPResponse& response) override;
		const std::string& getUrl() const override;
      // [END] IHTTPSession implementation

	private:
		boost::shared_ptr<Poco::Net::HTTPClientSession> m_session;
		std::string m_url;
	};
} // namespace shared
