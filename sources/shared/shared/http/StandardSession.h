#pragma once
#include "IHttpSession.h"
#include <Poco/Net/HTTPClientSession.h>

namespace shared
{
	class StandardSession : public IHTTPSession
	{
	public:
		explicit StandardSession(std::string url);
		void setTimeout(const Poco::Timespan& timeout) override;
		std::ostream& sendRequest(Poco::Net::HTTPRequest& request) override;
		std::istream& receiveResponse(Poco::Net::HTTPResponse& response) override;
		const std::string& getUrl() const override;
	private:
		boost::shared_ptr<Poco::Net::HTTPClientSession> m_session;
		std::string m_url;
	};
} // namespace shared
