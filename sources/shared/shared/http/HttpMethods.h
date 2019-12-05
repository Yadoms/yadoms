#pragma once
#include <shared/DataContainer.h>
#include <Poco/Net/HTTPClientSession.h>

#include "IHttpSession.h"
#include <boost/property_tree/exceptions.hpp>


namespace shared
{
   //--------------------------------------------------------------
   /// \brief	default value for HTTP Request default timeout
   //--------------------------------------------------------------
   static boost::posix_time::time_duration HttpRequestDefaultTimeout(boost::posix_time::seconds(45));

   //--------------------------------------------------------------
   /// \brief	Base class for threads
   //--------------------------------------------------------------
   class CHttpMethods
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Session type to use with request
      //--------------------------------------------------------------
      enum ESessionType
      {
         kStandard,
         kSecured
      };


      virtual ~CHttpMethods() = default;

      //--------------------------------------------------------------
      /// \brief	    Send get request to remote server
      /// \param[in]  url                 the url to send the request
      /// \param[in]  headerParameters    parameters included into the frame
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  sessionType         the session type to use
      /// \param[in]  timeout             timeout for the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static CDataContainer sendGetRequest(const std::string& url,
                                           const CDataContainer& headerParameters = CDataContainer(),
                                           const CDataContainer& parameters = CDataContainer(),
                                           const ESessionType& sessionType = kStandard,
                                           const boost::posix_time::time_duration& timeout = HttpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    Send post request to remote server
      /// \param[in]  url                 the url to send the request
      /// \param[in]  headerParameters    parameters included into the frame
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  body                the body of request
      /// \param[in]  sessionType         the session type to use
      /// \param[in]  timeout             timeout for the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static CDataContainer sendPostRequest(const std::string& url,
                                            const CDataContainer& headerParameters = CDataContainer(),
                                            const CDataContainer& parameters = CDataContainer(),
                                            const std::string& body = std::string(),
                                            const ESessionType& sessionType = kStandard,
                                            const boost::posix_time::time_duration& timeout = HttpRequestDefaultTimeout);

   protected:
      static boost::shared_ptr<IHttpSession> createSession(const ESessionType& sessionType,
                                                           const std::string& url,
                                                           const boost::posix_time::time_duration& timeout);

      static CDataContainer processResponse(Poco::Net::HTTPResponse& response,
                                            std::istream& receivedStream);

      static CDataContainer processJsonResponse(Poco::Net::HTTPResponse& response,
                                                std::istream& receivedStream);
      static CDataContainer processXmlElements(const boost::property_tree::ptree& node);
      static CDataContainer processXmlResponse(Poco::Net::HTTPResponse& response,
                                               std::istream& receivedStream);
      static CDataContainer processTextResponse(Poco::Net::HTTPResponse& response,
                                                std::istream& receivedStream);
      static CDataContainer processRawResponse(Poco::Net::HTTPResponse& response,
                                               std::istream& receivedStream);
   };
} // namespace shared
