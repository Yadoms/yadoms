#pragma once
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPResponse.h>

namespace shared
{
   class IHTTPSession
   {
   public:
      virtual ~IHTTPSession() = default;

      //--------------------------------------------------------------
      /// \brief	    set the time out time for the request response
      /// \param[in]  timeout             the timeout for the request
      //--------------------------------------------------------------
	   virtual void setTimeout(const boost::posix_time::time_duration& timeout) = 0;

      //--------------------------------------------------------------
      /// \brief	    send the request
      /// \param[in]  request             the request
      /// \return     the stream of the request
      //--------------------------------------------------------------
	   virtual std::ostream& sendRequest(Poco::Net::HTTPRequest& request) = 0;

      //--------------------------------------------------------------
      /// \brief	    receive the response
      /// \param[in]  response           
      /// \return     the stream of the answer
      //--------------------------------------------------------------
	   virtual std::istream& receiveResponse(Poco::Net::HTTPResponse& response) = 0;

      //--------------------------------------------------------------
      /// \brief	    return the url of the session
      /// \return     the url
      //--------------------------------------------------------------
	   virtual const std::string& getUrl() const = 0;
   };
} // namespace shared
