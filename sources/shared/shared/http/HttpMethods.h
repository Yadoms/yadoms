#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPResponse.h>

namespace shared
{
   //--------------------------------------------------------------
   /// \brief	default value for HTTP Request default timeout
   //--------------------------------------------------------------
   static boost::posix_time::time_duration httpRequestDefaultTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(45)));

   //--------------------------------------------------------------
   /// \brief	Base class for threads
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CHttpMethods
   {
   public:

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static CDataContainer SendGetRequest(const std::string & url);

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  timeout             timeout for the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static CDataContainer SendGetRequest(const std::string & url, 
                                           const CDataContainer & parameters,
                                           const boost::posix_time::time_duration& timeout = httpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  onReceive           function called on received data
      /// \param[in]  timeout             timeout for the request
      /// \return     false if the time has expired. In this case the onReceived is not executed
      //--------------------------------------------------------------
      static bool SendGetRequest(const std::string & url, 
                                 const CDataContainer& parameters,
                                 boost::function1<void, CDataContainer&> onReceive,
                                 const boost::posix_time::time_duration& timeout = httpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    JsonResponseReader
      /// \param[in]  httpresponse        the HTTP response answer
      /// \param[in]  response            the response
      /// \return     true if the response is Json, otherwise false
      //--------------------------------------------------------------
      static bool JsonResponseReader(Poco::Net::HTTPClientSession& session,
                                     Poco::Net::HTTPResponse& httpresponse,
                                     CDataContainer& response);
   };
} // namespace shared
