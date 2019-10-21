#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/URI.h>

#include "IHttpSession.h"


namespace shared
{
   //--------------------------------------------------------------
   /// \brief	default value for HTTP Request default timeout
   //--------------------------------------------------------------
   static boost::posix_time::time_duration httpRequestDefaultTimeout(
      boost::posix_time::time_duration(boost::posix_time::seconds(45)));

   //--------------------------------------------------------------
   /// \brief	Base class for threads
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CHttpMethods
   {
   public:
      virtual ~CHttpMethods() = default;

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \param[in]  headerParameters    parameters included into the frame
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  timeout             timeout for the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static CDataContainer sendGetRequest(const std::string& url,
                                           const CDataContainer& headerParameters = shared::CDataContainer(),
                                           const CDataContainer& parameters = shared::CDataContainer(),
                                           const boost::posix_time::time_duration& timeout =
                                              httpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  uri                 the full URI
      /// \param[in]  headerParameters    parameters included into the frame
      /// \param[in]  timeout             timeout for the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static CDataContainer sendGetRequest(const Poco::URI& uri,
                                           const CDataContainer& headerParameters,
                                           const boost::posix_time::time_duration& timeout =
                                              httpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  session             the session created for this request
      /// \param[in]  headerParameters    parameters included into the frame
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  onReceive           function called on received data
      /// \param[in]  timeout             timeout for the request
      //--------------------------------------------------------------
      static void sendGetRequest(const boost::shared_ptr<IHTTPSession> session,
                                 const CDataContainer& headerParameters,
                                 const CDataContainer& parameters,
                                 boost::function1<void, CDataContainer&> onReceive,
                                 const boost::posix_time::time_duration& timeout = httpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  session             the session created for this request
      /// \param[in]  headerParameters    parameters included into the frame
      /// \param[in]  uri		          Full URI
      /// \param[in]  onReceive           function called on received data
      /// \param[in]  timeout             timeout for the request
      //--------------------------------------------------------------
      static void sendGetRequest(const boost::shared_ptr<IHTTPSession> session,
                                 const CDataContainer& headerParameters,
                                 const Poco::URI& uri,
                                 boost::function1<void, CDataContainer&> onReceive,
                                 const boost::posix_time::time_duration& timeout = httpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    JsonResponseReader
      /// \param[in]  httpresponse        the HTTP response answer
      /// \param[in]  response            the response
      /// \return     true if the response is Json, otherwise false
      //--------------------------------------------------------------
      static bool jsonResponseReader(const boost::shared_ptr<IHTTPSession> session,
                                     Poco::Net::HTTPResponse& httpresponse,
                                     CDataContainer& response);
   };
} // namespace shared
