#pragma once
#include "IHttpSession.h"
#include <Poco/Net/HTTPSClientSession.h>
#include <boost/shared_ptr.hpp>

namespace shared
{
	class SecureSession : public IHTTPSession
	{
	public:
		explicit SecureSession(std::string url);
		void setTimeout(const Poco::Timespan& timeout) override;
		std::ostream& sendRequest(Poco::Net::HTTPRequest& request) override;
		std::istream& receiveResponse(Poco::Net::HTTPResponse& response) override;
		const std::string& getUrl() const override;
	private:
		boost::shared_ptr<Poco::Net::HTTPSClientSession> m_session;
		std::string m_url;
	};
} // namespace shared
