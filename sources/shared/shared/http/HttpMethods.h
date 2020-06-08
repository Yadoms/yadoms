#pragma once
#include <shared/DataContainer.h>

#include "IHttpSession.h" //TODO virer
#include <boost/property_tree/exceptions.hpp>


namespace shared
{
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

      //--------------------------------------------------------------
      /// \brief	default value for HTTP Request default timeout
      //--------------------------------------------------------------
      static const int HttpRequestDefaultTimeoutSeconds = 45;
      static boost::posix_time::time_duration HttpRequestDefaultTimeout; //TODO virer

      CHttpMethods() = delete;
      virtual ~CHttpMethods() = default;

      //--------------------------------------------------------------
      /// \brief	    Send the proxy config
      /// \param[in]  host                The proxy server host name or IP address
      /// \param[in]  port                The proxy server TCP port (can use default port with kUseProxyDefaultPort)
      /// \param[in]  username            The proxy server user name
      /// \param[in]  password            The proxy server password
      /// \param[in]  bypassRegex         Regex defining hosts for which the proxy should be bypassed ("localhost|127\.0\.0\.1|192\.168\.0\.\d+");
      //--------------------------------------------------------------
      enum { kUseProxyDefaultPort = -1 };

      static void setGlobalProxyConfig(const std::string& host,
                                       int port,
                                       const std::string& username,
                                       const std::string& password,
                                       const std::string& bypassRegex);

      //--------------------------------------------------------------
      /// \brief	    Send get request to remote server
      /// \param[in]  url                 the url to send the request
      /// \param[in]  headerParameters    parameters included into the frame
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  sessionType         the session type to use
      /// \param[in]  timeoutSeconds      timeout for the request (seconds)
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static std::string sendGetRequest(
         const std::string& url,
         const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
         const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
         const ESessionType& sessionType = kStandard,
         int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);

      //--------------------------------------------------------------
      /// \brief	    Send get request to remote server (for JSON answer)
      /// \param[in]  url                 the url to send the request
      /// \param[in]  headerParameters    parameters included into the frame
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  sessionType         the session type to use
      /// \param[in]  timeoutSeconds      timeout for the request (seconds)
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static boost::shared_ptr<CDataContainer> sendJsonGetRequest(
         const std::string& url,
         const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
         const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
         const ESessionType& sessionType = kStandard,
         int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);

      //--------------------------------------------------------------
      /// \brief	    Send get request to remote server with response processing injection
      /// \param[in]  url                 the url to send the request
      /// \param[in]  responseHandlerFct  lambda for response processing
      /// \param[in]  headerParameters    parameters included into the frame
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  sessionType         the session type to use
      /// \param[in]  timeoutSeconds      Timeout for the request (seconds)
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static void sendGetRequest(
         const std::string& url,
         const boost::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                    const std::string& data)>& responseHandlerFct,
         const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
         const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
         const ESessionType& sessionType = kStandard,
         int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);

      //--------------------------------------------------------------
      /// \brief	    Send post request to remote server (for JSON answer)
      /// \param[in]  url                 the url to send the request
      /// \param[in]  body                the body of request
      /// \param[in]  headerParameters    parameters included into the frame
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  sessionType         the session type to use
      /// \param[in]  timeout             timeout for the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static boost::shared_ptr<CDataContainer> sendPostRequest(const std::string& url,
                                                               const std::string& body,
                                                               const CDataContainer& headerParameters =
                                                                  CDataContainer(),
                                                               const CDataContainer& parameters = CDataContainer(),
                                                               const ESessionType& sessionType = kStandard,
                                                               const boost::posix_time::time_duration& timeout =
                                                                  HttpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    Send get request to remote server with response processing injection
      /// \param[in]  url                 the url to send the request
      /// \param[in]  body                the body of request
      /// \param[in]  responseHandlerFct  lambda for response processing
      /// \param[in]  headerParameters    parameters included into the frame
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  sessionType         the session type to use
      /// \param[in]  timeout             timeout for the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static void sendPostRequest(const std::string& url,
                                  const std::string& body,
                                  const boost::function<void(const Poco::Net::HTTPResponse& response,
                                                             std::istream& receivedStream)>& responseHandlerFct,
                                  const CDataContainer& headerParameters = CDataContainer(),
                                  const CDataContainer& parameters = CDataContainer(),
                                  const ESessionType& sessionType = kStandard,
                                  const boost::posix_time::time_duration& timeout = HttpRequestDefaultTimeout);

   protected:
      //TODO virer ? Ainsi que les classes Session ?
      static boost::shared_ptr<IHttpSession> createSession(const ESessionType& sessionType,
                                                           const std::string& url,
                                                           const boost::posix_time::time_duration& timeout);

      static boost::shared_ptr<CDataContainer> processJsonResponse(
         const std::map<std::string, std::string>& receivedHeaders,
         const std::string& data);

      static std::string stringifyParameters(const std::map<std::string, std::string>& parameters);

   private:
      // Proxy settings
      static std::string ProxyHost;
      static int ProxyPort;
      static std::string ProxyUsername;
      static std::string ProxyPassword;
      static std::string ProxyBypassRegex;
   };
} // namespace shared
