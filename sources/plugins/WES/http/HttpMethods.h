#pragma once
#include <shared/DataContainer.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPResponse.h>
#include "httpContext.h"

namespace http
{
   //--------------------------------------------------------------
   /// \brief	Base class for Http Methods
   //--------------------------------------------------------------
   class CHttpMethods
   {
   public:

      //--------------------------------------------------------------
      /// \brief	default value for HTTP Request default timeout
      //--------------------------------------------------------------
      static boost::posix_time::time_duration httpRequestDefaultTimeout;

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \param[in]  parameters          parameters at the end of the url
	  /// \param[in]  context             context specific for each equipment for sending information
      /// \param[in]  timeout             timeout for the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static boost::shared_ptr<shared::CDataContainer> SendGetRequest(
		  const std::string & url,
          const boost::shared_ptr<shared::CDataContainer>& parameters,
		    http::httpContext& context,
          const boost::posix_time::time_duration& timeout = httpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \param[in]  parameters          parameters at the end of the url
	  /// \param[in]  context             context specific for each equipment for sending information
      /// \param[in]  timeout             timeout for the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static boost::shared_ptr<shared::CDataContainer> SendGetRequest(
		  const std::string & url,
          const boost::shared_ptr<shared::CDataContainer>& credentials,
          const boost::shared_ptr<shared::CDataContainer>& parameters,
		    http::httpContext& context,
          const boost::posix_time::time_duration& timeout = httpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  onReceive           function called on received data
	  /// \param[in]  context             context specific for each equipment for sending information
      /// \param[in]  timeout             timeout for the request
      /// \return     false if the time has expired. In this case the onReceived is not executed
      //--------------------------------------------------------------
      static bool SendGetRequest(
		  const std::string & url, 
          const boost::shared_ptr<shared::CDataContainer>& credentials,
          const boost::shared_ptr<shared::CDataContainer>& parameters,
          boost::function1<void, boost::shared_ptr<shared::CDataContainer>&> onReceive,
		    http::httpContext& context,
          const boost::posix_time::time_duration& timeout = httpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    XmlResponseReader
      /// \param[in]  httpresponse        the HTTP response answer
      /// \param[in]  response            the response
      /// \return     true if the response is Xml, otherwise false
      //--------------------------------------------------------------
      static bool XmlResponseReader(std::istream& stream,
                                    Poco::Net::HTTPResponse& httpresponse,
                                    boost::shared_ptr<shared::CDataContainer>& treeResponse);
   };
} // namespace http