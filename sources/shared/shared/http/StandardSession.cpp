#include "StandardSession.h"
#include <Poco/URI.h>

namespace shared
{
	StandardSession::StandardSession(std::string url):
		m_url(url)
	{
		Poco::URI uri(url.c_str());
		m_session = boost::make_shared<Poco::Net::HTTPClientSession>(uri.getHost(), uri.getPort());
	}

	void StandardSession::setTimeout(const boost::posix_time::time_duration& timeout)
	{
		m_session->setTimeout(Poco::Timespan(timeout.seconds(), 0));
	}

	std::ostream& StandardSession::sendRequest(Poco::Net::HTTPRequest& request)
	{
		return m_session->sendRequest(request);
	}

	std::istream& StandardSession::receiveResponse(Poco::Net::HTTPResponse& response)
	{
		return m_session->receiveResponse(response);
	}

	const std::string& StandardSession::getUrl() const
	{
		return m_url;
	}
} // namespace shared
