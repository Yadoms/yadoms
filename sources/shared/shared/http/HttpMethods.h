#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <Poco/Net/HTTPClientSession.h>

#include "IHttpSession.h"


namespace shared
{
   //--------------------------------------------------------------
   /// \brief	default value for HTTP Request default timeout
   //--------------------------------------------------------------
   static boost::posix_time::time_duration HttpRequestDefaultTimeout(boost::posix_time::seconds(45));

   //--------------------------------------------------------------
   /// \brief	Base class for threads
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CHttpMethods
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Session type to use with request
      enum ESessionType
      {
         kStandard,
         kSecured
      };
      
      virtual ~CHttpMethods() = default;

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
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

   private:
      static boost::shared_ptr<IHttpSession> createSession(const ESessionType& sessionType,
                                                           const std::string& url,
                                                           const boost::posix_time::time_duration& timeout);

      static CDataContainer jsonResponseReader(Poco::Net::HTTPResponse& response,
                                               std::istream& receivedStream);
   };
} // namespace shared
