#pragma once
#include "IHttpSession.h"
#include <Poco/Net/HTTPSClientSession.h>
#include <boost/shared_ptr.hpp>

namespace shared
{
	class SecureSession : public IHTTPSession
	{
	public:

      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in]  url             the url where to send the request
      //--------------------------------------------------------------
		explicit SecureSession(std::string url);

      // IHTTPSession implementation
		void setTimeout(const boost::posix_time::time_duration& timeout) override;
		std::ostream& sendRequest(Poco::Net::HTTPRequest& request) override;
		std::istream& receiveResponse(Poco::Net::HTTPResponse& response) override;
		const std::string& getUrl() const override;
      // [END] IHTTPSession implementation

	private:
		boost::shared_ptr<Poco::Net::HTTPSClientSession> m_session;
		std::string m_url;
	};
} // namespace shared
