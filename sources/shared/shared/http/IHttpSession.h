#pragma once
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

namespace shared
{
   class IHTTPSession
   {
   public:
	   virtual void setTimeout(const Poco::Timespan& timeout) = 0;
	   virtual std::ostream& sendRequest(Poco::Net::HTTPRequest& request) = 0;
	   virtual std::istream& receiveResponse(Poco::Net::HTTPResponse& response) = 0;
	   virtual const std::string& getUrl() const = 0;
   };
} // namespace shared
